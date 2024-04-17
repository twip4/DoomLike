# DoomLike - Projet de Clone de Doom Classic en Raycasting

## Description

DoomLike est un projet de jeu vidéo inspiré par le célèbre jeu **Doom**. Ce projet vise à recréer l'expérience de Doom utilisant la technique du raycasting pour générer des environnements 3D à partir d'une perspective à la première personne. Le jeu est développé en C++ et utilise la bibliothèque SDL2 pour la gestion des entrées/sorties et le rendu graphique.

Ce projet est conçu comme un hommage aux jeux de tir classiques et explore les techniques de développement des premiers jeux 3D, tout en intégrant des fonctionnalités modernes pour améliorer l'expérience de jeu.

## Fonctionnalités

- **Moteur de Raycasting** : Simulation 3D des environnements en utilisant le raycasting pour le rendu des murs, plafonds et sols.
- **Système de Combat** : Intégration d'un système de combat avec des ennemis contrôlés par IA.
- **Collectibles et Améliorations** : Possibilité de ramasser des objets qui améliorent les capacités du joueur.
- **Interface Utilisateur** : Affichage des informations du joueur telles que la santé, l'armure et les munitions.

## Technologies Utilisées

- **C++** : Langage de programmation principal.
- **SDL2** : Utilisé pour le rendu graphique, la gestion des événements et la gestion audio.
- **Make** : Utilisé pour automatiser la compilation et la liaison du projet.

## Configuration requise

Pour exécuter ce projet, vous aurez besoin d'un ordinateur capable de compiler et d'exécuter des applications C++ utilisant SDL2. Les instructions spécifiques dépendent de votre système d'exploitation.

## Installation

### Prérequis

Assurez-vous d'avoir installé SDL2 sur votre machine. Vous pouvez le télécharger depuis [le site officiel de SDL](https://www.libsdl.org/download-2.0.php).

### Compilation

1. Clonez ce dépôt sur votre machine locale :

```
git clone https://github.com/votre_username/DoomLike.git
```
2. Naviguez dans le dossier du projet :
```
cd DoomLike
```
3. Compilez le projet :
```
make
```
## Lancement du jeu

Après la compilation, vous pouvez lancer le jeu en exécutant le fichier binaire créé :
```
./DoomLike
```

## Contribution

Les contributions sont les bienvenues. Si vous souhaitez améliorer le jeu DoomLike, veuillez forker le dépôt, apporter vos modifications et soumettre une pull request.

## Licence

Ce projet est sous licence MIT. Voir le fichier [LICENSE](LICENSE) pour plus d'informations.

