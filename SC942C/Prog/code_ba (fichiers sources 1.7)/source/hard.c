
#include <htc.h>

#include "constantes.h"
#include "struct.h"

#define extern
#include "global.h"
#include "hard.h"
#undef extern



void vd_InitMicro(void)
{

	ANSEL = 0;	// Désactive toutes les entrées analogiques (sinon empeche d'utiliser certains ports en TOR)
	LCDCON = 0x03;	// Désactive LCD Bias Voltage
//        OSCCON = 0b01110001;
        OSCCON = 0b00000000; // external clock

	CCP2CON = 0x0C;		// CCP en mode PWM
	CCPR2L = PWM_MSB;	// 8 msb
	CCP2CON |= PWM_LSB; // 2 lsb

	// le Timer2 est utilisé pour le PWM (ressource réservée)
	// Init Timer 2 : PWM période
	PR2 = PWM_PER;	//
	TMR2IF = 0;		// RAZ éventuel IT
	TMR2IE = 0;		// interdit IT Timer 2
	T2CON = 0x04;	// Timer2 ON - Prescaler : 1/1 - Postcaler : non utilisé

	// Init Timer 0 (cadencement traitement)
	// Configuration OPTION_REG
        OPTION_REG=0b10000011;                                                  // PDP
/*	OPTION_REG|=0x80;							// GPPU : GPIO Pull-ups disabled
	OPTION_REG&=~0x40;							// INTEDG : It sur front descendant nI_OVER_CURR
	OPTION_REG&=~0x20;							// T0CS : Internal Clk pour TMR0
	OPTION_REG&=~0x10;							// T0SE :
	OPTION_REG&=~0x08;							// PSA : Pre-scaler sur TMR0
	OPTION_REG&=~0x04;							// PS2 : Pre-scale = 1:4
	OPTION_REG&=~0x02;							// PS1 :
	OPTION_REG|=0x01;							// PS0 :
*/
	TMR0=uc_TIMER_416_US;						// timer initialisé
	T0IF = 0;									// raz bit d'IT avant autorisation de l'IT
	T0IE = 1;									// autorise IT timer 0

	// détection de chute de tension : non implémenté
	LVDCON = 0x16;		// low voltage detection level : 4,2 V (LVDL = 110b)
						// low voltage detect module enable (LVDEN = 1)

        // I2C esclave
	TRISC |= 0x0C0;     // SCL and SDA pins must be configured as inputs (TRISC<7,6> are set)
// xxx        SSPADD = I2C_ADDR;  // adresse I2C du boitier
        SSPADD = I2C_ADDR<<1;  // adresse I2C du boitier
        SSPSTAT = 0;
        SSPCON = 0;
        SSPCON = 0x36;      // Enables serial port / Enable clock / I2C Slave mode, 7-bit address
// xxx        SSPCON = 0x3E;      // Enables serial port / Enable clock / I2C Slave mode, 7-bit address
        SSPIF = 0;          // RAZ éventuel IT
        SSPIE = 1;          // autorise IT

        
	ENTREE0_INIT;	ENTREE1_INIT;	ENTREE2_INIT;	ENTREE3_INIT;
	ENTREE4_INIT;	ENTREE5_INIT;	ENTREE6_INIT;	ENTREE7_INIT;
	ENTREE8_INIT;	ENTREE9_INIT;	ENTREE10_INIT;	ENTREE11_INIT;
	ENTREE12_INIT;	ENTREE13_INIT;	ENTREE14_INIT;	ENTREE15_INIT;
	ENTREE16_INIT;	ENTREE17_INIT;	ENTREE18_INIT;	ENTREE19_INIT;
	ENTREE20_INIT;	ENTREE21_INIT;

	LED_INIT;
	RM0_INIT;	RM1_INIT;	RM2_INIT;	RM3_INIT;	RM4_INIT;
	RM5_INIT;	RM6_INIT;	RM7_INIT;	RM8_INIT;	RM9_INIT;
	RM10_INIT;	RM11_INIT;	RM12_INIT;	
	RBSENS1_INIT;	RBSENS2_INIT;
	RB0_INIT;	RB1_INIT;	RB2_INIT;	RB3_INIT;
	RB4_INIT;	RB5_INIT;	RB6_INIT;	RB7_INIT;
	RB8_INIT;	RB9_INIT;	RB10_INIT;	RB11_INIT;
	VARIATEUR0_INIT;	VARIATEUR1_INIT;	VARIATEUR2_INIT;	VARIATEUR3_INIT;
        VARIATEUR0_ON;          VARIATEUR1_ON;          VARIATEUR2_ON;          VARIATEUR3_ON;

	PORTA = s_uc_PortA;
	PORTB = s_uc_PortB;
	PORTC = s_uc_PortC;
	PORTD = s_uc_PortD;
	PORTE = s_uc_PortE;
	PORTF = s_uc_PortF;

	//INTCON |= 0xC0;	// Autorise les IT (GIE + PEIE)
        //INTCON = 0b11011000;	//PDP
        
    CLRWDT();   // xxxx
}


void	vd_ConfigBornier(void)
{
	// initialise les nombres d'entrées/sorties pour utiliser à la place des constantes dans les fonctions

// sur le 1er proto, la configuration était lue sur 2 entrées de configuration et la différenciation des boitiers
// se faisait en soft.
// Les entrées de configuration n'existent plus et 3 binaires coexistent (directives d'assemblage)
// Pour autant, les structures mémoires et code sont conservées
// à revoir si la taille mémoire devient critique
    
#ifdef TYPE_CHAMBRES
//case BORNIER_CHAMBRES:
    uc_NB_ES = 21;
    uc_ENTREE_TOR_NB = 7;
    uc_RM_NB = 7;
    uc_ENTREE_VARIATEUR_NB = 4;
    uc_VARIATEUR_NB = 4;
    uc_ENTREE_VOLET_NB = 10;
    uc_RB_NB = 10;
    uc_VOLET_NB = uc_RB_NB >> 1;		// 1 volet ou store pour 2 relais
//break;
#endif

#ifdef TYPE_PIECES_DE_VIE
//case BORNIER_PIECES_VIE:
    uc_NB_ES = 20;
    uc_ENTREE_TOR_NB = 5;
    uc_RM_NB = 5;
    uc_ENTREE_VARIATEUR_NB = 3;
    uc_VARIATEUR_NB = 3;
    uc_ENTREE_VOLET_NB = 12;
    uc_RB_NB = 12;
    uc_VOLET_NB = uc_RB_NB >> 1;		// 1 volet ou store pour 2 relais
//break;
#endif

#ifdef TYPE_PIECES_D_EAU
// case BORNIER_PIECES_EAU:
//default :
    uc_NB_ES = 22;
    uc_ENTREE_TOR_NB = 13;
    uc_RM_NB = 13;
    uc_ENTREE_VARIATEUR_NB = 1;
    uc_VARIATEUR_NB = 1;
    uc_ENTREE_VOLET_NB = 8;
    uc_RB_NB = 8;
    uc_VOLET_NB = uc_RB_NB >> 1;		// 1 volet ou store pour 2 relais
//break;
#endif

	// sécurité : vérifie par rapport aux constantes qui affectent l'espace mémoire
	if (uc_NB_ES > NB_ES)
		uc_NB_ES = NB_ES;
	if (uc_ENTREE_TOR_NB > ENTREE_TOR_NB)
		uc_ENTREE_TOR_NB = ENTREE_TOR_NB;
	if (uc_RM_NB > RM_NB)
		uc_RM_NB = RM_NB;
	if (uc_ENTREE_VARIATEUR_NB > ENTREE_VARIATEUR_NB)
		uc_ENTREE_VARIATEUR_NB = ENTREE_VARIATEUR_NB;
	if (uc_VARIATEUR_NB > VARIATEUR_NB)
		uc_VARIATEUR_NB = VARIATEUR_NB;
	if (uc_ENTREE_VOLET_NB > ENTREE_VOLET_NB)
		uc_ENTREE_VOLET_NB = ENTREE_VOLET_NB;
	if (uc_RB_NB > RB_NB)
		uc_RB_NB = RB_NB;
	if (uc_VOLET_NB > VOLET_NB)
		uc_VOLET_NB = VOLET_NB;
}

unsigned char uc_EtatEntree(unsigned char uc_NumeroEntree)
{
	unsigned char l_uc_EtatEntree;

	l_uc_EtatEntree = 0;
	switch(uc_NumeroEntree)
	{
		case uc_ENTREE0:
			if((ENTREE0_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE1:
			if((ENTREE1_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE2:
			if((ENTREE2_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE3:
			if((ENTREE3_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE4:
			if((ENTREE4_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE5:
			if((ENTREE5_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE6:
			if((ENTREE6_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE7:
			if((ENTREE7_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE8:
			if((ENTREE8_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE9:
			if((ENTREE9_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE10:
			if((ENTREE10_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE11:
			if((ENTREE11_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE12:
			if((ENTREE12_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE13:
			if((ENTREE13_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE14:
			if((ENTREE14_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE15:
			if((ENTREE15_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE16:
			if((ENTREE16_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE17:
			if((ENTREE17_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE18:
			if((ENTREE18_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE19:
			if((ENTREE19_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE20:
			if((ENTREE20_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
		case uc_ENTREE21:
			if((ENTREE21_ETAT) != 0)	l_uc_EtatEntree = 1;
		break;
	}

	// xxx inversion etat (carte microchip)
	if(l_uc_EtatEntree == 0)	l_uc_EtatEntree = 1;
	else						l_uc_EtatEntree = 0;

	return(l_uc_EtatEntree);
}

// Pilote toutes les sorties du micro
void vd_PiloterSortie(unsigned char uc_TypeSortie,unsigned char uc_NumeroSortie,unsigned char uc_Etat)
{
//xxx	static unsigned char s_uc_PortA = 0;
//xxx	static unsigned char s_uc_PortB = 0;
//xxx	static unsigned char s_uc_PortC = 0;
//xxx	static unsigned char s_uc_PortD = 0;
//xxx	static unsigned char s_uc_PortE = 0;
//xxx	static unsigned char s_uc_PortF = 0;
	
	switch(uc_TypeSortie)
	{
		case uc_SORTIE_RELAIS:
			switch(uc_NumeroSortie)
			{
				case uc_RM0:
					if(uc_Etat != 0)	RM0_ON;
					else				RM0_OFF;
				break;
				case uc_RM1:
					if(uc_Etat != 0)	RM1_ON;
					else				RM1_OFF;
				break;
				case uc_RM2:
					if(uc_Etat != 0)	RM2_ON;
					else				RM2_OFF;
				break;
				case uc_RM3:
					if(uc_Etat != 0)	RM3_ON;
					else				RM3_OFF;
				break;
				case uc_RM4:
					if(uc_Etat != 0)	RM4_ON;
					else				RM4_OFF;
				break;
				case uc_RM5:
					if(uc_Etat != 0)	RM5_ON;
					else				RM5_OFF;
				break;
				case uc_RM6:
					if(uc_Etat != 0)	RM6_ON;
					else				RM6_OFF;
				break;
				case uc_RM7:
					if(uc_Etat != 0)	RM7_ON;
					else				RM7_OFF;
				break;
				case uc_RM8:
					if(uc_Etat != 0)	RM8_ON;
					else				RM8_OFF;
				break;
				case uc_RM9:
					if(uc_Etat != 0)	RM9_ON;
					else				RM9_OFF;
				break;
				case uc_RM10:
					if(uc_Etat != 0)	RM10_ON;
					else				RM10_OFF;
				break;
				case uc_RM11:
					if(uc_Etat != 0)	RM11_ON;
					else				RM11_OFF;
				break;
				case uc_RM12:
					if(uc_Etat != 0)	RM12_ON;
					else				RM12_OFF;
				break;
			}
		break;
		case uc_SORTIE_RELAIS_BISTABLE_SENS:
			switch(uc_NumeroSortie)
			{
				case uc_RBSENS1:
					RBSENS1_ON;
					RBSENS2_OFF;
				break;
				case uc_RBSENS2:
					RBSENS1_OFF;
					RBSENS2_ON;
				break;
			}
		break;
		case uc_SORTIE_RELAIS_BISTABLE:
			switch(uc_NumeroSortie)
			{
				case uc_RB0:
					if(uc_Etat != 0)	RB0_ON;
					else				RB0_OFF;
				break;
				case uc_RB1:
					if(uc_Etat != 0)	RB1_ON;
					else				RB1_OFF;
				break;
				case uc_RB2:
					if(uc_Etat != 0)	RB2_ON;
					else				RB2_OFF;
				break;
				case uc_RB3:
					if(uc_Etat != 0)	RB3_ON;
					else				RB3_OFF;
				break;
				case uc_RB4:
					if(uc_Etat != 0)	RB4_ON;
					else				RB4_OFF;
				break;
				case uc_RB5:
					if(uc_Etat != 0)	RB5_ON;
					else				RB5_OFF;
				break;
				case uc_RB6:
					if(uc_Etat != 0)	RB6_ON;
					else				RB6_OFF;
				break;
				case uc_RB7:
					if(uc_Etat != 0)	RB7_ON;
					else				RB7_OFF;
				break;
				case uc_RB8:
					if(uc_Etat != 0)	RB8_ON;
					else				RB8_OFF;
				break;
				case uc_RB9:
					if(uc_Etat != 0)	RB9_ON;
					else				RB9_OFF;
				break;
				case uc_RB10:
					if(uc_Etat != 0)	RB10_ON;
					else				RB10_OFF;
				break;
				case uc_RB11:
					if(uc_Etat != 0)	RB11_ON;
					else				RB11_OFF;
				break;
			}
		break;
		case uc_SORTIE_VARIATEUR:
			switch(uc_NumeroSortie)
			{
				case uc_VARIATEUR0:
					if(uc_Etat != 0)
                                            VARIATEUR0_ON;
					else
                                            VARIATEUR0_OFF;
				break;
				case uc_VARIATEUR1:
					if(uc_Etat != 0)
                                            VARIATEUR1_ON;
					else
                                            VARIATEUR1_OFF;
				break;
				case uc_VARIATEUR2:
					if(uc_Etat != 0)
                                            VARIATEUR2_ON;
					else
                                            VARIATEUR2_OFF;
				break;
				case uc_VARIATEUR3:
					if(uc_Etat != 0)
                                            VARIATEUR3_ON;
					else
                                            VARIATEUR3_OFF;
				break;
			}
		break;
		case uc_SORTIE_LED:
			if(uc_Etat != 0)
			{
				LED_ON;
			}	
			else
			{
				LED_OFF;
			}
		break;
	}
	PORTA = s_uc_PortA;
	PORTB = s_uc_PortB;
	PORTC = s_uc_PortC;
	PORTD = s_uc_PortD;
	PORTE = s_uc_PortE;
	PORTF = s_uc_PortF;
}

// Pilote les sorties des relais bistables : active
/* void vd_PosSortieBistable_ON(unsigned char uc_NumeroSortie)
{
	switch(uc_NumeroSortie)
	{
		case uc_RB0:
			RB0_ON;
		break;
		case uc_RB1:
			RB1_ON;
		break;
		case uc_RB2:
			RB2_ON;
		break;
		case uc_RB3:
			RB3_ON;
		break;
		case uc_RB4:
			RB4_ON;
		break;
		case uc_RB5:
			RB5_ON;
		break;
		case uc_RB6:
			RB6_ON;
		break;
		case uc_RB7:
			RB7_ON;
		break;
		case uc_RB8:
			RB8_ON;
		break;
		case uc_RB9:
			RB9_ON;
		break;
		case uc_RB10:
			RB10_ON;
		break;
		case uc_RB11:
			RB11_ON;
		break;
	}
	PORTA = s_uc_PortA;
	PORTB = s_uc_PortB;
	PORTC = s_uc_PortC;
	PORTD = s_uc_PortD;
	PORTE = s_uc_PortE;
	PORTF = s_uc_PortF;
}
*/
// Pilote les sorties des relais bistables : inactive
/* void vd_PosSortieBistable_OFF(unsigned char uc_NumeroSortie)
{
	switch(uc_NumeroSortie)
	{
		case uc_RB0:
			RB0_OFF;
		break;
		case uc_RB1:
			RB1_OFF;
		break;
		case uc_RB2:
			RB2_OFF;
		break;
		case uc_RB3:
			RB3_OFF;
		break;
		case uc_RB4:
			RB4_OFF;
		break;
		case uc_RB5:
			RB5_OFF;
		break;
		case uc_RB6:
			RB6_OFF;
		break;
		case uc_RB7:
			RB7_OFF;
		break;
		case uc_RB8:
			RB8_OFF;
		break;
		case uc_RB9:
			RB9_OFF;
		break;
		case uc_RB10:
			RB10_OFF;
		break;
		case uc_RB11:
			RB11_OFF;
		break;
	}
	PORTA = s_uc_PortA;
	PORTB = s_uc_PortB;
	PORTC = s_uc_PortC;
	PORTD = s_uc_PortD;
	PORTE = s_uc_PortE;
	PORTF = s_uc_PortF;
}
*/

// // Pilote toutes les sorties du micro : à partir des états pré positionnés
// void vd_Pilote_Sortie(void)
// {
// 	PORTA = s_uc_PortA;
// 	PORTB = s_uc_PortB;
// 	PORTC = s_uc_PortC;
// 	PORTD = s_uc_PortD;
// 	PORTE = s_uc_PortE;
// 	PORTF = s_uc_PortF;
// }

// Pilote les sorties des variateurs
void vd_PiloteSortieVariateur(unsigned char uc_NumeroSortie,unsigned char uc_Etat)
{
	
	switch(uc_NumeroSortie)
	{
		case uc_VARIATEUR0:
			if(uc_Etat != 0)
                            VARIATEUR0_ON;
			else
                            VARIATEUR0_OFF;
		break;
		case uc_VARIATEUR1:
			if(uc_Etat != 0)
                            VARIATEUR1_ON;
			else
                            VARIATEUR1_OFF;
		break;
		case uc_VARIATEUR2:
			if(uc_Etat != 0)
                            VARIATEUR2_ON;
			else
                            VARIATEUR2_OFF;
		break;
		case uc_VARIATEUR3:
			if(uc_Etat != 0)
                            VARIATEUR3_ON;
			else
                            VARIATEUR3_OFF;
		break;
	}
	PORTA = s_uc_PortA;
	PORTB = s_uc_PortB;
	PORTC = s_uc_PortC;
	PORTD = s_uc_PortD;
	PORTE = s_uc_PortE;
	PORTF = s_uc_PortF;

}
