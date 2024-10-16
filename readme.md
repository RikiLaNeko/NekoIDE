
# NekoLang IDE

An Integrated Development Environment (IDE) for the NekoLang programming language, built with Qt.

## Description

This IDE allows you to write, run, and debug NekoLang code. It features syntax highlighting, auto-completion, and an output console to display program output.

## Features

- **Syntax Highlighting**: Keywords, comments, and other syntax elements are highlighted for better readability.
- **Auto-Completion**: Provides suggestions for NekoLang keywords as you type.
- **Dark/Light Theme Toggle**: Switch between dark and light themes.
- **Code Execution**: Run NekoLang code and see the output in the integrated console.
- **User Input Support**: Programs that require user input (`meow` command) prompt for input via GUI dialog boxes.

## Installation

1. **Clone the Repository**

   ```bash
   git clone https://github.com/RikiLaNeko/NekoIDE.git
   ```

2. **Build the Project**

   Open the project in Qt Creator or use the command line with qmake:

   ```bash
   cd neko-lang-ide
   qmake
   make
   ```

## Usage

### Writing Code

Start by writing your NekoLang code in the code editor. The IDE starts with a basic template:

```neko
neko {
    // Write your NekoLang code here
}
```

### Running Code

Click on the Run option in the Run menu or press F5 to execute your code. The output will be displayed in the console below the code editor.

### Saving and Opening Files

- **New File**: Create a new file.
- **Open File**: Open an existing .neko file.
- **Save File**: Save your current work.
- **Save As**: Save your work under a new file name.

### User Input

If your NekoLang program uses the `meow` command for user input, the IDE will prompt you with a dialog box to enter the required value.

## Example

Here's a simple NekoLang program that asks for your name and greets you:

```neko
neko {
    meow name
    purr "Hello, " + name
}
```

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss improvements or bugs.

## License

This project is licensed under the MIT License.

---

# IDE NekoLang (Français)

Un environnement de développement intégré (IDE) pour le langage de programmation NekoLang, construit avec Qt.

## Description

Cet IDE vous permet d'écrire, d'exécuter et de déboguer du code NekoLang. Il propose la coloration syntaxique, l'auto-complétion et une console de sortie pour afficher le résultat de vos programmes.

## Fonctionnalités

- **Coloration Syntaxique** : Les mots-clés, commentaires et autres éléments syntaxiques sont mis en évidence pour une meilleure lisibilité.
- **Auto-Complétion** : Fournit des suggestions pour les mots-clés NekoLang pendant la saisie.
- **Basculer Thème Sombre/Clair** : Changez entre les thèmes sombre et clair.
- **Exécution du Code** : Exécutez du code NekoLang et voyez le résultat dans la console intégrée.
- **Support de l'Entrée Utilisateur** : Les programmes qui nécessitent une entrée utilisateur (commande `meow`) demandent l'entrée via des boîtes de dialogue GUI.

## Installation

1. **Clonez le Référentiel**

   ```bash
   git clone https://github.com/RikiLaNeko/NekoIDE.git
   ```

2. **Construisez le Projet**

   Ouvrez le projet dans Qt Creator ou utilisez la ligne de commande avec qmake :

   ```bash
   cd neko-lang-ide
   qmake
   make
   ```

## Utilisation

### Écrire du Code

Commencez par écrire votre code NekoLang dans l'éditeur de code. L'IDE démarre avec un modèle de base :

```neko
neko {
    // Écrivez votre code NekoLang ici
}
```

### Exécuter le Code

Cliquez sur l'option Run dans le menu Run ou appuyez sur F5 pour exécuter votre code. Le résultat sera affiché dans la console sous l'éditeur de code.

### Enregistrer et Ouvrir des Fichiers

- **Nouveau Fichier** : Créez un nouveau fichier.
- **Ouvrir un Fichier** : Ouvrez un fichier .neko existant.
- **Enregistrer le Fichier** : Enregistrez votre travail en cours.
- **Enregistrer Sous** : Enregistrez votre travail sous un nouveau nom de fichier.

### Entrée Utilisateur

Si votre programme NekoLang utilise la commande `meow` pour une entrée utilisateur, l'IDE vous demandera via une boîte de dialogue d'entrer la valeur requise.

## Exemple

Voici un simple programme NekoLang qui vous demande votre nom et vous salue :

```neko
neko {
    meow name
    purr "Bonjour, " + name
}
```

## Contribuer

Les contributions sont les bienvenues ! Veuillez soumettre une pull request ou ouvrir une issue pour discuter des améliorations ou des bugs.

## Licence

Ce projet est sous licence MIT.
