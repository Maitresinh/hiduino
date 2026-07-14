# Interface galvo sans PCB personnalisé

## Pourquoi ce bloc reste dépendant du kit

Les ensembles 20 kpps se ressemblent, mais leurs drivers n'exposent pas toujours la même entrée. Certains attendent un signal simple, d'autres un signal bipolaire ou différentiel. Le mot "ILDA" dans une annonce ne suffit pas à déterminer le câblage interne.

## Chemin recommandé : entrée simple -5/+5 V

Le MVP cible volontairement un driver doté d'une entrée analogique simple référencée à une masse signal. Cela évite de fabriquer une sortie ILDA différentielle.

Utiliser un module commercial de translation et gain entre la sortie 0-2,048 V du MCP4822 et l'entrée -5/+5 V.

## Module de conversion

Rechercher un module commercial "0-5 V to -5V +5V analog signal converter" avec :

- alimentation symétrique compatible avec le kit ;
- réglage séparé d'offset et de gain ;
- bande passante supérieure à 20 kHz ;
- sortie capable de piloter une charge d'au moins 10 kOhm.

Réglage initial galvo déconnecté :

1. DAC à 2048 -> 0,000 V ;
2. DAC au minimum logiciel -> -0,5 V ;
3. DAC au maximum logiciel -> +0,5 V ;
4. vérifier l'absence d'oscillation à l'oscilloscope si disponible.

## Entrée 0-5 V

Elle peut être utilisée si elle est explicitement documentée. Le MCP4822 alimenté en 3,3 V ne fournit que 0-2,048 V en gain 1x : l'ouverture sera donc limitée sans amplificateur.

## Entrée différentielle

Employer un module de sortie ILDA différentiel. Les deux sorties doivent varier en sens opposé autour du même point commun. Ne pas relier arbitrairement X- à la masse sans confirmation de la notice.

## Test obligatoire

Le premier mouvement se fait :

- laser débranché ;
- amplitude logicielle limitée à environ 10 % ;
- alimentation prête à être coupée ;
- miroir inaccessible aux doigts.
