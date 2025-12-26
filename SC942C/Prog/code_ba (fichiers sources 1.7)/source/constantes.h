// PWM
// avec quartz externe : 20 MHz
// période
#define	PWM_PER 249;		// période de 20KHz : prescaler = 1 => 250
// largeur d'impulsion 0,371 de 20KHz soit 0.01855ms => CCPR = 371
#define	PWM_LSB		0x30;	// 371 : 2 bits de poids faibles rangés dans les bits 4 à 5
#define	PWM_MSB		0x5C;	// 371 : 8 bits de poids forts (sur 10) rangés dans les bits 0 à 7

// avec quartz interne : 4 MHz
//// période
//#define	PWM_PER 49;		// période de 20KHz : prescaler = 1 => 50
////#define	PWM_PER		24;		// période de 40KHz : prescaler = 1 => 25
//
//// largeur d'impulsion 0,371 de 20KHz soit 0.01855ms => CCPR = 74
//#define	PWM_LSB		0x30;	// 75 : 2 bits de poids faibles rangés dans les bits 4 à 5
//#define	PWM_MSB		0x12;	// 75 : 8 bits de poids forts (sur 10) rangés dans les bits 0 à 7
//// largeur d'impulsion 1/2 de 20KHz soit 0.0167ms => CCPR = 100
////#define	PWM_LSB		0x00;	// 100 : 2 bits de poids faibles rangés dans les bits 4 à 5
////#define	PWM_MSB		0x19;	// 100 : 8 bits de poids forts (sur 10) rangés dans les bits 0 à 7
//// largeur d'impulsion 1/3 de 20KHz soit 0.0167ms => CCPR = 66
////#define	PWM_LSB		0x10;	// 65 : 2 bits de poids faibles rangés dans les bits 4 à 5
////#define	PWM_MSB		0x10;	// 65 : 8 bits de poids forts (sur 10) rangés dans les bits 0 à 7
//// largeur d'impulsion 1/3 de 40KHz soit 0.008ms => CCPR = 33
////#define	PWM_LSB		0x10;	// 33 : 2 bits de poids faibles rangés dans les bits 4 à 5
////#define	PWM_MSB		0x08;	// 33 : 8 bits de poids forts (sur 10) rangés dans les bits 0 à 7

//#define uc_TIMER_10_MS	62
//#define uc_TIMER_5_MS	31	// Timer 5 ms (avec prescaler 1/16 et postcaler 1/10)
//							// Pour une horloge par défaut à 4 MHz (-> 1 Mhz pour le timer)
//							// Compte de 0 à PR2

//#define uc_TIMER_05_MS	255-125+1	// Timer 0,5 ms (avec prescaler 1/1)
//#define uc_TIMER_04_MS	100	// Timer 0,5 ms (avec prescaler 1/16) 4MIPS4/2.5KHz=1600cy 1600cy/16(prescaler)=100
#define uc_TIMER_416_US	130	// Timer 0,416 ms (avec prescaler 1/16) 5MIPS4/2.4KHz=2083cy 1667cy/16(prescaler)=130
//#define uc_TIMER_05_MS	255-156

//#define uc_TIMER_05_MS	50	// Timer 0,5 ms (avec prescaler 1/1 et postcaler 1/10)
//							// Pour une horloge par défaut à 4 MHz (-> 1 Mhz pour le timer)
//							// Compte de 0 à PR2

// reprise des valeurs dépendantes de l'IT horloge (passe de 0,5 ms à 0,416 ms)
//#define uc_ACQ				10	// Cadencement Acquisition des entrées - nb IT Timer -> 5 ms
//#define uc_VAR				50	// Cadencement rampes variateurs - nb IT Timer -> 25 ms
#define uc_ACQ					12	// Cadencement Acquisition des entrées - nb IT Timer -> 5 ms
#define uc_VAR					60	// Cadencement rampes variateurs - nb IT Timer -> 25 ms
#define uc_CYCLE				8	// Cadencement traitement cyclique - nb IT Timer * uc_ACQ -> 40 ms
#define uc_LED_CADENCEMENT                      3	// Cadencement flash led état -> 120 ms
#define uc_1s_CADENCEMENT                       25      // Cadencement base de temps 1s -> 1000 ms

#define uc_ENTREE_ANTI_REBOND_CHGT_ETAT	4	// Gestion anti-rebond des entrées - Nb coups sans chgt état pour prise en compte état entrée
											// 4 -> 20 ms
											// Comptage effectué sous IT TIMER
											// !!! Compteur sur 4 bits -> MAX : 15


#define ub4_TEMPS_VOLET_APPUI_COURT_LONG                 	6	// Tempo validant un appui court / appui long
									// 6 * 40 ms -> 240 ms
									// Comptage effectué sous APPLICATIF CYCLE

#define uc_TEMPS_VARIATEUR_APPUI_COURT_LONG             	15	// Tempo validant un appui court / appui double
									// 15 * 40 ms -> 600 ms
									// Comptage effectué sous APPLICATIF CYCLE

#define uc_TEMPS_VARIATEUR_SIMPLE_CLIC_DOUBLE_CLIC              13	// Tempo validant un appui court / appui double
									// 13 * 40 ms -> 520 ms
									// Comptage effectué sous APPLICATIF CYCLE


#define uc_DUREE_CDE_MAINTENUE_RELAIS_BISTABLE			20 	// Durée de la commande de pilotage des relais statiques : ON ou OFF
															// 20 * 5 ms -> 100 ms
//#define uc_DUREE_CDE_MAINTENUE_RELAIS_BISTABLE			30 	// Durée de la commande de pilotage des relais statiques : ON ou OFF
//															// 30 * 5 ms -> 150 ms
#define uc_DUREE_COMMANDE_MAINTENUE_RELAIS_BISTABLE		5 	// Durée de la commande de pilotage des relais statiques : temps entre 2 commandes
															// 5 * 40 ms -> 200 ms

#define uc_TEMPS_CDE_VOLET_DEFAUT       			2*60	// init à 2 minutes par défaut (si nulle)
#define uc_TEMPS_CDE_STORE_DEFAUT       			255	// valeur au maxi pour le store

#define uc_CDE_VAR_OFFSET					10		//commande variateur : offset pour ne jamais transmettre de valeur nulle
#define uc_VAR_MINI_CDE						10		// consigne minimun pour allumer les lampes sans parasites

#define uc_VAR_PAS_DOUX						5		// rampe douce : valeur du pas
													// 5% toutes les 25ms (uc_VAR) => 100 % en 500ms
// xxx #define uc_VAR_PAS_UTILISATEUR				10		// rampe utilisateur : valeur du pas 
// xxx 													// 10% toutes les 1s => 100 % en 10s
#define uc_VAR_PAS_UTILISATEUR				1		// rampe utilisateur : valeur du pas
//#define uc_VAR_TEMPO_UTILISATEUR			2		// rampe utilisateur : tempo du pas (x40ms)
													// 1% toutes les 1080ms => 100 % en 10s
#define uc_VAR_TEMPS_ENTRE_CONFIRMATION		138		// variateur : délai pour confirmation de la consigne
													// 138 * 40 ms -> 5,5s

enum enum_ENTREES
{
	uc_ENTREE0,
	uc_ENTREE1,
	uc_ENTREE2,
	uc_ENTREE3,
	uc_ENTREE4,
	uc_ENTREE5,
	uc_ENTREE6,
	uc_ENTREE7,
	uc_ENTREE8,
	uc_ENTREE9,
	uc_ENTREE10,
	uc_ENTREE11,
	uc_ENTREE12,
	uc_ENTREE13,
	uc_ENTREE14,
	uc_ENTREE15,
	uc_ENTREE16,
	uc_ENTREE17,
	uc_ENTREE18,
	uc_ENTREE19,
	uc_ENTREE20,
	uc_ENTREE21,
	NB_ES,
};	

// IR 12/03/13 : 3ième mode pour les variateurs
enum enum_ENTREE_VARIATEUR_MODE
{
	uc_MODE_SOFT_START,     // completement eteint / allume pleine puissance avec soft start
	uc_MODE_VARIATEUR,      // variation avec soft start
	uc_MODE_TOR,            // completement eteint / allume pleine puissance immediatement
};

enum enum_ENTREES_CMDE_TOR
{
	uc_TOR_OFF,
	uc_TOR_ON,
};

enum enum_ENTREES_CMDE_VOLET_STORE
{
	uc_VOLET_RIEN,			// xxx aucune action, même pas OFF
	uc_VOLET_OFF,
	uc_VOLET_APPUI_COURT,	// RAZ par applicatif une fois pris en compte
	uc_VOLET_APPUI_LONG,
	uc_VOLET_ON,
};

enum enum_ENTREES_CMDE_VARIATEUR
{
	uc_VARIATEUR_RIEN,
	uc_VARIATEUR_OFF,
	uc_VARIATEUR_ON,
	uc_VARIATEUR_APPUI_COURT,	// RAZ par applicatif une fois pris en compte
	uc_VARIATEUR_APPUI_DOUBLE,	// RAZ par applicatif une fois pris en compte
	uc_VARIATEUR_APPUI_LONG,
};

enum enum_ENTREES_TOR	// MAX 16 (gestion dans uc_EntreesTOR)
{
	uc_ENTREE_TOR0,
	uc_ENTREE_TOR1,
	uc_ENTREE_TOR2,
	uc_ENTREE_TOR3,
	uc_ENTREE_TOR4,
	uc_ENTREE_TOR5,
	uc_ENTREE_TOR6,
	uc_ENTREE_TOR7,
	uc_ENTREE_TOR8,
	uc_ENTREE_TOR9,
	uc_ENTREE_TOR10,
	uc_ENTREE_TOR11,
	uc_ENTREE_TOR12,
	ENTREE_TOR_NB,
};	

enum enum_ENTREE_VARIATEUR
{
	uc_ENTREE_VARIATEUR0,
	uc_ENTREE_VARIATEUR1,
	uc_ENTREE_VARIATEUR2,
	uc_ENTREE_VARIATEUR3,
	ENTREE_VARIATEUR_NB,
};

enum enum_ENTREE_VOLET
{
	uc_ENTREE_VOLET0,
	uc_ENTREE_VOLET1,
	uc_ENTREE_VOLET2,
	uc_ENTREE_VOLET3,
	uc_ENTREE_VOLET4,
	uc_ENTREE_VOLET5,
	uc_ENTREE_VOLET6,
	uc_ENTREE_VOLET7,
	uc_ENTREE_VOLET8,
	uc_ENTREE_VOLET9,
	uc_ENTREE_VOLET10,
	uc_ENTREE_VOLET11,
	ENTREE_VOLET_NB,
};	

enum enum_RELAIS
{
	uc_RM0,
	uc_RM1,
	uc_RM2,
	uc_RM3,
	uc_RM4,
	uc_RM5,
	uc_RM6,
	uc_RM7,
	uc_RM8,
	uc_RM9,
	uc_RM10,
	uc_RM11,
	uc_RM12,
	RM_NB,
};

enum enum_TYPES_SORTIES
{
	uc_SORTIE_RELAIS,
	uc_SORTIE_RELAIS_BISTABLE_SENS,
	uc_SORTIE_RELAIS_BISTABLE,
	uc_SORTIE_VARIATEUR,
	uc_SORTIE_LED,
};

enum enum_RELAIS_BISTABLE_SENS
{
	uc_RBSENS1,
	uc_RBSENS2,
};

enum enum_RELAIS_BISTABLES
{
	uc_RB0,
	uc_RB1,
	uc_RB2,
	uc_RB3,
	uc_RB4,
	uc_RB5,
	uc_RB6,
	uc_RB7,
	uc_RB8,
	uc_RB9,
	uc_RB10,
	uc_RB11,
	RB_NB,
};
#define VOLET_NB		RB_NB/2	// 2 relais bistable par volet

enum enum_RELAIS_PILOTAGE
{
	uc_OFF,
	uc_ON,
};

enum enum_TEMPO_RELAIS
{
	INIT,
	RAZ,
	MAINTIEN,
};

enum enum_VARIATEUR
{
	uc_VARIATEUR0,
	uc_VARIATEUR1,
	uc_VARIATEUR2,
	uc_VARIATEUR3,
	VARIATEUR_NB,
};

enum enum_TYPES_BORNIER
{
	BORNIER_PIECES_VIE,
	BORNIER_CHAMBRES,
	BORNIER_PIECES_EAU,
};
	