# Tower Defense 🏰🛡️

**Tower Defense C-engine** est un moteur de jeu minimaliste développé en langage **C** avec la bibliothèque **SDL2**. Ce projet a été réalisé en une semaine dans le cadre d'un projet d'études, avec pour objectif de créer une base solide et fonctionnelle pour un jeu de type Tower Defense.

## 🚀 Invitation à la Contribution

Ce code est conçu pour être une **base d'apprentissage**. Si vous êtes étudiant ou développeur passionné par le Game Development en C, vous êtes invité à :
- 🔱 **Forker** le dépôt pour expérimenter.
- 🗺️ Créer vos propres **niveaux** et configurations de vagues.
- 🧟 Ajouter de nouveaux types d'**ennemis** avec des comportements variés.
- 🧠 Améliorer l'algorithme de **cheminement** (pathfinding) pour des maps plus complexes.

---

## 🛠️ Installation & Lancement

### 1. Dépendances
Assurez-vous d'avoir installé les bibliothèques de développement SDL2 sur votre système :

Sur **Ubuntu/Debian** :
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

Sur **Arch / Base Arch** :
```bash
sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

Sur **Fedora** :
```bash
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
```

### 2. Compilation
Utilisez `gcc` (ou votre compilateur C préféré) pour compiler le projet :
```bash
gcc *.c -o tower_defense -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
```

### 3. Lancement
Exécutez l'exécutable généré :
```bash
./tower_defense
```

---

## ✨ Fonctionnalités Actuelles

### 🎮 Système de Jeu & Progression
- 📈 **10 Niveaux Progressifs** : Une difficulté croissante répartie sur 10 stages.
- 🌍 **3 Environnements Visuels** : Les textures et le décor changent tous les 3 niveaux pour varier l'expérience.
- 🌊 **Gestion de Vagues Dynamique** : Système de spawn contrôlé, avec possibilité de mettre le jeu en pause ou de quitter en cours de vague.
- 🏆 **Système de Transition** : Écrans de transition fluides entre les niveaux avec pause musicale.

### 🛡️ Défense & Stratégie (Tours)
- 📍 **Placement sur Slots** : Système de placement stratégique sur des emplacements prédéfinis.
- 💡 **Indicateurs de Slots** : Visualisation claire des emplacements disponibles (Jaune) ou bloqués par le décor (Gris).
- ⚙️ **Système d'Amélioration (Upgrade)** :
    - **Dégâts** : Doublés à chaque niveau.
    - **Portée** : Augmentée significativement (+30 pixels).
    - **Cadence** : Réduction du temps de recharge (cooldown) de 33%.
- 🎯 **Ciblage Intelligent** : Les tours verrouillent la première cible à portée et changent automatiquement si elle meurt ou sort de la zone.

### 🧟 Intelligence des Ennemis (Zombies)
- 🧠 **IA de Cheminement** : Suivi rigoureux de chemins complexes définis par matrice.
- 🔄 **Multi-Sentiers** : Support de plusieurs routes possibles pour les ennemis au sein d'un même niveau.
- 🎞️ **Animations Directionnelles** : Sprites animés qui s'adaptent à la direction du mouvement (haut, bas, gauche, droite).
- 🩸 **Résistance Évolutive** : Les points de vie des zombies scalent avec le numéro du niveau.

### 🎨 Graphismes & Rendu
- 🧱 **Système d'Autotiling** : Calcul automatique des tuiles de route (virages, lignes droites) pour un tracé fluide.
- 🖥️ **Rendu Optimisé** : Utilisation de surfaces SDL avec gestion de la transparence et rafraîchissement synchronisé.
- 🖱️ **Curseurs & Menus** : Interface utilisateur complète avec curseur personnalisé et effets visuels au survol (boutons réactifs).

### ⌨️ UI/UX & Interactivité
- 📝 **Saisie Clavier** : Système d'entrée de texte pour saisir votre pseudo avant de commencer.
- 📊 **Tableau de Classement** : Sauvegarde locale et affichage du Top-Ranking des joueurs.
- 🔊 **Contrôle Audio** : Musique d'ambiance avec option de mise en sourdine (Mute) intégrée à l'interface.
- 💖 **HUD Complet** : Affichage en temps réel des Vies, Kills, Score et nombre de tours placées.

---

## 🎨 Structure des Assets

Les graphismes des cartes sont organisés de manière modulaire dans le dossier `assets/Maps/`. 
- **Organisation** : Dossiers `map1/`, `map2/`, `map3/` changeant selon la progression.
- **Tuiles de Décor (IDs 0-14)** : 
    - `0` : Sol de base
    - `1-6` : Routes et virages (gérés par l'autotiling)
    - `7-14` : Éléments de décor (Arbres, Rochers, Buissons, etc.)
- **Modification** : Remplacez simplement les fichiers `.png` pour personnaliser l'univers visuel.

---

## 📄 Licence & Crédits

Ce projet est sous licence **MIT**. Vous êtes libre d'utiliser, de modifier et de distribuer le code.

**Note sur l'inspiration** : Ce projet a initialement utilisé des ressources de [ce dépôt](https://github.com/mmrosadev/AACOMPI), auxquelles j'ai ajouté mes propres fonctionnalités, graphismes et optimisations moteurs.

---
*J'espère que ce moteur saura vous aider dans votre apprentissage du C et de la SDL2 !*
