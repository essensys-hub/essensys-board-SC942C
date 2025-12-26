

// Structure correspondant à une entrée
// Acquisition et traitement anti-rebond (it timer)
struct struct_Entree
{
	unsigned char ub1_EtatActuel:1;					// Etat entrée brut avant traitement anti-rebond
	unsigned char ub4_AntiRebond_TempoChgtEtat:4;	// Compteur anti-rebond
	unsigned char ub1_EtatFiltrePourApp:1;			// Etat entrée après traitement anti-rebond
	unsigned char ub1_EtatFiltrePourAppPrecedent:1;	// Etat précédent après anti-rebond (pour différencier un chgt d'état)
};

// Traitement des entrées au niveau applicatif (traitement cycle) :
// entrée TOR -> mis dans un octet / 1 bit par entrée
// entrée variateur (peut être en TOR ou en mode variateur
struct struct_EntreeVariateur
{
        unsigned char uc_CompteurMaintenu;			// Compteur utilisé pour détecter un appui long
	unsigned char uc_CompteurDoubleClic;            	// Compteur utilisé pour détecter un double clic
	unsigned char ub3_EntreeMode:3;				// Mode TOR ou Mode Variateur
        unsigned char ub1_Etat1VueApresEtatStop:1;		// Permet de détecter un appui simple
        unsigned char ub1_SecondAppuiDetecte:1;                 // A 1 si deux appuis successifs dans le laps de temps ub4_CompteurDoubleClic

        unsigned char ub3_EtatCommande:3;			// Etat commande en fonction entrée -> pour applicatif
	unsigned char ub1_SensCommande:1;			// sens de commande : 1= augmentation / 0= diminution -> pour applicatif
};
// entrée volet roulant ou store
struct struct_EntreeVoletOuStore
{
	unsigned char ub4_CompteurMaintenu:4;			// Compteur utilisé pour mesurer la durée d'une impulsion
	unsigned char ub1_Etat1VueApresEtatStop:1;		// Permet de détecter un appui simple
	unsigned char ub3_EtatCommande:3;			// Etat commande en fonction entrée -> pour applicatif
};	

// Structure correspondant à une sortie RELAIS BISTABLE
struct struct_SortieRelaisBistable
{
	unsigned char ub1_EtatRelaisBistable:1;			// Contient la position du relais en cours
	unsigned char ub1_EtatRelaisBistableSouhaite:1;	// Contient l'état souhaité
};

// Structure correspondant à un variateur
struct struct_Variateur

{
	signed char	c_Eclairage;						// Consigne d'éclairage (voulue) du variateur
	signed char	c_EclairageOld;						// précédente consigne d'éclairage (avant extinction)
	signed char	c_Consigne;						// Consigne d'éclairage à envoyer au variateur (dont rampe)
	//signed char c_Pas; plus utilisé - cette rampe "rapide" est gérée directement dans le variateur (soft start)		// Pas à ajouter/soustraire à la consigne (rampe)
};

// Structure pour commandes générales à distance (par le boitier principal)
// IR 12/03/13 modification de la structure pour prise en compte rapide d'autres commandes
//struct  struct_Cde_BP
//{
//    unsigned char   ub1_Sauvegarde :1;      // ordre de sauvegarde en Eeprom : mise à 1 par com BP / mise à 0 par tache de fond après sauvegarde
//
//};
union  struct_Cde_BP
{
    struct {
        unsigned char   Sauvegarde :1;      // ordre de sauvegarde en Eeprom : mise à 1 par com BP / mise à 0 par tache de fond après sauvegarde
        unsigned char   bloqueVolets :1;    // ordre de blocage des volets : mise à 1 et à 0 par com BP
        unsigned char   AllumeTout :1;      // ordre d'allumage forcé : mise à 1 et à 0 par com BP
    } ub1;
    unsigned char   o;

};
	