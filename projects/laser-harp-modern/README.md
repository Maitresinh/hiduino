# Laser Harp Modern

Projet de harpe laser ouverte à un seul laser, un galvo et sortie USB-MIDI, conçu pour être prototypé sans PCB personnalisé.

## Objectif du MVP

- 8 cordes diatoniques ou 12 cordes chromatiques, sélectionnables par interrupteur ;
- latence électronique visée inférieure à 4 ms entre l'arrivée de la main et l'envoi MIDI, hors synthétiseur et interface audio ;
- octave globale déterminée par la hauteur de la main avec un VL53L4CD optionnel ;
- détection par réflexion de la lumière sur la main, avec soustraction de la lumière ambiante ;
- modules du commerce, borniers et câblage point à point ;
- laser rouge mesuré à 1 mW maximum pour le développement ;
- redémarrage manuel obligatoire après ouverture de l'interverrouillage.

## Ce qui est réellement repris des projets existants

Le projet conserve les principes éprouvés des harpes existantes : positions discrètes du galvo, blanking entre les cordes, état indépendant de chaque note, USB-MIDI Teensy, hystérésis et variation d'octave par distance. Le firmware fourni ici est une réécriture indépendante, car plusieurs dépôts n'ont pas de licence globale claire et certaines implémentations historiques ont une latence trop élevée.

## Architecture sans PCB maison

```text
Teensy 4.0
  |-- SPI --> module MCP4822 --> module offset/gain 0-2,048 V vers -5/+5 V --> driver galvo X
  |-- ADC <-- module OPT101 + capot optique
  |-- I2C <-- VL53L4CD optionnel pour l'octave
  |-- GPIO --> entrée TTL du laser
  |-- USB --> MIDI + port série de diagnostic

5 V laser --> arrêt d'urgence NF --> contact de capot NF --> laser
```

Un seul axe du kit XY est utilisé. Le second reste disponible pour une évolution ultérieure.

## Première lecture

1. `docs/00_SCOPE_AND_STATUS.md`
2. `docs/01_REPOSITORY_AUDIT.md`
3. `bom/BOM_MVP.csv`
4. `docs/03_BUILD_GUIDE.md`
5. `docs/04_WIRING.md`
6. `firmware/README.md`

## Statut honnête

Le code logique et les outils natifs sont testés dans ce dépôt. Le firmware Teensy n'a pas été validé sur le matériel réel dans cet environnement. Les niveaux d'entrée du driver galvo doivent être vérifiés sur le kit acheté avant connexion. Aucun Gerber n'est fourni, volontairement.

## Licence

Le code et la documentation originaux de ce dépôt sont sous licence MIT. Les projets étudiés restent sous leurs licences respectives. Voir `NOTICE.md` et `SOURCES.md`.
