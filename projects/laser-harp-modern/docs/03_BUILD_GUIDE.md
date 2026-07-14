# Guide de construction sans PCB personnalisé

## Phase 0 : préparer le matériel

- laisser le laser débranché ;
- régler le convertisseur 5 V du laser à exactement 5,0 V ;
- identifier les bornes d'entrée du driver galvo ;
- vérifier si l'entrée est 0-5 V, -5/+5 V simple ou différentielle ;
- imprimer uniquement le collier laser, le capot OPT101 et une plaque adaptatrice mesurée.

## Phase 1 : Teensy et DAC

1. câbler le MCP4822 ;
2. relier LDAC à la masse ;
3. flasher l'environnement `teensy40_basic` ;
4. en mode désarmé, mesurer environ 1,024 V au milieu de la course avec le DAC alimenté en 3,3 V et configuré en gain 1x ;
5. vérifier que les extrêmes restent dans la plage étroite configurée.

## Phase 2 : interface galvo

1. déconnecter le galvo ;
2. régler l'offset du module de conversion pour obtenir 0,000 V à la position centrale ;
3. limiter les extrêmes à environ -0,5 V et +0,5 V ;
4. connecter le driver galvo ;
5. armer sans laser ;
6. élargir progressivement l'éventail, sans dépasser l'angle nécessaire.

## Phase 3 : photodétecteur

1. alimenter l'OPT101 en 3,3 V ;
2. placer un tube noir devant le capteur ;
3. tester avec une LED rouge modulée à la place du laser ;
4. vérifier que la différence éclairé / ambiant est positive et répétable ;
5. orienter le capteur près de la tête optique pour recevoir la réflexion de la main.

## Phase 4 : sécurité et laser

1. câbler l'arrêt d'urgence et le contact de capot en série dans le 5 V du laser ;
2. ajouter une résistance de rappel de 10 kOhm entre TTL laser et masse ;
3. vérifier que l'ouverture d'un contact coupe physiquement le laser ;
4. connecter un laser rouge mesuré à 1 mW maximum ;
5. diriger les faisceaux vers le haut et une zone sans accès oculaire.

## Phase 5 : MIDI

1. démarrer en 8 cordes ;
2. calibrer seuils et positions ;
3. vérifier Note On et Note Off dans un moniteur MIDI ;
4. passer en 12 cordes avec l'interrupteur ;
5. mesurer la latence et les doubles déclenchements.

## Phase 6 : octave par hauteur

1. flasher `teensy40_height` ;
2. monter le VL53L4CD verticalement dans un tube noir ;
3. régler les trois limites de zone ;
4. vérifier que l'octave change sans bloquer les attaques ;
5. conserver par défaut l'octave mémorisée à l'attaque.
