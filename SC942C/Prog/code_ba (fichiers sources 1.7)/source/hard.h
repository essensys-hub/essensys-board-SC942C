
extern void vd_InitMicro(void);
extern void vd_ConfigBornier(void);
extern unsigned char uc_EtatEntree(unsigned char uc_NumeroEntree);
extern void vd_PiloterSortie(unsigned char uc_TypeSortie,unsigned char uc_NumeroSortie,unsigned char uc_Etat);

extern void vd_PosSortieBistable_ON(unsigned char uc_NumeroSortie);
extern void vd_PosSortieBistable_OFF(unsigned char uc_NumeroSortie);
extern void vd_PiloteSortieVariateur(unsigned char uc_NumeroSortie,unsigned char uc_Etat);

#undef TYPE_PIECES_DE_VIE           // SC940-0B
#undef TYPE_PIECES_D_EAU           // SC941-0A
#undef TYPE_CHAMBRES                // SC942-0A

// sélection du type de boitier : 1 seul doit être define à la fois, les autres doivent être undef
//#define TYPE_PIECES_DE_VIE           // SC940-0B
//#define TYPE_PIECES_D_EAU           // SC941-0A
#define TYPE_CHAMBRES                // SC942-0A

#undef TYPE_PIECES_DE_VIE_PROTO     // SC940-0A proto

#ifdef TYPE_CHAMBRES

// boitier auxiliaire "Chambres"

// adresse I2C
            //      0x11	// boitier pièces de vie
#define I2C_ADDR    0x12	// boitier chambres
            //      0x13	// boitier pièces d'eau

// Entrées de la carte "Chambres"
#define ENTREE0_INIT		TRISB |= 0x01;
#define ENTREE0_ETAT		(PORTB & 0x01)

#define ENTREE1_INIT		TRISF |= 0x08;
#define ENTREE1_ETAT		(PORTF & 0x08)

#define ENTREE2_INIT		TRISF |= 0x04;
#define ENTREE2_ETAT		(PORTF & 0x04)

#define ENTREE3_INIT		TRISF |= 0x02;
#define ENTREE3_ETAT		(PORTF & 0x02)

#define ENTREE4_INIT		TRISF |= 0x01;
#define ENTREE4_ETAT		(PORTF & 0x01)

#define ENTREE5_INIT		TRISG |= 0x20;
#define ENTREE5_ETAT		(PORTG & 0x20)

#define ENTREE6_INIT		TRISG |= 0x10;
#define ENTREE6_ETAT		(PORTG & 0x10)

#define ENTREE7_INIT		TRISG |= 0x08;
#define ENTREE7_ETAT		(PORTG & 0x08)

#define ENTREE8_INIT		TRISG |= 0x04;
#define ENTREE8_ETAT		(PORTG & 0x04)

#define ENTREE9_INIT		TRISG |= 0x02;
#define ENTREE9_ETAT		(PORTG & 0x02)

#define ENTREE10_INIT		TRISG |= 0x01;
#define ENTREE10_ETAT		(PORTG & 0x01)

#define ENTREE11_INIT		TRISD |= 0x80;
#define ENTREE11_ETAT		(PORTD & 0x80)

#define ENTREE12_INIT		TRISD |= 0x40;
#define ENTREE12_ETAT		(PORTD & 0x40)

#define ENTREE13_INIT		TRISD |= 0x20;
#define ENTREE13_ETAT		(PORTD & 0x20)

#define ENTREE14_INIT		TRISD |= 0x10;
#define ENTREE14_ETAT		(PORTD & 0x10)

#define ENTREE15_INIT		TRISC |= 0x20;
#define ENTREE15_ETAT		(PORTC & 0x20)

#define ENTREE16_INIT		TRISC |= 0x10;
#define ENTREE16_ETAT		(PORTC & 0x10)

#define ENTREE17_INIT		TRISD |= 0x01;
#define ENTREE17_ETAT		(PORTD & 0x01)

#define ENTREE18_INIT		TRISC |= 0x08;
#define ENTREE18_ETAT		(PORTC & 0x08)

#define ENTREE19_INIT		TRISC |= 0x04;
#define ENTREE19_ETAT		(PORTC & 0x04)

#define ENTREE20_INIT		TRISC |= 0x02;
#define ENTREE20_ETAT		(PORTC & 0x02)

#define ENTREE21_INIT		TRISA |= 0x02;	// non utilisé : sur pin non câblée
#define ENTREE21_ETAT		(PORTA & 0x02)	// non utilisé : sur pin non câblée

// Sorties de la carte "Chambres"
#define RM0_INIT	TRISB &= ~0x08;
#define RM0_OFF		s_uc_PortB &= ~0x08
#define RM0_ON		s_uc_PortB |= 0x08

#define RM1_INIT	TRISB &= ~0x10;
#define RM1_OFF		s_uc_PortB &= ~0x10
#define RM1_ON		s_uc_PortB |= 0x10

#define RM2_INIT	TRISB &= ~0x20;
#define RM2_OFF		s_uc_PortB &= ~0x20
#define RM2_ON		s_uc_PortB |= 0x20

#define RM3_INIT	TRISA &= ~0x01;
#define RM3_OFF		s_uc_PortA &= ~0x01
#define RM3_ON		s_uc_PortA |= 0x01

#define RM4_INIT	TRISE &= ~0x01;
#define RM4_OFF		s_uc_PortE &= ~0x01
#define RM4_ON		s_uc_PortE |= 0x01

#define RM5_INIT	TRISE &= ~0x20;
#define RM5_OFF		s_uc_PortE &= ~0x20
#define RM5_ON		s_uc_PortE |= 0x20

#define RM6_INIT	TRISE &= ~0x40;
#define RM6_OFF		s_uc_PortE &= ~0x40
#define RM6_ON		s_uc_PortE |= 0x40

#define RM7_INIT	;			// non utilisé : pas d'action
#define RM7_OFF                 		// non utilisé : pas d'action
#define RM7_ON                  		// non utilisé : pas d'action

#define RM8_INIT	;			// non utilisé : pas d'action
#define RM8_OFF                 		// non utilisé : pas d'action
#define RM8_ON                  		// non utilisé : pas d'action

#define RM9_INIT	;			// non utilisé : pas d'action
#define RM9_OFF                 		// non utilisé : pas d'action
#define RM9_ON                  		// non utilisé : pas d'action

#define RM10_INIT	;			// non utilisé : pas d'action
#define RM10_OFF                 		// non utilisé : pas d'action
#define RM10_ON                  		// non utilisé : pas d'action

#define RM11_INIT	;			// non utilisé : pas d'action
#define RM11_OFF                 		// non utilisé : pas d'action
#define RM11_ON                  		// non utilisé : pas d'action

#define RM12_INIT	;			// non utilisé : pas d'action
#define RM12_OFF                 		// non utilisé : pas d'action
#define RM12_ON                  		// non utilisé : pas d'action


// #define PWM_INIT_IN	TRISD |= 0x04;		// sera basculé en sortie à la fin du 1er cycle
#define PWM_INIT_OUT   	TRISD &= ~0x04;		// sera basculé en sortie à la fin du 1er cycle
#define RBSENS1_INIT	TRISD |= 0x04;
#define RBSENS1_OFF
#define RBSENS1_ON

#define RBSENS2_INIT	TRISD &= ~0x02;
#define RBSENS2_OFF	s_uc_PortD &= ~0x02
#define RBSENS2_ON	s_uc_PortD|= 0x02

#define RB0_INIT	TRISA &= ~0x04;
#define RB0_OFF		s_uc_PortA &= ~0x04
#define RB0_ON		s_uc_PortA |= 0x04

#define RB1_INIT	TRISA &= ~0x08;
#define RB1_OFF		s_uc_PortA &= ~0x08
#define RB1_ON		s_uc_PortA |= 0x08

#define RB2_INIT	TRISA &= ~0x10;
#define RB2_OFF		s_uc_PortA &= ~0x10
#define RB2_ON		s_uc_PortA |= 0x10

#define RB3_INIT	TRISA &= ~0x20;
#define RB3_OFF		s_uc_PortA &= ~0x20
#define RB3_ON		s_uc_PortA |= 0x20

#define RB4_INIT	TRISE &= ~0x02;
#define RB4_OFF		s_uc_PortE &= ~0x02
#define RB4_ON		s_uc_PortE |= 0x02

#define RB5_INIT	TRISE &= ~0x10;
#define RB5_OFF		s_uc_PortE &= ~0x10
#define RB5_ON		s_uc_PortE |= 0x10

#define RB6_INIT	TRISE &= ~0x80;
#define RB6_OFF		s_uc_PortE &= ~0x80
#define RB6_ON		s_uc_PortE |= 0x80

#define RB7_INIT	TRISF &= ~0x10;
#define RB7_OFF		s_uc_PortF &= ~0x10
#define RB7_ON		s_uc_PortF |= 0x10

#define RB8_INIT	TRISF &= ~0x20;
#define RB8_OFF		s_uc_PortF &= ~0x20
#define RB8_ON		s_uc_PortF |= 0x20

#define RB9_INIT	TRISF &= ~0x40;
#define RB9_OFF		s_uc_PortF &= ~0x40
#define RB9_ON		s_uc_PortF |= 0x40

#define RB10_INIT	;			// non utilisé : pas d'action
#define RB10_OFF				// non utilisé : pas d'action
#define RB10_ON					// non utilisé : pas d'action

#define RB11_INIT	;			// non utilisé : pas d'action
#define RB11_OFF				// non utilisé : pas d'action
#define RB11_ON					// non utilisé : pas d'action


#define VARIATEUR0_INIT	TRISB &= ~0x02;
#define VARIATEUR0_OFF	s_uc_PortB &= ~0x02
#define VARIATEUR0_ON	s_uc_PortB |= 0x02

#define VARIATEUR1_INIT	TRISB &= ~0x04;
#define VARIATEUR1_OFF	s_uc_PortB &= ~0x04
#define VARIATEUR1_ON	s_uc_PortB |= 0x04

#define VARIATEUR2_INIT	TRISF &= ~0x80;
#define VARIATEUR2_OFF	s_uc_PortF &= ~0x80
#define VARIATEUR2_ON	s_uc_PortF |= 0x80

#define VARIATEUR3_INIT	TRISC &= ~0x01;
#define VARIATEUR3_OFF	s_uc_PortC &= ~0x01
#define VARIATEUR3_ON	s_uc_PortC |= 0x01

#define LED_INIT	TRISD &= ~0x08;
#define LED_OFF		s_uc_PortD &= ~0x08
#define LED_ON		s_uc_PortD |= 0x08

#endif

#ifdef TYPE_PIECES_D_EAU

// boitier auxiliaire "Pièces d'eau"

// adresse I2C
            //      0x11	// boitier pièces de vie
            //      0x12	// boitier chambres
#define I2C_ADDR    0x13	// boitier pièces d'eau
// xxx #define I2C_ADDR    0x62	// boitier pièces d'eau

// Entrées de la carte "Pièces d'eau"
#define ENTREE0_INIT		TRISB |= 0x01;
#define ENTREE0_ETAT		(PORTB & 0x01)

#define ENTREE1_INIT		TRISF |= 0x08;
#define ENTREE1_ETAT		(PORTF & 0x08)

#define ENTREE2_INIT		TRISF |= 0x04;
#define ENTREE2_ETAT		(PORTF & 0x04)

#define ENTREE3_INIT		TRISF |= 0x02;
#define ENTREE3_ETAT		(PORTF & 0x02)

#define ENTREE4_INIT		TRISF |= 0x01;
#define ENTREE4_ETAT		(PORTF & 0x01)

#define ENTREE5_INIT		TRISG |= 0x20;
#define ENTREE5_ETAT		(PORTG & 0x20)

#define ENTREE6_INIT		TRISG |= 0x10;
#define ENTREE6_ETAT		(PORTG & 0x10)

#define ENTREE7_INIT		TRISG |= 0x08;
#define ENTREE7_ETAT		(PORTG & 0x08)

#define ENTREE8_INIT		TRISG |= 0x04;
#define ENTREE8_ETAT		(PORTG & 0x04)

#define ENTREE9_INIT		TRISG |= 0x02;
#define ENTREE9_ETAT		(PORTG & 0x02)

#define ENTREE10_INIT		TRISG |= 0x01;
#define ENTREE10_ETAT		(PORTG & 0x01)

#define ENTREE11_INIT		TRISD |= 0x80;
#define ENTREE11_ETAT		(PORTD & 0x80)

#define ENTREE12_INIT		TRISD |= 0x40;
#define ENTREE12_ETAT		(PORTD & 0x40)

#define ENTREE13_INIT		TRISD |= 0x20;
#define ENTREE13_ETAT		(PORTD & 0x20)

#define ENTREE14_INIT		TRISD |= 0x10;
#define ENTREE14_ETAT		(PORTD & 0x10)

#define ENTREE15_INIT		TRISC |= 0x20;
#define ENTREE15_ETAT		(PORTC & 0x20)

#define ENTREE16_INIT		TRISC |= 0x10;
#define ENTREE16_ETAT		(PORTC & 0x10)

#define ENTREE17_INIT		TRISD |= 0x01;
#define ENTREE17_ETAT		(PORTD & 0x01)

#define ENTREE18_INIT		TRISC |= 0x08;
#define ENTREE18_ETAT		(PORTC & 0x08)

#define ENTREE19_INIT		TRISC |= 0x04;
#define ENTREE19_ETAT		(PORTC & 0x04)

#define ENTREE20_INIT		TRISC |= 0x02;
#define ENTREE20_ETAT		(PORTC & 0x02)

#define ENTREE21_INIT		TRISC |= 0x01;
#define ENTREE21_ETAT		(PORTC & 0x01)


// Sorties de la carte "Pièces d'eau"
#define RM0_INIT	TRISB &= ~0x02;
#define RM0_OFF		s_uc_PortB &= ~0x02
#define RM0_ON		s_uc_PortB |= 0x02

#define RM1_INIT	TRISB &= ~0x04;
#define RM1_OFF		s_uc_PortB &= ~0x04
#define RM1_ON		s_uc_PortB |= 0x04

#define RM2_INIT	TRISE &= ~0x04;
#define RM2_OFF		s_uc_PortE &= ~0x04
#define RM2_ON		s_uc_PortE |= 0x04

#define RM3_INIT	TRISF &= ~0x80;
#define RM3_OFF		s_uc_PortF &= ~0x80
#define RM3_ON		s_uc_PortF |= 0x80

#define RM4_INIT	TRISF &= ~0x40;
#define RM4_OFF		s_uc_PortF &= ~0x40
#define RM4_ON		s_uc_PortF |= 0x40

#define RM5_INIT	TRISF &= ~0x20;
#define RM5_OFF		s_uc_PortF &= ~0x20
#define RM5_ON		s_uc_PortF |= 0x20

#define RM6_INIT	TRISF &= ~0x10;
#define RM6_OFF		s_uc_PortF &= ~0x10
#define RM6_ON		s_uc_PortF |= 0x10

#define RM7_INIT	TRISA &= ~0x20;
#define RM7_OFF		s_uc_PortA &= ~0x20
#define RM7_ON		s_uc_PortA |= 0x20

#define RM8_INIT	TRISA &= ~0x10;
#define RM8_OFF		s_uc_PortA &= ~0x10
#define RM8_ON		s_uc_PortA |= 0x10

#define RM9_INIT	TRISA &= ~0x08;
#define RM9_OFF		s_uc_PortA &= ~0x08
#define RM9_ON		s_uc_PortA |= 0x08

#define RM10_INIT	TRISA &= ~0x04;
#define RM10_OFF	s_uc_PortA &= ~0x04
#define RM10_ON		s_uc_PortA |= 0x04

#define RM11_INIT	TRISA &= ~0x02;
#define RM11_OFF	s_uc_PortA &= ~0x02
#define RM11_ON		s_uc_PortA |= 0x02

#define RM12_INIT	TRISA &= ~0x01;
#define RM12_OFF	s_uc_PortA &= ~0x01
#define RM12_ON		s_uc_PortA |= 0x01


// #define PWM_INIT_IN	TRISD |= 0x04;		// sera basculé en sortie à la fin du 1er cycle
#define PWM_INIT_OUT   	TRISD &= ~0x04;		// sera basculé en sortie à la fin du 1er cycle
#define RBSENS1_INIT	TRISD |= 0x04;
#define RBSENS1_OFF
#define RBSENS1_ON

#define RBSENS2_INIT	TRISD &= ~0x02;
#define RBSENS2_OFF	s_uc_PortD &= ~0x02
#define RBSENS2_ON	s_uc_PortD|= 0x02

#define RB0_INIT	TRISB &= ~0x08;
#define RB0_OFF		s_uc_PortB &= ~0x08
#define RB0_ON		s_uc_PortB |= 0x08

#define RB1_INIT	TRISB &= ~0x10;
#define RB1_OFF		s_uc_PortB &= ~0x10
#define RB1_ON		s_uc_PortB |= 0x10

#define RB2_INIT	TRISB &= ~0x20;
#define RB2_OFF		s_uc_PortB &= ~0x20
#define RB2_ON		s_uc_PortB |= 0x20

#define RB3_INIT	TRISE &= ~0x02;
#define RB3_OFF		s_uc_PortE &= ~0x02
#define RB3_ON		s_uc_PortE |= 0x02

#define RB4_INIT	TRISE &= ~0x10;
#define RB4_OFF		s_uc_PortE &= ~0x10
#define RB4_ON		s_uc_PortE |= 0x10

#define RB5_INIT	TRISE &= ~0x20;
#define RB5_OFF		s_uc_PortE &= ~0x20
#define RB5_ON		s_uc_PortE |= 0x20

#define RB6_INIT	TRISE &= ~0x80;
#define RB6_OFF		s_uc_PortE &= ~0x80
#define RB6_ON		s_uc_PortE |= 0x80

#define RB7_INIT	TRISE &= ~0x40;
#define RB7_OFF		s_uc_PortE &= ~0x40
#define RB7_ON		s_uc_PortE |= 0x40

#define RB8_INIT	;			// non utilisé : pas d'action
#define RB8_OFF					// non utilisé : pas d'action
#define RB8_ON					// non utilisé : pas d'action

#define RB9_INIT	;			// non utilisé : pas d'action
#define RB9_OFF					// non utilisé : pas d'action
#define RB9_ON					// non utilisé : pas d'action

#define RB10_INIT	;			// non utilisé : pas d'action
#define RB10_OFF				// non utilisé : pas d'action
#define RB10_ON					// non utilisé : pas d'action

#define RB11_INIT	;			// non utilisé : pas d'action
#define RB11_OFF				// non utilisé : pas d'action
#define RB11_ON					// non utilisé : pas d'action


#define VARIATEUR0_INIT	TRISE &= ~0x01;
#define VARIATEUR0_OFF	s_uc_PortE &= ~0x01
#define VARIATEUR0_ON	s_uc_PortE |= 0x01

#define VARIATEUR1_INIT	;			// non utilisé : pas d'action
#define VARIATEUR1_OFF				// non utilisé : pas d'action
#define VARIATEUR1_ON				// non utilisé : pas d'action

#define VARIATEUR2_INIT	;			// non utilisé : pas d'action
#define VARIATEUR2_OFF				// non utilisé : pas d'action
#define VARIATEUR2_ON				// non utilisé : pas d'action

#define VARIATEUR3_INIT ;			// non utilisé : pas d'action
#define VARIATEUR3_OFF				// non utilisé : pas d'action
#define VARIATEUR3_ON				// non utilisé : pas d'action

#define LED_INIT	TRISD &= ~0x08;
#define LED_OFF		s_uc_PortD &= ~0x08
#define LED_ON		s_uc_PortD |= 0x08

#endif

#ifdef TYPE_PIECES_DE_VIE

// boitier auxiliaire "Pièces de vie"

// adresse I2C
#define I2C_ADDR    0x11	// boitier pièces de vie
            //      0x12	// boitier chambres
            //      0x13	// boitier pièces d'eau

// n'existe plus #define ENTREE_CONF_INIT	TRISB |= 0x0C;
// n'existe plus #define ENTREE_CONF_ETAT	(PORTB & 0x0C) >> 2

// Entrées de la carte "Pièces de vie"
#define ENTREE0_INIT		TRISB |= 0x01;
#define ENTREE0_ETAT		(PORTB & 0x01)

#define ENTREE1_INIT		TRISF |= 0x08;
#define ENTREE1_ETAT		(PORTF & 0x08)

#define ENTREE2_INIT		TRISF |= 0x04;
#define ENTREE2_ETAT		(PORTF & 0x04)

#define ENTREE3_INIT		TRISF |= 0x02;
#define ENTREE3_ETAT		(PORTF & 0x02)

#define ENTREE4_INIT		TRISF |= 0x01;
#define ENTREE4_ETAT		(PORTF & 0x01)

#define ENTREE5_INIT		TRISG |= 0x20;
#define ENTREE5_ETAT		(PORTG & 0x20)

#define ENTREE6_INIT		TRISG |= 0x10;
#define ENTREE6_ETAT		(PORTG & 0x10)

#define ENTREE7_INIT		TRISG |= 0x08;
#define ENTREE7_ETAT		(PORTG & 0x08)

#define ENTREE8_INIT		TRISG |= 0x04;
#define ENTREE8_ETAT		(PORTG & 0x04)

#define ENTREE9_INIT		TRISG |= 0x02;
#define ENTREE9_ETAT		(PORTG & 0x02)

#define ENTREE10_INIT		TRISG |= 0x01;
#define ENTREE10_ETAT		(PORTG & 0x01)

#define ENTREE11_INIT		TRISD |= 0x80;
#define ENTREE11_ETAT		(PORTD & 0x80)

#define ENTREE12_INIT		TRISD |= 0x40;
#define ENTREE12_ETAT		(PORTD & 0x40)

#define ENTREE13_INIT		TRISD |= 0x20;
#define ENTREE13_ETAT		(PORTD & 0x20)

#define ENTREE14_INIT		TRISD |= 0x10;
#define ENTREE14_ETAT		(PORTD & 0x10)

#define ENTREE15_INIT		TRISC |= 0x20;
#define ENTREE15_ETAT		(PORTC & 0x20)

#define ENTREE16_INIT		TRISC |= 0x10;
#define ENTREE16_ETAT		(PORTC & 0x10)

#define ENTREE17_INIT		TRISD |= 0x01;
#define ENTREE17_ETAT		(PORTD & 0x01)

#define ENTREE18_INIT		TRISC |= 0x08;
#define ENTREE18_ETAT		(PORTC & 0x08)

#define ENTREE19_INIT		TRISC |= 0x04;
#define ENTREE19_ETAT		(PORTC & 0x04)

#define ENTREE20_INIT		TRISB |= 0x02;	// non utilisé : sur pin non câblée
#define ENTREE20_ETAT		(PORTB & 0x02)	// non utilisé : sur pin non câblée

#define ENTREE21_INIT		TRISB |= 0x02;	// non utilisé : sur pin non câblée
#define ENTREE21_ETAT		(PORTB & 0x02)	// non utilisé : sur pin non câblée


// Sorties de la carte "Pièces de vie"
#define RM0_INIT	TRISA &= ~0x01;
#define RM0_OFF		s_uc_PortA &= ~0x01
#define RM0_ON		s_uc_PortA |= 0x01

#define RM1_INIT	TRISA &= ~0x02;
#define RM1_OFF		s_uc_PortA &= ~0x02
#define RM1_ON		s_uc_PortA |= 0x02

#define RM2_INIT	TRISA &= ~0x04;
#define RM2_OFF		s_uc_PortA &= ~0x04
#define RM2_ON		s_uc_PortA |= 0x04

#define RM3_INIT	TRISE &= ~0x20;
#define RM3_OFF		s_uc_PortE &= ~0x20
#define RM3_ON		s_uc_PortE |= 0x20

#define RM4_INIT	TRISE &= ~0x40;
#define RM4_OFF		s_uc_PortE &= ~0x40
#define RM4_ON		s_uc_PortE |= 0x40

#define RM5_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM5_OFF		s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM5_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM6_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM6_OFF		s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM6_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM7_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM7_OFF		s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM7_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM8_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM8_OFF		s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM8_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM9_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM9_OFF		s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM9_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM10_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM10_OFF	s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM10_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM11_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM11_OFF	s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM11_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define RM12_INIT	TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define RM12_OFF	s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define RM12_ON		s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée


// #define PWM_INIT_IN	TRISD |= 0x04;		// sera basculé en sortie à la fin du 1er cycle
#define PWM_INIT_OUT   	TRISD &= ~0x04;		// sera basculé en sortie à la fin du 1er cycle
#define RBSENS1_INIT	TRISD |= 0x04;
#define RBSENS1_OFF		
#define RBSENS1_ON		

#define RBSENS2_INIT	TRISD &= ~0x02;
#define RBSENS2_OFF	s_uc_PortD &= ~0x02
#define RBSENS2_ON	s_uc_PortD|= 0x02

#define RB0_INIT	TRISA &= ~0x08;
#define RB0_OFF		s_uc_PortA &= ~0x08
#define RB0_ON		s_uc_PortA |= 0x08

#define RB1_INIT	TRISA &= ~0x10;
#define RB1_OFF		s_uc_PortA &= ~0x10
#define RB1_ON		s_uc_PortA |= 0x10

#define RB2_INIT	TRISA &= ~0x20;
#define RB2_OFF		s_uc_PortA&= ~0x20
#define RB2_ON		s_uc_PortA |= 0x20

#define RB3_INIT	TRISE &= ~0x01;
#define RB3_OFF		s_uc_PortE &= ~0x01
#define RB3_ON		s_uc_PortE |= 0x01

#define RB4_INIT	TRISE &= ~0x02;
#define RB4_OFF		s_uc_PortE &= ~0x02
#define RB4_ON		s_uc_PortE |= 0x02

#define RB5_INIT	TRISE &= ~0x04;
#define RB5_OFF		s_uc_PortE &= ~0x04
#define RB5_ON		s_uc_PortE |= 0x04

#define RB6_INIT	TRISE &= ~0x10;
#define RB6_OFF		s_uc_PortE &= ~0x10
#define RB6_ON		s_uc_PortE |= 0x10

#define RB7_INIT	TRISE &= ~0x80;
#define RB7_OFF		s_uc_PortE &= ~0x80
#define RB7_ON		s_uc_PortE |= 0x80

#define RB8_INIT	TRISF &= ~0x10;
#define RB8_OFF		s_uc_PortF &= ~0x10
#define RB8_ON		s_uc_PortF |= 0x10

#define RB9_INIT	TRISF &= ~0x20;
#define RB9_OFF		s_uc_PortF &= ~0x20
#define RB9_ON		s_uc_PortF |= 0x20

#define RB10_INIT	TRISF &= ~0x40;
#define RB10_OFF	s_uc_PortF &= ~0x40
#define RB10_ON		s_uc_PortF |= 0x40

#define RB11_INIT	TRISC &= ~0x02;
#define RB11_OFF	s_uc_PortC &= ~0x02
#define RB11_ON		s_uc_PortC |= 0x02


#define VARIATEUR0_INIT	TRISB &= ~0x10;
#define VARIATEUR0_OFF	s_uc_PortB &= ~0x10
#define VARIATEUR0_ON	s_uc_PortB |= 0x10

#define VARIATEUR1_INIT	TRISF &= ~0x80;
#define VARIATEUR1_OFF	s_uc_PortF &= ~0x80
#define VARIATEUR1_ON	s_uc_PortF |= 0x80

#define VARIATEUR2_INIT	TRISC &= ~0x01;
#define VARIATEUR2_OFF	s_uc_PortC &= ~0x01
#define VARIATEUR2_ON	s_uc_PortC |= 0x01

#define VARIATEUR3_INIT TRISB &= ~0x04;			// non utilisé : sur pin non câblée
#define VARIATEUR3_OFF	s_uc_PortB &= ~0x04		// non utilisé : sur pin non câblée
#define VARIATEUR3_ON	s_uc_PortB |= 0x04		// non utilisé : sur pin non câblée

#define LED_INIT	TRISD &= ~0x08;
#define LED_OFF		s_uc_PortD &= ~0x08
#define LED_ON		s_uc_PortD |= 0x08

#endif

#ifdef TYPE_PIECES_DE_VIE_PROTO

// boitier "pièces de vie" proto -----------------------------------------

// adresse I2C
#define I2C_ADDR    0x11	// boitier pièces de vie
            //      0x12	// boitier chambres
            //      0x13	// boitier pièces d'eau

#define ENTREE_CONF_INIT	TRISB |= 0x0C;
#define ENTREE_CONF_ETAT	(PORTB & 0x0C) >> 2

// Entrées de la carte "Pièces de vie"
#define ENTREE0_INIT		TRISB |= 0x01;
#define ENTREE0_ETAT		(PORTB & 0x01)

#define ENTREE1_INIT		TRISF |= 0x08;
#define ENTREE1_ETAT		(PORTF & 0x08)

#define ENTREE2_INIT		TRISF |= 0x04;
#define ENTREE2_ETAT		(PORTF & 0x04)

#define ENTREE3_INIT		TRISF |= 0x02;
#define ENTREE3_ETAT		(PORTF & 0x02)

#define ENTREE4_INIT		TRISF |= 0x01;
#define ENTREE4_ETAT		(PORTF & 0x01)

#define ENTREE5_INIT		TRISG |= 0x20;
#define ENTREE5_ETAT		(PORTG & 0x20)

#define ENTREE6_INIT		TRISG |= 0x10;
#define ENTREE6_ETAT		(PORTG & 0x10)

#define ENTREE7_INIT		TRISG |= 0x08;
#define ENTREE7_ETAT		(PORTG & 0x08)

#define ENTREE8_INIT		TRISG |= 0x04;
#define ENTREE8_ETAT		(PORTG & 0x04)

#define ENTREE9_INIT		TRISG |= 0x02;
#define ENTREE9_ETAT		(PORTG & 0x02)

#define ENTREE10_INIT		TRISG |= 0x01;
#define ENTREE10_ETAT		(PORTG & 0x01)

#define ENTREE11_INIT		TRISD |= 0x80;
#define ENTREE11_ETAT		(PORTD & 0x80)

#define ENTREE12_INIT		TRISD |= 0x40;
#define ENTREE12_ETAT		(PORTD & 0x40)

#define ENTREE13_INIT		TRISD |= 0x20;
#define ENTREE13_ETAT		(PORTD & 0x20)

#define ENTREE14_INIT		TRISD |= 0x10;
#define ENTREE14_ETAT		(PORTD & 0x10)

#define ENTREE15_INIT		TRISF |= 0x40;
#define ENTREE15_ETAT		(PORTF & 0x40)

#define ENTREE16_INIT		TRISE |= 0x80;
#define ENTREE16_ETAT		(PORTE & 0x80)

#define ENTREE17_INIT		TRISE |= 0x40;
#define ENTREE17_ETAT		(PORTE & 0x40)

#define ENTREE18_INIT		TRISC |= 0x04;
#define ENTREE18_ETAT		(PORTC & 0x04)

#define ENTREE19_INIT		TRISC |= 0x08;
#define ENTREE19_ETAT		(PORTC & 0x08)

#define ENTREE20_INIT		TRISC |= 0x08;	
#define ENTREE20_ETAT		(PORTC & 0x08)	

#define ENTREE21_INIT		TRISC |= 0x08;	
#define ENTREE21_ETAT		(PORTC & 0x08)	


// Sorties de la carte "Pièces de vie"
#define RM0_INIT	TRISA &= ~0x02;
#define RM0_OFF		s_uc_PortA &= ~0x02
#define RM0_ON		s_uc_PortA |= 0x02

#define RM1_INIT	TRISA &= ~0x04;
#define RM1_OFF		s_uc_PortA &= ~0x04
#define RM1_ON		s_uc_PortA |= 0x04

#define RM2_INIT	TRISA &= ~0x08;
#define RM2_OFF		s_uc_PortA &= ~0x08
#define RM2_ON		s_uc_PortA |= 0x08

#define RM3_INIT	TRISC &= ~0x10;
#define RM3_OFF		s_uc_PortC &= ~0x10
#define RM3_ON		s_uc_PortC |= 0x10

#define RM4_INIT	TRISC &= ~0x20;
#define RM4_OFF		s_uc_PortC &= ~0x20
#define RM4_ON		s_uc_PortC |= 0x20

#define RM5_INIT	TRISC &= ~0x20;			
#define RM5_OFF		s_uc_PortC &= ~0x20		
#define RM5_ON		s_uc_PortC |= 0x20		

#define RM6_INIT	TRISC &= ~0x20;			
#define RM6_OFF		s_uc_PortC &= ~0x20		
#define RM6_ON		s_uc_PortC |= 0x20		

#define RM7_INIT	TRISC &= ~0x20;			
#define RM7_OFF		s_uc_PortC &= ~0x20		
#define RM7_ON		s_uc_PortC |= 0x20		

#define RM8_INIT	TRISC &= ~0x20;			
#define RM8_OFF		s_uc_PortC &= ~0x20		
#define RM8_ON		s_uc_PortC |= 0x20		

#define RM9_INIT	TRISC &= ~0x20;			
#define RM9_OFF		s_uc_PortC &= ~0x20		
#define RM9_ON		s_uc_PortC |= 0x20		

#define RM10_INIT	TRISC &= ~0x20;			
#define RM10_OFF		s_uc_PortC &= ~0x20		
#define RM10_ON		s_uc_PortC |= 0x20		

#define RM11_INIT	TRISC &= ~0x20;			
#define RM11_OFF		s_uc_PortC &= ~0x20		
#define RM11_ON		s_uc_PortC |= 0x20		

#define RM12_INIT	TRISC &= ~0x20;			
#define RM12_OFF		s_uc_PortC &= ~0x20		
#define RM12_ON		s_uc_PortC |= 0x20		


// par vrai PWM  #define RBSENS1_INIT	TRISC &= ~0x01;
// par vrai PWM  #define RBSENS1_OFF		s_uc_PortC &= ~0x01
// par vrai PWM  #define RBSENS1_ON		s_uc_PortC |= 0x01
// par vrai PWM  sortie PWM soudée sur cette entrée
#define RBSENS1_INIT	TRISC |= 0x01;
#define RBSENS1_OFF
#define RBSENS1_ON

#define RBSENS2_INIT	TRISF &= ~0x80;
#define RBSENS2_OFF		s_uc_PortF &= ~0x80
#define RBSENS2_ON		s_uc_PortF |= 0x80

#define RB0_INIT		TRISA &= ~0x10;
#define RB0_OFF			s_uc_PortA &= ~0x10
#define RB0_ON			s_uc_PortA |= 0x10

#define RB1_INIT		TRISA &= ~0x20;
#define RB1_OFF			s_uc_PortA &= ~0x20
#define RB1_ON			s_uc_PortA |= 0x20

#define RB2_INIT		TRISE &= ~0x01;
#define RB2_OFF			s_uc_PortE &= ~0x01
#define RB2_ON			s_uc_PortE |= 0x01

#define RB3_INIT		TRISE &= ~0x02;
#define RB3_OFF			s_uc_PortE &= ~0x02
#define RB3_ON			s_uc_PortE |= 0x02

#define RB4_INIT		TRISE &= ~0x04;
#define RB4_OFF			s_uc_PortE &= ~0x04
#define RB4_ON			s_uc_PortE |= 0x04

#define RB5_INIT		TRISE &= ~0x10;
#define RB5_OFF			s_uc_PortE &= ~0x10
#define RB5_ON			s_uc_PortE |= 0x10

#define RB6_INIT		TRISA &= ~0x80;
#define RB6_OFF			s_uc_PortA &= ~0x80
#define RB6_ON			s_uc_PortA |= 0x80

#define RB7_INIT		TRISA &= ~0x40;
#define RB7_OFF			s_uc_PortA &= ~0x40
#define RB7_ON			s_uc_PortA |= 0x40

#define RB8_INIT		TRISE &= ~0x20;
#define RB8_OFF			s_uc_PortE &= ~0x20
#define RB8_ON			s_uc_PortE |= 0x20

#define RB9_INIT		TRISF &= ~0x10;
#define RB9_OFF			s_uc_PortF &= ~0x10
#define RB9_ON			s_uc_PortF |= 0x10

// par vrai PWM  #define RB10_INIT		TRISD &= ~0x04;
// par vrai PWM  #define RB10_OFF		s_uc_PortD &= ~0x04
// par vrai PWM  #define RB10_ON			s_uc_PortD |= 0x04
// par vrai PWM  RB10 non piloté : besoin de la sortie PWM2
#define PWM_INIT_IN		TRISD |= 0x04;		// sera basculé en sortie à la fin du 1er cycle
#define PWM_INIT_OUT	TRISD &= ~0x04;		// sera basculé en sortie à la fin du 1er cycle
#define RB10_INIT		TRISD |= 0x04;
#define RB10_OFF
#define RB10_ON

#define RB11_INIT		TRISD &= ~0x08;
#define RB11_OFF		s_uc_PortD &= ~0x08
#define RB11_ON			s_uc_PortD |= 0x08


#define VARIATEUR0_INIT		TRISA &= ~0x01;
#define VARIATEUR0_OFF		s_uc_PortA &= ~0x01
#define VARIATEUR0_ON		s_uc_PortA |= 0x01

#define VARIATEUR1_INIT		TRISC &= ~0x02;
#define VARIATEUR1_OFF		s_uc_PortC &= ~0x02
#define VARIATEUR1_ON		s_uc_PortC |= 0x02

#define VARIATEUR2_INIT		TRISD &= ~0x01;
#define VARIATEUR2_OFF		s_uc_PortD &= ~0x01
#define VARIATEUR2_ON		s_uc_PortD |= 0x01

#define VARIATEUR3_INIT		TRISD &= ~0x01;			
#define VARIATEUR3_OFF		s_uc_PortD &= ~0x01		
#define VARIATEUR3_ON		s_uc_PortD |= 0x01		

#define LED_INIT			TRISD &= ~0x02;
#define LED_OFF				s_uc_PortD &= ~0x02
#define LED_ON				s_uc_PortD |= 0x02

#endif