# Révision 1.1

Corrections principales :

- photodétection définie avec un module OPT101 ;
- DAC SPI MCP4822 à la place du MCP4725 I2C ;
- suppression des délais millisecondes dans la boucle de jeu ;
- soustraction laser éteint / laser allumé pour compenser la lumière ambiante ;
- modes 8 notes diatoniques et 12 notes chromatiques ;
- octaves par hauteur avec VL53L4CD non bloquant ;
- octave mémorisée au Note On pour stabiliser les notes tenues ;
- cible Teensy 4.0 ;
- machine d'états SAFE_OFF / ARMED / FAULT ;
- tests C++ natifs ;
- PCB personnalisé retiré du chemin critique ;
- pièces 3D limitées aux adaptateurs réellement nécessaires.
