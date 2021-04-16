# Light Painting with a Robot

Les livrables :

Les codes
  => codes de base (fonctions d'asservissement et autre)
  => codes de base avec la liste des consignes pour réaliser une figure
  => Les photos et vidéos

Les simulations
=> Un fichier Excel accompagné de graphs : recueil de valeurs expérimentales ayant permis de trouver les coefficients du correspond proportionnel et dérivé (correcteur intégral inutile puisqu’il n’y a pas d’erreur statique).




Tutoriel d'utilisation : 


1 - Télécharger le logiciel Arduino IDE


2 - Installer les deux librairies : motor et Encoder

Pour cela il faut télécharger le répertoire Arduino et copier les deux dossiers motor et Encoder dans Documents/Arduino/librairies


3 - Ouvrir le fichier souhaité avec Arduino IDE

Si vous souhaitez ouvrir le code d'une forme particulière, télécharger le fichier du répertoire correspondant à la forme



Trois codes de base (à compléter avec des instructions) :

- **PP**
  
  Contient le code de base pour le moteur pas à pas

- **CC**
  
  Contient le code de base pour le moteur à courant continu
  
# Examples : 


https://user-images.githubusercontent.com/30113273/113608778-44727f00-964b-11eb-8930-175f7c6b480f.mp4




Il y a plusieurs figures prêtes à l'emploi :
- La Pokeball
- La Mandala
- Le E
- La Spirale
- La bordure
- Le Nuage
- Le triangle
- Le cube

## Librairies Utilisées

Les librairies sont disponibles dans le dossier "librairie" de ce repertoire github

* [Encoder](https://github.com/PaulStoffregen/Encoder) - "Encoder counts pulses from quadrature encoded signals"
* [PID_v2](https://github.com/gelraen/Arduino-PID-Library) - "PID controller"
* Motor.h - Librairie de Mr Sylvain Bertrand

## Equipes

**Meyssan KANZARI :** Encodeurs, Correcteur PID_v2 et v1, méthode point à points, conception du projet, recherche, recherche et test des différentes librairies, implémentation des méthodes de calcul, réalisation du cahier des charges, conception du robot à moteur pas à pas  
**Raphaël KOSKAS :** Correcteur PID manuel, conception robot à moteur courant continu  
**Richard TAQUET :** Réalisations des formes et participation dans la réalisation  
**Falitianna ANDRIANANTOANDRO :** participation dans la réalisation  

## Remerciements

**Felicia IONASCU**  
**Sylvain BERTRAND**  
**Lionel PREVOST**  
