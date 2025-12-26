
#include "constantes.h"
#include "struct.h"
#include "global.h"

#define extern
#include "gestionentrees.h"
#undef extern

#include "hard.h"

// Effectue l'acquisition des entrées
// Applique un traitement anti-rebond
void vd_AcquisitionEntrees(void)
{
	unsigned char l_uc_Compteur;
	unsigned char l_uc_EtatEntree;


	for(l_uc_Compteur=0;l_uc_Compteur<uc_NB_ES;l_uc_Compteur++)
	{
		l_uc_EtatEntree = uc_EtatEntree(l_uc_Compteur);
		if(l_uc_EtatEntree != st_Entrees[l_uc_Compteur].ub1_EtatActuel)
		{
			st_Entrees[l_uc_Compteur].ub1_EtatActuel = l_uc_EtatEntree;
			st_Entrees[l_uc_Compteur].ub4_AntiRebond_TempoChgtEtat = 0;
		}
		else
		{
			if(st_Entrees[l_uc_Compteur].ub4_AntiRebond_TempoChgtEtat < uc_ENTREE_ANTI_REBOND_CHGT_ETAT)
			{
				st_Entrees[l_uc_Compteur].ub4_AntiRebond_TempoChgtEtat++;
			}
			else
			{
				st_Entrees[l_uc_Compteur].ub1_EtatFiltrePourApp = st_Entrees[l_uc_Compteur].ub1_EtatActuel;
			}
		}
	}
}

unsigned char uc_EntreeTOREntreeCarte(unsigned char uc_NumeroEntreeTOR)
{
	unsigned char l_uc_Compteur;
	
	l_uc_Compteur = 0;

#ifdef TYPE_CHAMBRES
        // case BORNIER_CHAMBRES:
	switch(uc_NumeroEntreeTOR)
	{
		case uc_ENTREE_TOR0:
			l_uc_Compteur = uc_ENTREE0;
		break;
		case uc_ENTREE_TOR1:
			l_uc_Compteur = uc_ENTREE2;
		break;
		case uc_ENTREE_TOR2:
        		l_uc_Compteur = uc_ENTREE3;
		break;
		case uc_ENTREE_TOR3:
			l_uc_Compteur = uc_ENTREE9;
		break;
		case uc_ENTREE_TOR4:
			l_uc_Compteur = uc_ENTREE10;
		break;
		case uc_ENTREE_TOR5:
			l_uc_Compteur = uc_ENTREE14;
		break;
		case uc_ENTREE_TOR6:
			l_uc_Compteur = uc_ENTREE18;
		break;
	}
#endif

#ifdef TYPE_PIECES_DE_VIE
        // case BORNIER_PIECES_VIE:
	switch(uc_NumeroEntreeTOR)
	{
		case uc_ENTREE_TOR0:
			l_uc_Compteur = uc_ENTREE0;
		break;
		case uc_ENTREE_TOR1:
			l_uc_Compteur = uc_ENTREE2;
		break;
		case uc_ENTREE_TOR2:
        		l_uc_Compteur = uc_ENTREE3;
		break;
		case uc_ENTREE_TOR3:
			l_uc_Compteur = uc_ENTREE18;
		break;
		case uc_ENTREE_TOR4:
			l_uc_Compteur = uc_ENTREE19;
		break;
	}
#endif

#ifdef TYPE_PIECES_D_EAU
	// case BORNIER_PIECES_EAU:
	switch(uc_NumeroEntreeTOR)
	{
		case uc_ENTREE_TOR0:
			l_uc_Compteur = uc_ENTREE0;
		break;
		case uc_ENTREE_TOR1:
			l_uc_Compteur = uc_ENTREE1;
		break;
		case uc_ENTREE_TOR2:
        		l_uc_Compteur = uc_ENTREE7;
		break;
		case uc_ENTREE_TOR3:
			l_uc_Compteur = uc_ENTREE10;
		break;
		case uc_ENTREE_TOR4:
			l_uc_Compteur = uc_ENTREE11;
		break;
		case uc_ENTREE_TOR5:
			l_uc_Compteur = uc_ENTREE12;
		break;
		case uc_ENTREE_TOR6:
			l_uc_Compteur = uc_ENTREE13;
		break;
		case uc_ENTREE_TOR7:
			l_uc_Compteur = uc_ENTREE14;
		break;
		case uc_ENTREE_TOR8:
			l_uc_Compteur = uc_ENTREE15;
		break;
		case uc_ENTREE_TOR9:
			l_uc_Compteur = uc_ENTREE16;
		break;
		case uc_ENTREE_TOR10:
			l_uc_Compteur = uc_ENTREE17;
		break;
		case uc_ENTREE_TOR11:
			l_uc_Compteur = uc_ENTREE20;
		break;
		case uc_ENTREE_TOR12:
			l_uc_Compteur = uc_ENTREE21;
		break;
	}
#endif

	return l_uc_Compteur;
}	

unsigned char uc_EntreeVoletEntreeCarte(unsigned char uc_NumeroEntreeVolet)
{
	unsigned char l_uc_Compteur;
	
	l_uc_Compteur = 0;

#ifdef TYPE_CHAMBRES
	// case BORNIER_CHAMBRES:
	switch(uc_NumeroEntreeVolet)
	{
            case uc_ENTREE_VOLET0:
		l_uc_Compteur = uc_ENTREE4;
            break;
            case uc_ENTREE_VOLET1:
		l_uc_Compteur = uc_ENTREE5;
            break;
            case uc_ENTREE_VOLET2:
                l_uc_Compteur = uc_ENTREE6;
            break;
            case uc_ENTREE_VOLET3:
		l_uc_Compteur = uc_ENTREE7;
            break;
            case uc_ENTREE_VOLET4:
		l_uc_Compteur = uc_ENTREE11;
            break;
            case uc_ENTREE_VOLET5:
		l_uc_Compteur = uc_ENTREE12;
            break;
            case uc_ENTREE_VOLET6:
		l_uc_Compteur = uc_ENTREE15;
            break;
            case uc_ENTREE_VOLET7:
		l_uc_Compteur = uc_ENTREE16;
            break;
            case uc_ENTREE_VOLET8:
		l_uc_Compteur = uc_ENTREE19;
            break;
            case uc_ENTREE_VOLET9:
		l_uc_Compteur = uc_ENTREE20;
            break;
	}
#endif

#ifdef TYPE_PIECES_DE_VIE
        // case BORNIER_PIECES_VIE:
	switch(uc_NumeroEntreeVolet)
	{
            case uc_ENTREE_VOLET0:
		l_uc_Compteur = uc_ENTREE4;
            break;
            case uc_ENTREE_VOLET1:
		l_uc_Compteur = uc_ENTREE5;
            break;
            case uc_ENTREE_VOLET2:
                l_uc_Compteur = uc_ENTREE6;
            break;
            case uc_ENTREE_VOLET3:
		l_uc_Compteur = uc_ENTREE7;
            break;
            case uc_ENTREE_VOLET4:
		l_uc_Compteur = uc_ENTREE8;
            break;
            case uc_ENTREE_VOLET5:
		l_uc_Compteur = uc_ENTREE9;
            break;
            case uc_ENTREE_VOLET6:
		l_uc_Compteur = uc_ENTREE11;
            break;
            case uc_ENTREE_VOLET7:
		l_uc_Compteur = uc_ENTREE12;
            break;
            case uc_ENTREE_VOLET8:
		l_uc_Compteur = uc_ENTREE13;
            break;
            case uc_ENTREE_VOLET9:
		l_uc_Compteur = uc_ENTREE14;
            break;
            case uc_ENTREE_VOLET10:
		l_uc_Compteur = uc_ENTREE16;
            break;
            case uc_ENTREE_VOLET11:
		l_uc_Compteur = uc_ENTREE17;
            break;
	}
#endif

#ifdef TYPE_PIECES_D_EAU
	// case BORNIER_PIECES_EAU:
	switch(uc_NumeroEntreeVolet)
	{
            case uc_ENTREE_VOLET0:
		l_uc_Compteur = uc_ENTREE2;
            break;
            case uc_ENTREE_VOLET1:
		l_uc_Compteur = uc_ENTREE3;
            break;
            case uc_ENTREE_VOLET2:
                l_uc_Compteur = uc_ENTREE4;
            break;
            case uc_ENTREE_VOLET3:
		l_uc_Compteur = uc_ENTREE5;
            break;
            case uc_ENTREE_VOLET4:
		l_uc_Compteur = uc_ENTREE8;
            break;
            case uc_ENTREE_VOLET5:
		l_uc_Compteur = uc_ENTREE9;
            break;
            case uc_ENTREE_VOLET6:
		l_uc_Compteur = uc_ENTREE18;
            break;
            case uc_ENTREE_VOLET7:
		l_uc_Compteur = uc_ENTREE19;
            break;
	}
#endif

        return l_uc_Compteur;
}	

unsigned char uc_EntreeVariateurEntreeCarte(unsigned char uc_NumeroEntreeVariateur)
{
	unsigned char l_uc_Compteur;
	
	l_uc_Compteur = 0;

#ifdef TYPE_CHAMBRES
	// case BORNIER_CHAMBRES:
	switch(uc_NumeroEntreeVariateur)
	{
		case uc_ENTREE_VARIATEUR0:
			l_uc_Compteur = uc_ENTREE1;
		break;
		case uc_ENTREE_VARIATEUR1:
			l_uc_Compteur = uc_ENTREE8;
		break;
		case uc_ENTREE_VARIATEUR2:
			l_uc_Compteur = uc_ENTREE13;
		break;
		case uc_ENTREE_VARIATEUR3:
			l_uc_Compteur = uc_ENTREE17;
		break;
	}
#endif

#ifdef TYPE_PIECES_DE_VIE
	//case BORNIER_PIECES_VIE:
	switch(uc_NumeroEntreeVariateur)
	{
		case uc_ENTREE_VARIATEUR0:
			l_uc_Compteur = uc_ENTREE1;
		break;
		case uc_ENTREE_VARIATEUR1:
			l_uc_Compteur = uc_ENTREE10;
		break;
		case uc_ENTREE_VARIATEUR2:
			l_uc_Compteur = uc_ENTREE15;
		break;
	}
#endif

#ifdef TYPE_PIECES_D_EAU
	// case BORNIER_PIECES_EAU:
	switch(uc_NumeroEntreeVariateur)
	{
		case uc_ENTREE_VARIATEUR0:
			l_uc_Compteur = uc_ENTREE6;
		break;
	}
#endif

    return l_uc_Compteur;
}	

// -----------------------------------------------------------------------------------------------------

// reprise du traitement pour ne prendre en compte que le front montant de l'entrée
// mode applicatif
// xxx mettre en assemblage conditionnel
void vd_AnalyserEntreesTOR(void)
{
	unsigned char l_uc_Compteur;
	unsigned short l_us_Masque;			// xxx passe sur 16 bits à cause du bornier "pièces de vie"
	unsigned char l_uc_EntreeTOR;

	for(l_uc_Compteur=0;l_uc_Compteur<uc_ENTREE_TOR_NB;l_uc_Compteur++)
	{
		l_us_Masque = 0x01 << l_uc_Compteur;
		l_uc_EntreeTOR = uc_EntreeTOREntreeCarte(l_uc_Compteur);

		if(st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourApp !=
		   st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent)
		{
			// xxx us_EntreesTOR = us_EntreesTOR ^ l_us_Masque;		// front montant et descendant ?
			if(st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourApp == 0)
			{	//st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent = st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourApp NE MARCHE PAS !!!
				st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent = 0;

				// xxx prend en compte uniquement le front montant
				us_EntreesTOR &= ~l_us_Masque;		// front descendant => 0
			}
			else
			{
				st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent = 1;
				// xxx prend en compte uniquement le front montant
				us_EntreesTOR |= l_us_Masque;		// front montant => 1
			}	
		}
		else
		{	
			// xxx prend en compte uniquement le front montant
			us_EntreesTOR &= ~l_us_Masque;		// pas de front => 0
		}
	}
}

// -----------------------------------------------------------------------------------------------------
// traitement prend en compte le front montant et le front descendant de l'entrée
// mode test
// xxx mettre en assemblage conditionnel
/* xxx
void vd_AnalyserEntreesTOR(void)
{
	unsigned char l_uc_Compteur;
	unsigned short l_us_Masque;			// xxx passe sur 16 bits à cause du bornier "pièces de vie"
	unsigned char l_uc_EntreeTOR;

	for(l_uc_Compteur=0;l_uc_Compteur<uc_ENTREE_TOR_NB;l_uc_Compteur++)
	{
		l_us_Masque = 0x01 << l_uc_Compteur;
		l_uc_EntreeTOR = uc_EntreeTOREntreeCarte(l_uc_Compteur);

		if(st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourApp !=
		   st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent)
		{
			us_EntreesTOR = us_EntreesTOR ^ l_us_Masque;		// front montant et descendant 
			if(st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourApp == 0)
			{	//st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent = st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourApp NE MARCHE PAS !!!
				st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent = 0;
			}
			else
			{
				st_Entrees[l_uc_EntreeTOR].ub1_EtatFiltrePourAppPrecedent = 1;
			}	
		}
	}
}
xxx */


// -----------------------------------------------------------------------------------------------------

void vd_AnalyserEntreesVariateurs(void)
{
	unsigned char l_uc_Compteur;
	unsigned char l_uc_EntreeVariateur;

	for(l_uc_Compteur=0;l_uc_Compteur<uc_ENTREE_VARIATEUR_NB;l_uc_Compteur++)
	{
		l_uc_EntreeVariateur = uc_EntreeVariateurEntreeCarte(l_uc_Compteur);
		switch(st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode)
		{
			case uc_MODE_TOR:
			case uc_MODE_SOFT_START:
				if(st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourApp !=
				   st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourAppPrecedent)
				{
					if(st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourApp == 0)
					{	//st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourAppPrecedent = st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourApp NE MARCHE PAS !!!
						st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourAppPrecedent = 0;
					}
					else
					{
						st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourAppPrecedent = 1;
						st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_APPUI_COURT; // xxx
					}	
				}
			break;
			case uc_MODE_VARIATEUR:
				if(st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourApp !=
				   st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourAppPrecedent)
				{
					if(st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourApp == 0)
					{	//st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourAppPrecedent = st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourApp NE MARCHE PAS !!!
						st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourAppPrecedent = 0;
					}
					else
					{
						st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourAppPrecedent = 1;
					}	
                                        st_EntreesVariateur[l_uc_Compteur].uc_CompteurMaintenu = 0;	// RAZ compteur durée état en cours
				}

				if(st_Entrees[l_uc_EntreeVariateur].ub1_EtatFiltrePourApp == 0)
				{
                                        if(st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic < uc_TEMPS_VARIATEUR_SIMPLE_CLIC_DOUBLE_CLIC &&
                                           (st_EntreesVariateur[l_uc_Compteur].ub1_Etat1VueApresEtatStop != 0 ||
                                           st_EntreesVariateur[l_uc_Compteur].ub1_SecondAppuiDetecte != 0))
					{
						st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic++;
					}
                                        else
                                        {
                                                st_EntreesVariateur[l_uc_Compteur].ub1_SecondAppuiDetecte = 0;
                                                st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic = 0;
                                        }

                                        if(st_EntreesVariateur[l_uc_Compteur].ub1_Etat1VueApresEtatStop != 0)
                                        {
                                                st_EntreesVariateur[l_uc_Compteur].ub1_Etat1VueApresEtatStop = 0;
                                                if(st_EntreesVariateur[l_uc_Compteur].ub1_SecondAppuiDetecte != 0)
                                                {
                                                        st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_APPUI_DOUBLE;
                                                        st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic = 0;
                                                        st_EntreesVariateur[l_uc_Compteur].ub1_SecondAppuiDetecte = 0;
                                                        st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic = 0;
                                                }
                                                else
                                                {
                                                        st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_APPUI_COURT;
                                                        st_EntreesVariateur[l_uc_Compteur].ub1_SecondAppuiDetecte = 1;
                                                }
                                        }

					if(st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande == uc_VARIATEUR_APPUI_LONG)
					{
						st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_RIEN;
						st_EntreesVariateur[l_uc_Compteur].ub1_SensCommande = st_EntreesVariateur[l_uc_Compteur].ub1_SensCommande ^ 1;			// xxx à voir
					}
					// uc_COMMANDE_APPUI_COURT et uc_COMMANDE_APPUI_DOUBLE : RAZ par applicatif une fois pris en compte
					// uc_COMMANDE_APPUI_LONG : RAZ dès que l'entrée repasse à 0
				}
				else	// Entréee à 1
				{
                                        st_EntreesVariateur[l_uc_Compteur].ub1_Etat1VueApresEtatStop = 1;

                                        if(st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic < uc_TEMPS_VARIATEUR_SIMPLE_CLIC_DOUBLE_CLIC)
					{
						st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic++;
					}

                                        if(st_EntreesVariateur[l_uc_Compteur].uc_CompteurMaintenu < uc_TEMPS_VARIATEUR_APPUI_COURT_LONG)
					{
						st_EntreesVariateur[l_uc_Compteur].uc_CompteurMaintenu++;
					}
					else
					{
						st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_APPUI_LONG;
						st_EntreesVariateur[l_uc_Compteur].ub1_Etat1VueApresEtatStop = 0;
                                                st_EntreesVariateur[l_uc_Compteur].ub1_SecondAppuiDetecte = 0;
                                                st_EntreesVariateur[l_uc_Compteur].uc_CompteurDoubleClic = 0;
					}
				}
			break;
		}
	}
}

void vd_AnalyserEntreesVolets(void)
{
	unsigned char l_uc_Compteur;
	unsigned char l_uc_EntreeVoletRoulant;

	for(l_uc_Compteur=0;l_uc_Compteur<uc_ENTREE_VOLET_NB;l_uc_Compteur++)
	{
		l_uc_EntreeVoletRoulant = uc_EntreeVoletEntreeCarte(l_uc_Compteur);

		if(st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourApp !=
		   st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourAppPrecedent)
		{
			if(st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourApp == 0)
			{	//st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourAppPrecedent = st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourApp NE MARCHE PAS !!!
				st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourAppPrecedent = 0;
			}
			else
			{
				st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourAppPrecedent = 1;
			}	
			st_EntreesVoletRoulant[l_uc_Compteur].ub4_CompteurMaintenu = 0;	// RAZ compteur durée état en cours
		}
		
		if(st_Entrees[l_uc_EntreeVoletRoulant].ub1_EtatFiltrePourApp == 0)
		{
			if(st_EntreesVoletRoulant[l_uc_Compteur].ub1_Etat1VueApresEtatStop != 0)
			{
				st_EntreesVoletRoulant[l_uc_Compteur].ub3_EtatCommande = uc_VOLET_APPUI_COURT;
				st_EntreesVoletRoulant[l_uc_Compteur].ub1_Etat1VueApresEtatStop = 0;	// RAZ obligatoire sinon déclenchement APPUI_COURT permanent
			}	
			if(st_EntreesVoletRoulant[l_uc_Compteur].ub3_EtatCommande == uc_VOLET_APPUI_LONG)
			{
				st_EntreesVoletRoulant[l_uc_Compteur].ub3_EtatCommande = uc_VOLET_OFF;
			}
			// uc_COMMANDE_APPUI_COURT et uc_COMMANDE_APPUI_DOUBLE : RAZ par applicatif une fois pris en compte
			// uc_COMMANDE_APPUI_LONG : RAZ dès que l'entrée repasse à 0
		}
		else	// Entréee à 1
		{
			if(st_EntreesVoletRoulant[l_uc_Compteur].ub4_CompteurMaintenu < ub4_TEMPS_VOLET_APPUI_COURT_LONG)
			{
				st_EntreesVoletRoulant[l_uc_Compteur].ub4_CompteurMaintenu++;
        			st_EntreesVoletRoulant[l_uc_Compteur].ub1_Etat1VueApresEtatStop = 1;
			}
			else
			{
				st_EntreesVoletRoulant[l_uc_Compteur].ub3_EtatCommande = uc_VOLET_APPUI_LONG;
				st_EntreesVoletRoulant[l_uc_Compteur].ub1_Etat1VueApresEtatStop = 0;
			}					
		}
	}
}
	
// ------------------------------------------------------------- en fin de fichier pour s'affranchir des déclarations de fonctions

// Appelé dans traitement cycle
// Exploite l'état des entrées pour en déduire la commande souhaitée par l'utilisateur
void vd_AnalyserEntrees(void)
{
	vd_AnalyserEntreesTOR();
	vd_AnalyserEntreesVariateurs();
	vd_AnalyserEntreesVolets();
}





