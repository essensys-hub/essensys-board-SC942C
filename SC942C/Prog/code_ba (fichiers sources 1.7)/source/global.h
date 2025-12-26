

// Gestion cycle
extern unsigned char uc_CompteurCycle;	// Permet de cadencer le traitement cyclique (incrémenté par le traitement d'acquisition des entrées)
extern unsigned char uc_CompteurAcq;	// Permet de cadencer le traitement d'acquisition des entrées (incrémenté par IT Timer)
//extern unsigned char uc_CompteurVar;	// Permet de cadencer l'envoi des consignes aux variateurs (incrémenté par IT Timer)
extern unsigned char uc_CompteurBase1S;	// cadencement base de temps 1 seconde (incrémenté par le traitement cyclique)
//extern unsigned char uc_COmpteurVar2;	// cadencement rampe utilisateur (incrémenté par le traitement cyclique)

extern unsigned char b1_BT_1S;			// flag seconde : à 1 chaque seconde pendant 1 cycle

// Gestion pilotage led
extern unsigned char uc_CompteurLED;	// Cadence l'allumage de la led d'état

// Gestion impulsions de commande relais bistable
extern unsigned char uc_CompteurBiStable;	

// Gestion Watchdog
extern unsigned char uc_FlagITTimer;	// Permet de savoir si au moins une IT Timer a eu lieu durant le cycle

// Configuration du bornier
// extern unsigned char uc_TypeBornier;
extern unsigned char uc_NB_ES;
extern unsigned char uc_ENTREE_TOR_NB;
extern unsigned char uc_RM_NB;
extern unsigned char uc_ENTREE_VARIATEUR_NB;
extern unsigned char uc_VARIATEUR_NB;
extern unsigned char uc_ENTREE_VOLET_NB;
extern unsigned char uc_RB_NB;
extern unsigned char uc_VOLET_NB;


// Entrées
extern struct struct_Entree st_Entrees[NB_ES];		// Contient l'état filtré (anti-rebond) de toutes les entrées
extern unsigned short us_EntreesTOR;				// xxx passe sur 16 bits à cause du bornier "pièces de vie"
extern struct struct_EntreeVariateur st_EntreesVariateur[ENTREE_VARIATEUR_NB];
extern struct struct_EntreeVoletOuStore st_EntreesVoletRoulant[ENTREE_VOLET_NB];


// Sorties : contient les états à mettre sur les sorties
extern unsigned short us_SortiesRelais;				// xxx passe sur 16 bits à cause du bornier "pièces de vie"
extern struct struct_SortieRelaisBistable st_SortiesRelaisBistables[RB_NB];

extern struct struct_Variateur	st_Variateurs[VARIATEUR_NB];

// xxx extern unsigned short us_CdeRelaisBistables[uc_RB_NB];		// pour commande en "PWM"
// xxx passage de 16 bits à 8 bits non significatif sur les temps d'éxécution
extern unsigned char uc_CdeRelaisBistables[RB_NB];		// mot de commande de la sortie : pour commande en "PWM"
extern unsigned short us_CdeVariateurs[VARIATEUR_NB];	// mot de commande de la sortie : pour commande en liaison série

//images des ports du micro : utilisées pour préparer le signal avant positionnement sur les sorties physiques
extern unsigned char s_uc_PortA;
extern unsigned char s_uc_PortB;
extern unsigned char s_uc_PortC;
extern unsigned char s_uc_PortD;
extern unsigned char s_uc_PortE;
extern unsigned char s_uc_PortF;

// applicatif
unsigned char uc_TempsCdeVolet[VOLET_NB];		// temps max de montée/descente du volet ou store (s) = temps de maintien de la commande
unsigned char uc_TempsCdeLampe[RM_NB];			// temps max d'allumage (mn) = temps de maintien de la commande
												// 0 = pas d'extinction automatique
extern union struct_Cde_BP   st_Commandes;

// eeprom
// xxx pour voir si l'écriture se termine à la perte de tension
extern unsigned char	uc_compteur_ecriture_eeprom;
extern unsigned char	uc_compteur_lecture_eeprom;

// détection chute de tension
//extern unsigned char	ub1_Tension_OK;
//extern unsigned char	ub1_Tension_Chute;
//extern unsigned char	ub1_Tension_ChuteIT;

