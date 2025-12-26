
#include "constantes.h"
#include "struct.h"

#include "global.h"

#include "gestionsorties.h"


// génération de la trame série d'un variateur
// modification du protocole : 1 start / 8 datas / pas de parité / 2 stop
// par IR le 13/09/2012
unsigned short us_CodeVariateur (char c_valeur)
{
	unsigned short l_us_valeur;

	// start bit = 0, suivi de 7 bits de data
	l_us_valeur = (c_valeur + uc_CDE_VAR_OFFSET) <<1;
	l_us_valeur &= 0x01FE;		// sécurité

	// pas de parité
	// stop bit = 1
	l_us_valeur |= 0x0600;

	return(l_us_valeur);
}
/* ancien traitement : 1 start / 7 datas / 1 parité / 1 stop
unsigned short us_CodeVariateur (char c_valeur)
{
	unsigned char l_uc_Compteur;
	unsigned short l_uc_valeur;
	unsigned char l_b_Parite;

	l_b_Parite = 0;
	l_uc_valeur = c_valeur + uc_CDE_VAR_OFFSET;
	for(l_uc_Compteur=0;l_uc_Compteur<7;l_uc_Compteur++)
	{
		l_b_Parite = (l_uc_valeur & 0x01) ^ l_b_Parite;
		l_uc_valeur = l_uc_valeur >> 1;
	}
	// start bit = 0, suivi de 7 bits de data
	l_uc_valeur = (c_valeur + uc_CDE_VAR_OFFSET) <<1;
	l_uc_valeur &= 0x00FE;		// sécurité

	// parité paire
	if (l_b_Parite !=0)
		l_uc_valeur |= 0x0100;
	// stop bit = 1
	l_uc_valeur |= 0x0200;

	return(l_uc_valeur);
}
 fin ancien traitement */

// modification traitement des rampes : gérées par le gradateur lorsque la consigne est > 100
// IR 13/09/2012
//void vd_CalculRampeVariateur(struct struct_Variateur * pst_Variateur, unsigned char type)
//{
//
//	if (pst_Variateur->c_Eclairage > 100)
//		pst_Variateur->c_Eclairage = 100;		// sécurité : 100% max
//	if (pst_Variateur->c_Eclairage < 0)
//		pst_Variateur->c_Eclairage = 0;		// sécurité : 100% max
//
//        if (pst_Variateur->c_Eclairage != pst_Variateur->c_Consigne)
//	{
//            // pas à faire = valeur finale
//            pst_Variateur->c_Pas = pst_Variateur->c_Eclairage - pst_Variateur->c_Consigne;
//	}
//
//}
/* ancien traitement : rampe géré par le boitier auxiliaire en mode variateur 
void vd_CalculRampeVariateur(struct struct_Variateur * pst_Variateur, unsigned char type)
{

	if (pst_Variateur->c_Eclairage > 100)
		pst_Variateur->c_Eclairage = 100;		// sécurité : 100% max
	if (pst_Variateur->c_Eclairage < 0)
		pst_Variateur->c_Eclairage = 0;		// sécurité : 100% max

	if (pst_Variateur->c_Eclairage != pst_Variateur->c_Consigne)
	{
		if (pst_Variateur->c_Eclairage > pst_Variateur->c_Consigne)
		{
			if ((pst_Variateur->c_Eclairage > (pst_Variateur->c_Consigne + uc_VAR_PAS_DOUX))
				&& (type == uc_MODE_VARIATEUR))
			{	// écart significatif en mode variateur
				// pas à faire = valeur définie pour une rampe douce
				pst_Variateur->c_Pas = uc_VAR_PAS_DOUX;	
			}
			else
			{	// écart faible ou mode TOR
				// pas à faire = valeur finale
				pst_Variateur->c_Pas = pst_Variateur->c_Eclairage - pst_Variateur->c_Consigne;
			}
		}
		else
		{
			if ((pst_Variateur->c_Eclairage < (pst_Variateur->c_Consigne - uc_VAR_PAS_DOUX))
				&& (type == uc_MODE_VARIATEUR))
			{	// écart significatif en mode variateur
				// pas à faire = valeur définie pour une rampe douce
				pst_Variateur->c_Pas = -uc_VAR_PAS_DOUX;
			}
			else
			{	// écart faible ou mode TOR
				// pas à faire = valeur finale
				pst_Variateur->c_Pas = pst_Variateur->c_Eclairage - pst_Variateur->c_Consigne;
			}
		}		
	}

}
fin ancien traitement : rampe géré par le boitier auxiliaire */

//void vd_RampeVariateurs(void)
//{
//	unsigned char l_uc_Compteur;
//
//	for(l_uc_Compteur=0;l_uc_Compteur<uc_VARIATEUR_NB;l_uc_Compteur++)
//	{
//		if (st_Variateurs[l_uc_Compteur].c_Pas !=0)
//		{
//			st_Variateurs[l_uc_Compteur].c_Consigne += st_Variateurs[l_uc_Compteur].c_Pas;
//
//			if (st_Variateurs[l_uc_Compteur].c_Pas >0)
//			{		// pas positif => augmentation
//				if (st_Variateurs[l_uc_Compteur].c_Consigne >= st_Variateurs[l_uc_Compteur].c_Eclairage)
//				{
//					st_Variateurs[l_uc_Compteur].c_Consigne = st_Variateurs[l_uc_Compteur].c_Eclairage;
//					st_Variateurs[l_uc_Compteur].c_Pas =0;		// rampe terminée
//				}
//			}
//			else
//			{		// pas négatif => diminution
//				if (st_Variateurs[l_uc_Compteur].c_Consigne <= st_Variateurs[l_uc_Compteur].c_Eclairage)
//				{
//					st_Variateurs[l_uc_Compteur].c_Consigne = st_Variateurs[l_uc_Compteur].c_Eclairage;
//					st_Variateurs[l_uc_Compteur].c_Pas =0;		// rampe terminée
//				}
//			}
//
//			if (st_Variateurs[l_uc_Compteur].c_Consigne > 100)
//				st_Variateurs[l_uc_Compteur].c_Consigne = 100;	// sécurité
//			if (st_Variateurs[l_uc_Compteur].c_Consigne < 0)
//				st_Variateurs[l_uc_Compteur].c_Consigne = 0;	// sécurité
//
//                        // IR 12/03/2013 : gestion allumage forcé
//                        if (st_Commandes.ub1.AllumeTout == 0)
//                        {
//                            // fonctionnement normal
//
//                            // IR 13/09/2012 : rampe gérée par le gradateur
//                            // IR 12/03/2013 : mode supplémentaire : TOR simple
//                            //if (st_EntreesVariateur[l_uc_Compteur].ub1_EntreeMode == uc_MODE_VARIATEUR)
//                            //{       // mode variateur : gradation par le gradateur lui même
//                            //    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (st_Variateurs[l_uc_Compteur].c_Consigne+ 110);
//                            //}
//                            //else
//                            //{       // mode TOR : pas de gradation
//                            //    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (st_Variateurs[l_uc_Compteur].c_Consigne);
//                            //}
//                            switch (st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode) {
//                                case uc_MODE_TOR :
//                                    // pas de gradation. 0 ou 100%
//                                    if (st_Variateurs[l_uc_Compteur].c_Consigne < 20)
//                                    {
//                                        us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (107); // pour 0%
//                                    }
//                                    else
//                                    {
//                                        us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (106); // pour 100%
//                                    }
//                                break;
//
//                                case uc_MODE_SOFT_START :
//                                case uc_MODE_VARIATEUR :
//                                    // gradation par le gradateur lui même : rampe rapide
//                                    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (st_Variateurs[l_uc_Compteur].c_Consigne+ 110);
//                                break;
//                            }
//                        }
//                        else
//                        {
//                            // allumage forcé
//
//                            switch (st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode) {
//                                case uc_MODE_TOR :
//                                    // pas de gradation.
//                                    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (106); // pour 100%
//                                break;
//
//                                case uc_MODE_SOFT_START :
//                                case uc_MODE_VARIATEUR :
//                                    // gradation par le gradateur lui même
//                                    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (210); // pour 100%
//                                break;
//                            }
//                       }
//		}
//	}
//}

/* IR 12/10/2012 : suppression répétition sur demande de Nicolas Gille
 xxx en cas de remise en service : mettre en conformité avec les commandes de vd_RampeVariateurs() ! (utiliser le même code)
void vd_ConfirmeVariateurs(void)
{
	static	unsigned char s_DelaiConfirm = 0;

	unsigned char l_uc_Compteur;

 	s_DelaiConfirm ++;
	if (s_DelaiConfirm >= uc_VAR_TEMPS_ENTRE_CONFIRMATION)
	{
	 	s_DelaiConfirm =0;

		for(l_uc_Compteur=0;l_uc_Compteur<uc_VARIATEUR_NB;l_uc_Compteur++)
		{
			if (st_Variateurs[l_uc_Compteur].c_Pas ==0)
			{		// pas de commande, ni de rampe en cours
	
				if (st_Variateurs[l_uc_Compteur].c_Consigne > 100)
					st_Variateurs[l_uc_Compteur].c_Consigne = 100;	// sécurité
				if (st_Variateurs[l_uc_Compteur].c_Consigne < 0)
					st_Variateurs[l_uc_Compteur].c_Consigne = 0;	// sécurité
	
                                // IR 13/09/2012 : rampe gérée par le gradateur
                                if (st_EntreesVariateur[l_uc_Compteur].ub1_EntreeMode == uc_MODE_VARIATEUR)
                                {       // mode variateur : gradation par le gradateur lui même
                                    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (st_Variateurs[l_uc_Compteur].c_Consigne+ 110);
                                }
                                else
                                {       // mode TOR : pas de gradation
                                    us_CdeVariateurs[l_uc_Compteur] = us_CodeVariateur (st_Variateurs[l_uc_Compteur].c_Consigne);
                                }
			}
		}	

	}
}
*/

