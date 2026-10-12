# Firmware BA 1.8 — SC942C (chambres, adresse 0x12)

Feature `i2c-surge-protection-2026-10-007`, Refs essensys-hub/essensys-board-SC944D#1 (tâche 5.9).

Sources du firmware BA 1.8, dérivées de la 1.7 (`code_ba (fichiers sources 1.7)`, conservée pour le retour arrière) :

- chien de garde actif (`__CONFIG(HS & WDTEN)`, `WDTCON` 1:16384, environ 0,5 s) ; un PIC bloqué relâche SCL en moins de 2 s ;
- erreurs du module SSP : SSPEN 0 puis 1, CKP à 1, purge de SSPOV et WCOL ; timeout de 50 ms quand une ligne reste basse sans activité ;
- NACK quand le tampon de commande est occupé (SSPBUF laissé plein), au lieu d'accepter puis de perdre la commande ;
- 6e octet de lecture `comm_stat` (signature `0xA0`, drapeau de redémarrage), ignoré par un BP 099-37 qui n'en lit que 5 ;
- LED d'état selon l'état de la communication.

`hard.h` sélectionne le type de ce dépôt (`TYPE_CHAMBRES`). Les sources 1.7 de ce dépôt
sélectionnaient `TYPE_CHAMBRES` quel que soit le dépôt : ne jamais réutiliser
un `.hex` sans vérifier son adresse.

Build de référence : essensys-gcc, `make -C ba BOARD=SC942C` (mêmes sources,
une seule macro `TYPE_*` par cible ; tests hôte NR-gcc-12 à -16).

**Non qualifié.** Aucun BA ne doit être reprogrammé (ICSP) avec ce firmware
avant `NR-board-sc944d-8` et `NR-board-sc944d-11` au vert sur banc. Aucun
`.hex` n'est fourni ici.
