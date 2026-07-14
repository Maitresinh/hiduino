# Architecture et budget de latence

Pour chaque corde : laser OFF, écriture SPI du DAC, stabilisation galvo, mesure ambiante, laser ON, mesure éclairée, laser OFF, soustraction et mise à jour MIDI.

Le VL53L4CD, les boutons et les diagnostics sont servis entre deux balayages, jamais dans le créneau critique d'une corde.

Valeurs initiales : SPI 20 MHz, stabilisation 80 µs, montée laser 12 µs, trois lectures ADC ambiantes et trois éclairées, maintien visuel 20 µs.

Avec environ 133 µs par corde :

- 8 cordes : environ 1,06 ms par balayage ;
- 12 cordes : environ 1,60 ms par balayage ;
- fréquence de balayage 12 cordes : environ 626 Hz ;
- cible contrôleur complète : 2 à 4 ms avant synthétiseur.

La latence audio dépend du synthétiseur et du tampon. Viser 64 ou 128 échantillons avec un pilote à faible latence.
