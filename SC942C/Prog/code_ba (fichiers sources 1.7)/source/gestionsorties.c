#include <htc.h>

#include "constantes.h"
#include "struct.h"
#include "global.h"

#define extern
#include "gestionsorties.h"
#undef extern

#include "hard.h"


// Fait flasher la led d'état périodiquement
void vd_PiloterLed(void)
{
	if(uc_CompteurLED < uc_LED_CADENCEMENT)
	{
		vd_PiloterSortie(uc_SORTIE_LED,0,uc_ON);
		uc_CompteurLED++;
	}
	else if(uc_CompteurLED < uc_1s_CADENCEMENT)
	{
		vd_PiloterSortie(uc_SORTIE_LED,0,uc_OFF);
		uc_CompteurLED++;
	}
        else
        {
		vd_PiloterSortie(uc_SORTIE_LED,0,uc_ON);
		uc_CompteurLED = 1;
	}	

	// sécurité : surveillance des constantes déclarées
        /*
	if ((uc_ENTREE_TOR_NB != uc_RM_NB)
		|| (uc_ENTREE_VOLET_NB != uc_RB_NB)
		|| (uc_ENTREE_VARIATEUR_NB != uc_VARIATEUR_NB)
		|| (uc_NB_ES != (uc_ENTREE_TOR_NB + uc_ENTREE_VOLET_NB + uc_ENTREE_VARIATEUR_NB))
		)
	{
		vd_PiloterSortie(uc_SORTIE_LED,0,uc_ON);
	}
         */

}

// Pilote les relais en fonction de l'état requis par l'applicatif
void vd_PiloterRelais(void)
{
	unsigned char l_uc_Compteur;
	unsigned short l_us_Masque;				// xxx passe sur 16 bits à cause du bornier "pièces de vie"
	
	// Afin de gagner de la mémoire, l'état des relais sont mémorisés dans la variable uc_SortiesRelais : 1 bit par relais
	for(l_uc_Compteur=0;l_uc_Compteur<uc_RM_NB;l_uc_Compteur++)
	{
		l_us_Masque = 0x01 << l_uc_Compteur;
		// IR 12/03/2013 : gestion allumage forcé
                if(     ((us_SortiesRelais & l_us_Masque) == 0)
                    &&  (st_Commandes.ub1.AllumeTout == 0))
                    // extinction normale sans allumage forcé
                    vd_PiloterSortie(uc_SORTIE_RELAIS,l_uc_Compteur,uc_OFF);
		else
                    // allumage normal ou forcé
                    vd_PiloterSortie(uc_SORTIE_RELAIS,l_uc_Compteur,uc_ON);
	}	
}	

// Positionne la bonne commande et sélectionne le relais à piloter
void vd_PiloterRelaisBistable(unsigned char uc_NumeroRelaisBistable)
{
//	// le pilotage de la sortie de commande se fera dans l'IT horloge

	if(st_SortiesRelaisBistables[uc_NumeroRelaisBistable].ub1_EtatRelaisBistable == 0)
	{

		vd_PiloterSortie(uc_SORTIE_RELAIS_BISTABLE_SENS,uc_RBSENS1,0);
		CCPR2L = PWM_MSB;	// 8 msb
		CCP2CON |= PWM_LSB; // 2 lsb
		uc_CompteurBiStable = uc_DUREE_CDE_MAINTENUE_RELAIS_BISTABLE;	// calibre la durée d'impulsion de la commande du relais bistable
		// uc_CdeRelaisBistables[uc_NumeroRelaisBistable] = 0x49; // 0100 1001: "PWM" rapport 1/3
		vd_PiloterSortie(uc_SORTIE_RELAIS_BISTABLE,uc_NumeroRelaisBistable,1);
	}	
	else
	{
		vd_PiloterSortie(uc_SORTIE_RELAIS_BISTABLE_SENS,uc_RBSENS2,0);
		CCPR2L = 0;			// 8 msb
		CCP2CON &= ~0x30; 	// 2 lsb
		uc_CompteurBiStable = uc_DUREE_CDE_MAINTENUE_RELAIS_BISTABLE;	// calibre la durée d'impulsion de la commande du relais bistable
		// uc_CdeRelaisBistables[uc_NumeroRelaisBistable] = 0xFF;	// toujours 1 : impulsion maintenue
		vd_PiloterSortie(uc_SORTIE_RELAIS_BISTABLE,uc_NumeroRelaisBistable,1);
	}	
}

// Pilote les relais bistables en fonction de l'état requis par l'applicatif
void vd_PiloterRelaisBistables(void)
{
	unsigned char l_uc_Compteur;
	static unsigned char s_uc_CompteurCommandeMaintenu = uc_DUREE_COMMANDE_MAINTENUE_RELAIS_BISTABLE;
	
        // IR 13/03/2013 : gestion blocage des volets
        //if (st_Commandes.ub1.bloqueVolets == 1)
        //{
            // commande des volets bloqués par ordre du BP
        //    return;  // sortie immédiate de la fonction
        //} Bug si relais deja commandé, reste tout le temps commandé...

	if(s_uc_CompteurCommandeMaintenu < uc_DUREE_COMMANDE_MAINTENUE_RELAIS_BISTABLE)
	{
		// Pilotage en cours...
		s_uc_CompteurCommandeMaintenu++;
	}
	
	for(l_uc_Compteur=0;l_uc_Compteur<uc_RB_NB;l_uc_Compteur++)
	{
		if(s_uc_CompteurCommandeMaintenu >= uc_DUREE_COMMANDE_MAINTENUE_RELAIS_BISTABLE)
		{
                    if (st_Commandes.ub1.bloqueVolets != 0)
                    {
                        st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite = 0;
                    }

			// xxx ce compteur ne correspond plus à la durée des impulsions 
			if(st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistable != st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite)
			{
                            // l'arrêt du relais1 ne pose pas de pb
                            // par contre avant de piloter relais1, il faut s'assurer que relais2 est a 0 -> sinon mettre relais2 a 0
                            if( (l_uc_Compteur % 2) == 0 && st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite != 0)
                            {
                                if(st_SortiesRelaisBistables[l_uc_Compteur+1].ub1_EtatRelaisBistable != 0)
                                {
                                    st_SortiesRelaisBistables[l_uc_Compteur+1].ub1_EtatRelaisBistableSouhaite = 0;    // Normalement pas utile, devrait etre mis à 0 par applicatif...
                                    st_SortiesRelaisBistables[l_uc_Compteur+1].ub1_EtatRelaisBistable = st_SortiesRelaisBistables[l_uc_Compteur+1].ub1_EtatRelaisBistableSouhaite;
                                    s_uc_CompteurCommandeMaintenu = 0;
                                    vd_PiloterRelaisBistable(l_uc_Compteur+1);
                                }
                                else
                                {
                                    st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistable = st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite;
                                    s_uc_CompteurCommandeMaintenu = 0;
                                    vd_PiloterRelaisBistable(l_uc_Compteur);
                                }
                            }
                            // l'arrêt du relais2 ne pose pas de pb
                            // par contre avant de piloter relais2, il faut s'assurer que relais1 est a 0 -> sinon mettre relais1 a 0
                            else if( (l_uc_Compteur % 2) != 0 && st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite != 0)
                            {
                                if(st_SortiesRelaisBistables[l_uc_Compteur-1].ub1_EtatRelaisBistable != 0)
                                {
                                    st_SortiesRelaisBistables[l_uc_Compteur-1].ub1_EtatRelaisBistableSouhaite = 0;    // Normalement pas utile, devrait etre mis à 0 par applicatif...
                                    st_SortiesRelaisBistables[l_uc_Compteur-1].ub1_EtatRelaisBistable = st_SortiesRelaisBistables[l_uc_Compteur-1].ub1_EtatRelaisBistableSouhaite;
                                    s_uc_CompteurCommandeMaintenu = 0;
                                    vd_PiloterRelaisBistable(l_uc_Compteur-1);
                                }
                                else
                                {
                                    st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistable = st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite;
                                    s_uc_CompteurCommandeMaintenu = 0;
                                    vd_PiloterRelaisBistable(l_uc_Compteur);
                                }
                            }
                            else
                            {
				st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistable = st_SortiesRelaisBistables[l_uc_Compteur].ub1_EtatRelaisBistableSouhaite;
				s_uc_CompteurCommandeMaintenu = 0;
				vd_PiloterRelaisBistable(l_uc_Compteur);
                            }

                            break;	// sortie de la boucle suite à la 1ière commande faite
			}
		}	
	}	
}

// Arrete le pilotage de TOUS les relais bistables
void vd_ArretPilotageRelaisBist(void)
{
	unsigned char l_uc_Compteur;
	
	for(l_uc_Compteur=0;l_uc_Compteur<uc_RB_NB;l_uc_Compteur++)
	{
            vd_PiloterSortie(uc_SORTIE_RELAIS_BISTABLE,l_uc_Compteur,0);
	}
        // IR 12/10/2012 : arrêt du PWM en même temps que la commande pour éviter le 20 kHz permanent sur la carte
	CCPR2L = 0;			// 8 msb
	CCP2CON &= ~0x30; 	// 2 lsb
}

// Pilotage des Variateurs
// à partir des trames séries : par l'IT horloge
inline void vd_PilotageVariateurs(void) //[PDP_2012-10-22]
{
	static unsigned char s_uc_NumVar =0;

	if (s_uc_NumVar >= uc_VARIATEUR_NB)
		s_uc_NumVar = 0;	

	if (us_CdeVariateurs[s_uc_NumVar] != 0)
	{	// action uniquement si besoin
		if ((us_CdeVariateurs[s_uc_NumVar] &1) != 0)
		{
			// xxx vd_PiloterSortie(uc_SORTIE_VARIATEUR,l_uc_Compteur,uc_ON);	
			vd_PiloteSortieVariateur(s_uc_NumVar,uc_ON);	
		}
		else
		{
			// xxx vd_PiloterSortie(uc_SORTIE_VARIATEUR,l_uc_Compteur,uc_OFF);	
			vd_PiloteSortieVariateur(s_uc_NumVar,uc_OFF);	
		}
		us_CdeVariateurs[s_uc_NumVar] = us_CdeVariateurs[s_uc_NumVar] >>1; // liaison série => bits suivants

	}
	else
	{
		s_uc_NumVar ++;
	}
}


// Appele toutes les fonctions pilotant les sorties, après traitement applicatif (pilotage en fonction entrées)
void vd_PiloterSorties(void)
{
        //xxx visualisation de l'état de la com sur la led : pour tests
	// xxx fonction assurer par la tache I2C    vd_PiloterLed();
        vd_PiloterLed();        // Modif suite email NG 07/09/2014 01:56
	vd_PiloterRelais();
	vd_PiloterRelaisBistables();
	//vd_PiloterVariateurs();	xxx
}	

	