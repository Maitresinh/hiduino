# Firmware

## Environnements PlatformIO

- `teensy40_basic` : 8/12 notes, sans capteur de hauteur ;
- `teensy40_height` : ajoute le VL53L4CD non bloquant.

```bash
pio run -e teensy40_basic
pio run -e teensy40_height
pio test -e native
```

Le type USB est `MIDI + Serial`. Le port série sert uniquement au diagnostic et n'est pas utilisé dans la boucle critique.

## Modes musicaux

- interrupteur ouvert : 8 notes de Do majeur, Do à Do ;
- interrupteur fermé vers GND : 12 notes chromatiques, Do à Si.

Le changement de mode coupe toutes les notes, recentre le galvo et relance une courte calibration de fond.

## Hauteur

Le VL53L4CD fonctionne avec un budget de 10 ms en continu. Le firmware teste `dataReady()` entre deux balayages. Aucune attente I2C n'est ajoutée au créneau d'une corde.

Par défaut, l'octave est mémorisée au Note On. Une note tenue ne saute donc pas d'octave lorsque la main bouge. `HEIGHT_RETUNE_HELD_NOTES` permet un comportement continu, déconseillé pour commencer.
