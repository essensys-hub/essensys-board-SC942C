

#include <htc.h>
#include "traitement.h"
#include "hard.h"
#include "constantes.h"
#include "struct.h"
#include "global.h"
#include "gestionentrees.h"
#include "gestionsorties.h"
#include "variateur.h"

extern  void SSP_Handler(void);
extern void vd_I2C_Setup(void);
extern void vd_Traitement_I2C(void);

__CONFIG(HS & WDTDIS); // CONFIG MICRO : HORLOGE HS / PAS DE WDG (AU DEMARRAGE)

// ----------------------- VERSION ----------------------

#define VERSION_EEPROM  1       // mémorisée en eeprom après 1ière écriture
                                // utilisée pour réinitialiser l'eeprom en cas d'évolution du mapping
                                // à incrémenter à chaque fois que le mapping eeprom change


// IR 13/03/13 : reprise mapping eeprom / ajout temps d'extinction et temps d'action
// mapping eeprom : tous boitiers 
// 0x000D   version mapping eeprom : pour gestion compatibilité eeprom
// 0x000E   test eeprom : 0x55 si initialisée au moins une fois
// 0x000F   test eeprom : 0xAA si initialisée au moins une fois
// 0x0010   sorties lampes / relais monostables (LSB)
// 0x0011   sorties lampes / relais monostables (MSB)
// 0x0012   variateur 1 : allumage en cours
// 0x0013   variateur 1 : allumage sauvegardé
// 0x0014   variateur 1 : configuration des variateurs
// 0x0015   variateur 2 : allumage en cours
// 0x0016   variateur 2 : allumage sauvegardé
// 0x0017   variateur 2 : configuration des variateurs
// 0x0018 - 0x0029  variateurs 3 à 8
// 0x002A   volet 1 : temps d'action
// 0x002B   volet 2 : temps d'action   
// 0x002C - 0x0031 : volets 3 à 8
// 0x0032   lampe 1 : temps d'extinction
// 0x0033   lampe 2 : temps d'extinction
// 0x0034 - 0x0042 : lampes 3 à 15
// 0x0042   compteur d'écriture en eeprom (etats)


//-------------------------------------- eeprom

// vérification version d'eeprom
void vd_Verif_Version (void)
{
    unsigned char   l_uc_Data;
    unsigned int    address = 0x000D;

    l_uc_Data = eeprom_read(address++);
    if (l_uc_Data != VERSION_EEPROM)
    {
        // efface les octets de test pour forcer l'initialisation de l'eeprom
        eeprom_write(address++, 0);
        eeprom_write(address, 0);
    }
}

// lecture données sauvegardées en eeprom
void vd_Lit_Config (void)
{
	unsigned char l_uc_Compteur;
//	unsigned char l_uc_Masque;
//	unsigned char data;
	unsigned int address = 0x000E;
        unsigned short  l_us_Test = 0;

 	l_us_Test = eeprom_read(address++);
	l_us_Test |= (eeprom_read(address++) << 8);
        if (l_us_Test == 0xAA55)
        {
            // eeprom déja programmée : lecture des données sauvegardées

            // lampes / relais simples

            us_SortiesRelais = eeprom_read(address++);
            us_SortiesRelais |= (eeprom_read(address++) << 8);

            // variateur

            // pour chaque variateur : allumage en cours, allumage sauvegardé et configuration
            for(l_uc_Compteur=0;l_uc_Compteur<uc_VARIATEUR_NB;l_uc_Compteur++)
            {
                st_Variateurs[l_uc_Compteur].c_Eclairage = eeprom_read(address++);
                st_Variateurs[l_uc_Compteur].c_EclairageOld = eeprom_read(address++);
                st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode = eeprom_read(address++) & 0x07;
             }

            // pour chaque volet : temps d'action
            address = 0x002A;
            for (l_uc_Compteur = 0; l_uc_Compteur < VOLET_NB; l_uc_Compteur++)
            {
                uc_TempsCdeVolet[l_uc_Compteur] = eeprom_read(address++);
            }

            // pour chaque lampe : temps d'extinction
            address = 0x0032;
            for (l_uc_Compteur = 0; l_uc_Compteur < RM_NB; l_uc_Compteur++)
            {
                uc_TempsCdeLampe[l_uc_Compteur] = eeprom_read(address++);
            }

            // compteur de surveillance écrit en dernier
            address = 0x0042;
            uc_compteur_lecture_eeprom = eeprom_read(address);

        }
        else
        {
            // eeprom mauvaise : écriture par défaut

            // lampes / relais simples

            eeprom_write(address++, 0);
            eeprom_write(address++, 0);
            us_SortiesRelais = 0;

            // variateur

            // pour chaque variateur : allumage en cours, allumage sauvegardé, configuration
            for(l_uc_Compteur=0;l_uc_Compteur<uc_VARIATEUR_NB;l_uc_Compteur++)
            {
                eeprom_write(address++, 0);
                eeprom_write(address++, 0);
                st_Variateurs[l_uc_Compteur].c_Eclairage = 0;
                st_Variateurs[l_uc_Compteur].c_EclairageOld = 0;
                eeprom_write(address++, uc_MODE_VARIATEUR);  // par défaut : mode variateur
                st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode = uc_MODE_VARIATEUR;
            }

            // pour chaque volet : temps d'action
            address = 0x002A;
            for (l_uc_Compteur = 0; l_uc_Compteur < VOLET_NB; l_uc_Compteur++)
            {
#ifdef TYPE_PIECES_D_EAU
                if(l_uc_Compteur == 3)
                {
                        // Par défaut pour store -> durée différente !
                        uc_TempsCdeVolet[l_uc_Compteur] = uc_TEMPS_CDE_STORE_DEFAUT;
                }
                else
#endif
                {
                        uc_TempsCdeVolet[l_uc_Compteur] = uc_TEMPS_CDE_VOLET_DEFAUT;
                }
                eeprom_write(address++, uc_TempsCdeVolet[l_uc_Compteur]);
            }

            // pour chaque lampe : temps d'extinction
            address = 0x0032;
            for (l_uc_Compteur = 0; l_uc_Compteur < RM_NB; l_uc_Compteur++)
            {
                eeprom_write(address++, 0);     // par défaut : pas d'extinction
                uc_TempsCdeLampe[l_uc_Compteur] = 0;
            }

            // compteur de surveillance écrit en dernier
            address = 0x0042;
            uc_compteur_lecture_eeprom = 0;
            eeprom_write(address++, uc_compteur_lecture_eeprom);

            address = 0x000D;
            eeprom_write(address++, VERSION_EEPROM);     //version
            eeprom_write(address++, 0x55);     //octet de test
            eeprom_write(address++, 0xAA);     //octet de test
        }

}

// sauvegarde des données en eeprom
// sauvegarde de l'état des lampes
// les données de configuration sont stockées lorsqu'elles sont reçues
void vd_Sauve_Etat (void)
{
	unsigned char l_uc_Compteur;
	unsigned char data;
	unsigned int address = 0x0010;

	// lampes / relais simples

	data = us_SortiesRelais & 0x00FF;
	eeprom_write(address++, data);
	data = us_SortiesRelais >> 8;
	eeprom_write(address++, data);

	// volets
	// pas de mémorisations de l'état des relais : seront mis à l'arrêt au redémarrage

	// variateurs

	// pour chaque variateur : allumage en cours et allumage sauvegardé
	for(l_uc_Compteur=0;l_uc_Compteur<uc_VARIATEUR_NB;l_uc_Compteur++)
	{
            eeprom_write(address++, st_Variateurs[l_uc_Compteur].c_Eclairage);
            eeprom_write(address++, st_Variateurs[l_uc_Compteur].c_EclairageOld);
            address++;
        }

	// compteur de surveillance écrit en dernier
        address = 0x0042;
	uc_compteur_ecriture_eeprom = eeprom_read(address)+1;
	if (uc_compteur_ecriture_eeprom >= 255)
		uc_compteur_ecriture_eeprom = 1;
	eeprom_write(address, uc_compteur_ecriture_eeprom);

}

//-------------------------------------- interruption

// Exécuté en cas d'IT (timer, I2C)
void interrupt IT(void)
{
    static unsigned char s_uc_TempoStart = 0;

// xxx essai par vrai PWM xxx 	
// bascule sur IT timer 0 : timer 2 réservé au PWM
// !!! timer 0 lié à la gestion du watch dog => passé sur timer 1
//	if((PIR1 & 0x02) != 0)
	if (T0IF==1 && T0IE==1)
// xxx fin essai par vrai PWM xxx 	
	{
		// IT Timer
		// traitement ecrit directement ici à cause de la faible profondeur de la pile
		// Cadencement traitement cycle / acquisition des entrées
		// Commande en "PWM" des relais bistable
		// Commande en "liaison série" des variateurs

		uc_FlagITTimer = 1;
		
		uc_CompteurAcq++;		// acquisition des entrées déplacée dans la tache de fond, pour que le traitement de l'IT soit court
		//uc_CompteurVar++;
	
		// commande des variateurs
                if(s_uc_TempoStart < 50)    // Tempo au start environ 20 ms : laisser le temps a la ligne de comm de stabiliser avant d'envoyer la commande
                {
                    s_uc_TempoStart++;
                }
                else
                {
                    vd_PilotageVariateurs();
                }
	
		// fin traitement IT Timer

		TMR0-=uc_TIMER_416_US;	 //[PDP_2012-10-22]_Erreur Init fixed					// timer initialisé
		T0IF=0;
	}

	/* xxx gestion chute de tension : ne marche pas sur ce hard
	// IT chute de tension 
	if((LVDIF != 0) && (LVDIE !=0))
	{
		// IT chute de tension 
		if (ub1_Tension_OK !=0)
		{	// surveillance chute de tension active
			if (ub1_Tension_Chute ==0)
			{
				vd_Sauve_Config ();
				ub1_Tension_Chute = 1;
				ub1_Tension_ChuteIT = 1;
			}
		}
		// fin traitement IT chute de tension 
		LVDIF =0;
	}
	xxx */

    // I2C
    if(SSPIF)
    {
        SSPIF = 0; // Clear the interrupt flag.

        if (SSPIE == 1)
        {
            // IT autorisée
            SSP_Handler(); // Do I2C communication
        }
    }

}

//-------------------------------------- principal

void main(void)
{
	vd_InitVariables();	// !!! A FAIRE AVANT TOUTE AUTRE FONCTION
	vd_InitMicro();
        vd_I2C_Setup();
	vd_ConfigBornier();	// lit le type de bornier pour initialiser les valeurs max
        vd_Verif_Version();     // vérification version
	vd_Lit_Config ();	// lecture des paramètres sauvegardés en eeprom
	vd_InitVariables2();	// IR 14/03/2013 init scindée pour prise en compte des valeurs lues en eeprom

        GIE = 1;
        PEIE = 1;

	// attend 1 nouveau cycle pour autoriser la sortie PWM (init du PWM)
	while(TMR2IF == 1)
	{
		PWM_INIT_OUT		// bascule en sortie à la fin du 1er cycle
		TMR2IF = 0;
	}

	while(1)
	{

		vd_ConfigBornier();		// confirme le type de bornier


                /* xxx traitement chute de tension : ne fonctionne pas sur ce hard
                // chute de tension
                if (IRVST !=0)
                {	// Indicates that the PLVD is stable and PLVD interrupt is reliable
                        if (ub1_Tension_OK == 0)
                        {	// indique "tension stable vue à la mise sous tension" => la surveillance peut commencer
                                ub1_Tension_OK = 1;
                                LVDIF =0;
                        }
                        if (LVDIE == 0)
                        {	// autorise l'interruption sur chute de tension, dès que la tension est stable à la mise sous tension
                                LVDIE = 1;	// Enables Low Voltage Detect Interrupt
                        }
                }
                if (ub1_Tension_OK !=0)
                {	// surveillance chute de tension active
                        if (LVDIF !=0)
                        {	// chute de tension détectée
                                if (ub1_Tension_Chute ==0)
                                {
                                        // xxw vd_Sauve_Config ();
                                        // xxx ub1_Tension_Chute = 1;
                                        ;	// traitement se passe en IT
                                }
                                LVDIF =0;
                        }
                }
                // xxx tests
                // if (RF1 == 0)	// entrée 3
                //	LVDIF =1;		// simulation par entrée
                xxx fin traitement chute de tension */

                // eeprom
                // IR 14/03/2013 Sauvegarde déplacée dans le traitement I2C pour prendre en compte le front uniquement
                //if (st_Commandes.ub1.Sauvegarde ==1)
                //{
                //        vd_Sauve_Etat ();
                //        st_Commandes.ub1.Sauvegarde = 0;    // raz commande une fois traitée
                //}


// Cadencement acquisition des entrées : [5ms]
// Traitement entrées DIN (anti rebond, simple appui / double appui / appui long)
// traitement fin de commande des relais bistables
		if(uc_CompteurAcq >= uc_ACQ)
		{
			uc_CompteurAcq -= uc_ACQ;	// prend en compte le retard de traitement
			if (uc_CompteurAcq > 100)
				uc_CompteurAcq = 0;		// pour sécurité et surveillance avec l'émulateur

			uc_CompteurCycle++;
		
			vd_AcquisitionEntrees();
		
			if (uc_CompteurBiStable >0)
			{	// impulsion de commande de relais bi stable en cours
				uc_CompteurBiStable--;
				if (uc_CompteurBiStable ==0)
				{	// impulsions à terminer
					vd_ArretPilotageRelaisBist();
				}
			}

		}

// Cadencement rampes variateurs : [25ms]
//		if(uc_CompteurVar >= uc_VAR)
//		{
//			uc_CompteurVar -= uc_VAR;	// prend en compte le retard de traitement
//			if (uc_CompteurVar > 100)
//				uc_CompteurVar = 0;		// pour sécurité et surveillance avec l'émulateur
//
//			vd_RampeVariateurs();
//
//		}

// Cadencement traitement cycle [40 ms]
		if(uc_CompteurCycle >= uc_CYCLE)
		{
			uc_CompteurCycle = 0;

			// gestion base de temps 1 seconde	
			uc_CompteurBase1S ++;
			if(uc_CompteurBase1S >= uc_1s_CADENCEMENT)
			{
				uc_CompteurBase1S = 0;
				b1_BT_1S = 1;			// flag seconde : à 1 chaque seconde pendant 1 cycle
			}

			vd_AnalyserEntrees();
			vd_TraitementApplicatif();
                        vd_Traitement_I2C();
			vd_PiloterSorties();

			b1_BT_1S = 0;				// flag seconde : à 1 chaque seconde pendant 1 cycle
			
			// xxx faire controle uc_FlagITTimer
			// xxx relance watchdog (+ init à faire au début)
                        /* xxx

        // Check WDT software flag to see if we need to clear the WDT. The
        // clearing of the WDT is disabled by this flag during I2C events to
        // increase reliablility of the slave I2C function. In the event that
        // a sequence on the I2C bus takes longer than expected, the WDT will
        // reset the device (and SSP module).
        if(!stat.wdtdis)
        CLRWDT();
                         * */

		}
	
	}
}
