# Câblage MVP

## Broches Teensy 4.0

| Fonction | Broche |
|---|---:|
| Interrupteur 12 notes, fermé vers GND | 2 |
| Bouton ARM, momentané vers GND | 3 |
| Surveillance interverrouillage | 4 |
| TTL laser | 6 |
| LED d'état externe | 7 |
| CS MCP4822 | 10 |
| MOSI SPI | 11 |
| SCK SPI | 13 |
| OPT101 analogique | A1 / 15 |
| SDA VL53L4CD | 18 |
| SCL VL53L4CD | 19 |

## MCP4822

```text
Teensy 3.3 V  -> VDD
Teensy GND    -> VSS
Teensy 13     -> SCK
Teensy 11     -> SDI
Teensy 10     -> CS
LDAC           -> GND
VOUTA          -> entrée du module de conversion galvo
VOUTB          -> non utilisé
```

Le firmware utilise le gain 1x du MCP4822 alimenté en 3,3 V : la sortie nominale est comprise entre 0 et environ 2,048 V. Le module de conversion galvo fournit ensuite l'offset et le gain nécessaires.

## Driver galvo

Le câblage dépend de la notice du kit.

- MVP recommandé : driver avec entrée simple -5/+5 V référencée à une masse signal ; utiliser le module de translation offset/gain ;
- entrée 0-5 V : possible seulement si elle est explicitement documentée, avec ouverture limitée par la sortie 0-2,048 V ;
- entrée différentielle X+ / X- : hors chemin critique du MVP, sauf achat d'un module ILDA différentiel prêt à l'emploi.

Voir `docs/05_GALVO_INTERFACE.md`.

## OPT101

```text
Teensy 3.3 V -> VCC module
Teensy GND   -> GND module
OUT module   -> 1 kOhm -> A1
A1           -> 1 nF -> GND, facultatif après mesure
```

Alimenter le module en 3,3 V évite de dépasser la tension maximale de l'entrée ADC du Teensy.

## VL53L4CD

```text
Teensy 3.3 V -> VIN du breakout compatible 3,3 V
Teensy GND   -> GND
Teensy 18    -> SDA
Teensy 19    -> SCL
```

## Laser et interverrouillage

```text
5 V régulé -> arrêt d'urgence NF -> contact de capot NF -> VCC laser
GND laser ----------------------------------------------> GND logique
Teensy 6 -> 1 kOhm -> TTL laser
TTL laser -> 10 kOhm -> GND
```

La surveillance utilise la tension après les contacts :

```text
5 V après contacts -> 12 kOhm -> Teensy 4
Teensy 4 -> 22 kOhm -> GND
Teensy 4 -> 100 kOhm -> GND, facultatif mais conseillé
```

Avec 12 kOhm et 22 kOhm, l'entrée reçoit environ 3,24 V lorsque la chaîne est fermée. Une chaîne ouverte donne 0 V. Vérifier au multimètre avant connexion au Teensy.

## Boutons

Les broches 2 et 3 utilisent `INPUT_PULLUP` : interrupteur ou bouton fermé vers GND.
