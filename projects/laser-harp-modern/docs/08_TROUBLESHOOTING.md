# Dépannage

## Le galvo ne bouge pas

- vérifier la sortie du MCP4822 au multimètre ;
- vérifier le centre et les extrêmes après le module de conversion ;
- confirmer la masse et la nature simple/différentielle de l'entrée ;
- ne pas augmenter brutalement l'amplitude.

## Les cordes clignotent

- augmenter légèrement `VISIBLE_DWELL_US` ;
- réduire le nombre de cordes pour isoler le problème ;
- diminuer l'ouverture angulaire ;
- vérifier que le galvo est réellement un kit fermé avec driver apparié.

## Notes doubles

- augmenter `PHOTO_ON_DELTA` ;
- augmenter `ATTACK_CONFIRM_FRAMES` à 2 ;
- assombrir et allonger le capot OPT101 ;
- ajouter un filtre rouge ;
- vérifier les réflexions sur les vis ou profilés.

## Note qui reste bloquée

- augmenter `RELEASE_CONFIRM_FRAMES` seulement si le signal saute ;
- vérifier que le capteur revient sous `PHOTO_OFF_DELTA` ;
- tester l'ouverture de l'interverrouillage, qui doit envoyer tous les Note Off ;
- vérifier que le synthétiseur reçoit le canal MIDI 1.

## Octave instable

- monter le VL53L4CD dans un tube noir ;
- augmenter `HEIGHT_HYSTERESIS_MM` ;
- ignorer les mesures au-delà de 1100 mm ;
- conserver `HEIGHT_RETUNE_HELD_NOTES` à `false`.
