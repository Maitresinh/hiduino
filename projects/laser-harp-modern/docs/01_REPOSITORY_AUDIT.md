# Audit technique des dépôts

## M1r3o/Laser-Harp-Project

À conserver : positions discrètes, blanking pendant le mouvement, détection synchronisée, état indépendant des cordes, test du DAC avant galvo.

À corriger : MCP4725 I2C, attentes de 1 ms par corde, ambiguïtés `nbCordes - 1`, absence d'armement matériel et laser 100 mW. Les fichiers consultés portent CC BY-NC-SA, donc le firmware est réécrit sans copie.

## Trasselfrisyr/laserharp

À conserver : Teensy, `usbMIDI`, hystérésis, gammes et mémorisation des notes. Les versions principales sont multi-lasers et aucune licence globale n'a été trouvée.

## Evanthx/laserharp

À conserver : logique pure séparée du matériel et tests unitaires. Le nouveau projet place mapping, octaves et hystérésis dans un cœur C++ testable.

## cybercraftics/laser_harp

À conserver : octave par distance et pièces génériques sous MIT. À corriger : deux attentes de 50 ms pour les ultrasons, remplacées par un VL53L4CD continu non bloquant.

## Conclusion

Réutiliser les principes validés, mais remplacer les goulets d'étranglement : DAC I2C, délais millisecondes, ultrasons bloquants et sécurité seulement logicielle.
