# 🔌 Hardware

## Spécifications Techniques

*   **Microcontrôleur** : Microchip PIC16F946-I/PT (8-bit)
*   **Alimentation** : Régulateur Step-Down LMR12010YMK (3V-20V vers basse tension)
*   **Horloge** : Quartz 20 MHz

## Interfaces Contrôlées

Le module pilote une série de relais et de variateurs.

### Sorties (Relais)

| Type | Designator | Description | Quantité |
| :--- | :--- | :--- | :--- |
| **Relais Simple** | K1, K2, K3, K8, K9, K12, K15 | Finder 34.51 (1RT, 6A) sur support 93.11 | 7 |
| **Relais Bistable** | K4-K7, K10-K11, K13-K14, K16-K17 | Finder 40.61.6 (1RT, 16A) sur support 95.15.2 | 10 |

### Variateurs

| Composant | Designator | Description | Quantité |
| :--- | :--- | :--- | :--- |
| **Module Gradateur** | U2, U3, U6, U8 | Carte fille SC943-0C | 4 |

## Liste des Composants (BOM)

Voici la liste exhaustive des composants électroniques du module.

| Désignation | Valeur / Réf | Qté |
| :--- | :--- | :--- |
| **Microcontrôleur** | PIC16F946-I/PT | 1 |
| **Régulateur** | Texas Instruments LMR12010YMK | 1 |
| **Superviseur** | NCP803SN308T1G (Reset 3.08V) | 1 |
| **Driver MOSFET** | MIC4426YM (Dual 1.5A) | 1 |
| **Transistors** | ZXMN3A01F (MOSFET N 30V 2A) | 28 |
| **Quartz** | Citizen Finedevice HCM49 20.000MABJ-UT | 1 |
| **Relais Simples** | Finder 34.51 (12Vdc) | 7 |
| **Relais Bistables** | Finder 40.61.6 (12Vdc) | 10 |
| **Cartes Filles** | Variateur SC943-0C | 4 |
| **Diodes** | | |
| - Transil/TVS | SMF5V0A-GS08 (5V 200W) | 23 |
| - Schottky | BAT54C (Double) | 18 |
| - Schottky | PMEG2020AEA (2A 20V) | 1 |
| - Redressement | GF1B (1A 100V) | 1 |
| **Connecteurs** | | |
| - Borniers (Ressort) | Phoenix Contact FFKDSA1/V2 (7.62mm) | 29 |
| - Borniers (Ressort) | Phoenix Contact FFKDS/V2 (5.08mm) | 18 |
| **Passifs** | | |
| - Condensateurs | 100nF, 10uF, 10nF, 18pF, 220pF | ~40 |
| - Résistances | 10k, 22k, 6.8k, 1k, etc. | ~80 |
| - Selfs | 10uH, Ferrites | 3 |
| - Varistances | VDR (275Vac) | 4 |

*Note : Cette liste est une synthèse. Se référer au fichier Excel/CSV complet pour les références précises lors de la fabrication.*
