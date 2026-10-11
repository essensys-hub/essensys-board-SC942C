# Retouche foudre du SC942C : note de schéma

Cette note décrit la variante « retouche foudre » du boîtier auxiliaire (BA) SC942C. La retouche se fait sur un BA déposé, à l'établi, sans refaire de circuit imprimé. Elle s'applique aux cartes en service ; la révision E corrigera le schéma.

| Référence | Valeur |
|---|---|
| Change OpenSpec | `i2c-surge-protection-2026-10-007`, dans le dépôt [essensys-board-SC944D](https://github.com/essensys-hub/essensys-board-SC944D) (`openspec/changes/i2c-surge-protection-2026-10-007/`, décisions D1 à D3 du `design.md`) |
| Ticket | [essensys-hub/essensys-board-SC944D#1](https://github.com/essensys-hub/essensys-board-SC944D/issues/1) |
| Tâches | 2.2 à 2.5 et 2.7 (retouche), 2.9 (publication) |
| BOM de la variante | `BOM_retouche-foudre.csv`, à la racine du dépôt |
| BOM d'origine | `BOM_[No Variations].csv`, inchangée |
| Sources des repères | schéma `SC942C/SC942C_Coeur.SchDoc`, fichier de placement `SC942C/Assembly/SC942C_Pick'nPlace.csv`, BOM « DM » |
| Note du boîtier principal | `docs/retouche-foudre.md` du dépôt essensys-board-SC944D |

## 1. Contexte

Une surtension d'origine atmosphérique a mis hors service un BA et coupé le bus entre le boîtier principal (BP, SC944D) et les BA. L'analyse du change montre des défauts communs à tous les BA (SC940D, SC941C, SC942C, topologie identique) :

- SDA et SCL arrivent de J1/J2 sur les broches RC7/RC6 du PIC16F946 par des résistances de 0 Ω (R1/R2) : le courant injecté dans les diodes de protection du PIC n'est pas limité ;
- l'alimentation du bus (+VBUS) entre par R84 (0 Ω), sans limiteur ni écrêteur propre au BA ;
- C2 (10 µF / 16 V) et D6 (PMEG2020AEA, 20 V) n'ont pas de marge face au rail « +12V » d'environ 14,3 V.

Chaîne d'alimentation d'origine :

```text
+VBUS (J1/J2, chaînage) ─ R84 0 Ω (0805) ─ +Vin ─ FI1 (25 V) ─ D3 ─ « +12V » ≈ 14,3 V ─ C2 10 µF / 16 V
                                     └─ C42 220 pF (0603)                       └─ U1 LMR12010 (VIN 20 V) ─ D6 20 V ─ +5 V
```

## 2. Synthèse des modifications

| Repère | Fonction | Avant | Après | Tâche | Statut |
|---|---|---|---|---|---|
| R1/R2 | Résistances série de SDA/SCL | 0 Ω, 0805 | 47 Ω, 0805 anti-impulsion, Panasonic ERJ-P06J470V (ou Vishay CRCW080547R0FKEAHP) | 2.2 | Base |
| R1/R2 | Idem | — | 100 Ω, seulement aux conditions de la tâche 3.8 | 3.8 | Option |
| R84 | Entrée +VBUS → +Vin | 0 Ω, 0805 | PTC ≥ 30 V en montage volant, courant de maintien à dimensionner selon la mesure 2.1 | 2.3 | Base |
| DRF1 (ajout) | Écrêteur de +Vin | — | TVS 16 V (SMAJ16A ou SMF16A) sur les pastilles de C42, en aval de la PTC | 2.4 | Base |
| C2 | Découplage d'entrée de U1 | 10 µF / 16 V X5R | 10 µF / 35 V X5R 0805 | 2.5 | Base |
| D6 | Diode Schottky du régulateur | PMEG2020AEA, 20 V | Schottky 40 V SOD-323F (PMEG4010CEJ) | 2.5 | Base |
| C27/C29 (empreintes non montées) | Écrêteur secondaire côté PIC | — | BAT54S | 2.7 | **OPTION**, si 2.6 l'autorise |

DRF1 est un repère propre à la retouche : le circuit imprimé n'a pas d'empreinte pour cet écrêteur. Dans la BOM de la variante, les lignes de la retouche ont une description qui commence par « RETOUCHE FOUDRE », et la ligne de l'option une description qui commence par « OPTION RENFORCÉE ».

### Remarque sur la BOM

La variante recopie `BOM_[No Variations].csv` ligne pour ligne (même en-tête, UTF-8 avec BOM), en retirant les repères modifiés de leur ligne d'origine ; les lignes de la retouche sont ajoutées à la fin. R84 (0 Ω, 0805) figure au schéma et dans la BOM « DM » (`SC942C/Assembly/SC942C_BOM_Format‚ (DM).xlsx`), mais ni dans la BOM CSV ni dans le fichier de placement : la ligne PTC de la variante l'ajoute. Ici, C40 est un 100 nF 0805 sans rapport avec l'entrée : la TVS va sur C42 (220 pF, 0603).

## 3. Justification

- **R1/R2 à 47 Ω (décision D1).** C'est le meilleur compromis entre le courant injecté dans le PIC et la marge de niveau bas vue par le microcontrôleur du BP. 33 Ω dépasse les plafonds de courant de la spec ; 100 Ω n'est admis qu'avec les pull-ups de 2,2 kΩ côté BP (tâche 3.8). Le format anti-impulsion tient l'énergie d'une impulsion que le 0805 standard ne tient pas.
- **PTC à la place de R84 (décision D3).** Une TVS d'alimentation finit en court-circuit. Elle doit donc être en aval d'un limiteur réarmable propre au BA. Sinon, une TVS morte éteint tous les BA chaînés sur +VBUS. Aucune PTC de 30 V n'existe en 0805 : la PTC (CMS 1206 ou 1812) est montée en volant, sur fils courts, collée. Son courant de maintien à 50 °C vaut au moins 1,5 fois le courant d'entrée du BA mesuré à la tâche 2.1. Il doit rester nettement inférieur à celui de la PTC de L31 du BP, et la PTC du BA doit déclencher plus vite au courant de court-circuit (sélectivité).
- **TVS sur C42, jamais sur +VBUS.** +VBUS est le nœud de chaînage partagé. La TVS est posée sur +Vin, en aval de la PTC, sur les pastilles 0603 de C42 (conservé).
- **C2 en 35 V, D6 en 40 V.** Ils voient le rail « +12V », qui monte avec l'écrêtage de la TVS. La règle du change demande une tension assignée d'au moins 1,5 fois la tension nominale.
- **BAT54S (option, décision D2).** Montée « en l'air » côté PIC, après R1/R2, sur les empreintes non montées C27/C29 : anode au 0 V, point commun sur la ligne, cathode au +5 V par un fil court. R1/R2 restent à 47 Ω.

## 4. Exception de marge assumée : U1 (LMR12010)

U1 est un LMR12010 : VIN de 20 V en fonctionnement, 24 V en absolu, sous un rail d'environ 14,3 V (1,4 fois au lieu de 1,5 fois). Au courant crête, la SMAJ16A laisse monter +Vin vers 26 V, soit environ 25 V après D3 : c'est au-delà des 24 V de U1 et au niveau des 25 V de FI1. **U1 lâchera avant la TVS.** Sur une carte retouchée, la spec n'exige que le confinement de la défaillance : la PTC et la TVS empêchent qu'un BA défaillant n'éteigne les autres.

| Composant | Contrainte | Statut | Levée |
|---|---|---|---|
| U1 (LMR12010) | 20 V en fonctionnement ; environ 25 V après D3 au courant crête de la TVS | Exception assumée jusqu'à la révision E | Révision E : LMR16006 (4 à 60 V, même brochage), C2 en 50 V, FI1 en tenue supérieure |

## 5. Procédure

1. Déposer le BA, hors tension. Retoucher une carte à la fois. Ne jamais faire d'essai d'injection sur une carte de l'installation.
2. **R1/R2** : dessouder les 0 Ω, poser les 47 Ω ERJ-P06J470V. Contrôle hors tension : 44 à 50 Ω entre J1-1 et RC7, puis entre J1-2 et RC6.
3. **R84** : dessouder le 0 Ω, poser la PTC choisie selon la mesure 2.1 en montage volant sur fils courts entre ses pastilles, la coller.
4. **DRF1** : souder la TVS en montage volant, sur fils courts, sur les pastilles de C42 (cathode sur +Vin, anode au 0 V). Test en mode diode.
5. **C2 et D6** : remplacer par le 10 µF / 35 V et la Schottky 40 V SOD-323F.
6. **Option C27/C29** : seulement si la tâche 2.6 l'autorise.
7. Vérifications (tâches 2.2 à 2.5) :
    - carte seule sous 15,0 V par J1-4/J1-3, SDA et SCL non raccordées : écart entre le +5 V et chaque ligne d'au plus 0,5 V ;
    - le BA démarre, +5 V entre 4,83 et 5,03 V, et la PTC ne déclenche pas pendant 1 h à consommation maximale ;
    - sous 15,0 V, « +12V » entre 14,2 V (moins la chute dans la PTC) et 14,4 V ;
    - sur le bus : niveau bas au microcontrôleur du BP inférieur à 0,85 V et au PIC inférieur à 1,2 V (procédure `NR-board-sc944d-1`) ;
    - sur un échantillon de banc, TVS remplacée par un court-circuit : la PTC déclenche et les autres BA restent alimentés (procédure `NR-board-sc944d-9`).

## 6. Hors périmètre

Liaison équipotentielle du coffret (tâches 4.x), firmware (tâches 5.x), retouche du BP (`docs/retouche-foudre.md` du SC944D) et révision E. Les critères et les mesures de référence sont dans `tasks.md` et `design.md` du change.
