# Calibration

## Positions des cordes

Les positions sont calculées entre `DAC_MIN` et `DAC_MAX`. Commencer avec une plage étroite autour de 2048. Une plage plus large ne rend pas le projet plus réussi, seulement plus nerveux.

1. passer en 8 notes ;
2. projeter sur un carton mat ;
3. ajuster `DAC_MIN` et `DAC_MAX` ;
4. vérifier l'espacement ;
5. passer en 12 notes et confirmer que les extrêmes ne changent pas.

## Détection

Le firmware mesure :

```text
signal = lecture_laser_allumé - lecture_laser_éteint
```

Une moyenne de fond est suivie par corde. Le seuil s'applique à `signal - fond`.

Procédure :

1. armer sans main pendant la phase de préchauffage ;
2. relever les valeurs de diagnostic ;
3. présenter une main à différentes hauteurs ;
4. choisir `PHOTO_ON_DELTA` au-dessus du bruit maximal ;
5. régler `PHOTO_OFF_DELTA` à environ 50 à 70 % du seuil d'attaque ;
6. si une corde est plus faible, utiliser le tableau `PER_BEAM_ON_DELTA`.

## Octaves

| Distance VL53L4CD | Décalage |
|---:|---:|
| 80 à 300 mm | -12 demi-tons |
| 300 à 600 mm | 0 |
| 600 à 900 mm | +12 |
| 900 à 1100 mm | +24 |

Une hystérésis de 35 mm évite les oscillations aux frontières. Une mesure hors plage est ignorée. Après 600 ms sans mesure valide, les nouvelles notes reviennent à l'octave de base.

## Mesure de latence

Le script `tools/latency_budget.py` donne une estimation théorique. Pour une mesure réelle :

- filmer à 240 images/s la main et une LED déclenchée avec le Note On ;
- ou enregistrer simultanément la sortie du photodétecteur et un GPIO d'événement à l'oscilloscope ;
- mesurer au moins 100 attaques et publier médiane, 95e percentile et maximum.
