# Sources auditées

## M1r3o/Laser-Harp-Project
https://github.com/M1r3o/Laser-Harp-Project

- schémas, Gerbers, tests de DAC et firmwares ;
- MCP4725 I2C et attentes millisecondes, remplacés ici pour réduire la latence ;
- certains fichiers sous CC BY-NC-SA 2.0 BE ;
- laser 100 mW explicitement rejeté.

## Trasselfrisyr/laserharp
https://github.com/Trasselfrisyr/laserharp

- Teensy, USB-MIDI, hystérésis et tables de gammes ;
- version principale multi-lasers ;
- aucune licence globale trouvée, donc référence seulement.

## Evanthx/laserharp
https://github.com/Evanthx/laserharp

- logique C++ séparée et tests historiques ;
- référence d'organisation, sans copie.

## cybercraftics/laser_harp
https://github.com/cybercraftics/laser_harp

- licence MIT ;
- code, schémas et pièces 3D ;
- moteur pas-à-pas et deux capteurs ultrasoniques ;
- deux attentes de 50 ms dans la lecture de hauteur, remplacées par un VL53L4CD non bloquant.

## VL53L4CD Arduino
https://github.com/pololu/vl53l4cd-arduino

- budget de mesure minimal de 10 ms ;
- `dataReady()` et `read(false)` permettent une lecture hors boucle critique.
