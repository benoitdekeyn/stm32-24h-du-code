# Installer STM32CubeMX sur Windows ou Debian (WSL)

Objectif :
- Installer STM32CubeMX pour configurer les périphériques et générer le code d'initialisation.
- Ajouter une commande simple pour lancer CubeMX depuis le terminal.

## Environnement

Au choix (Linux conseillé) :
- **Windows 10/11** : installation graphique classique.
- **Debian/Ubuntu (WSL)** : installation via archive `.zip` et script graphique.

---

## Installation sur Windows

### 1) Télécharger STM32CubeMX

Récupérer l'installateur Windows sur le site officiel :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubemx.html)

### 2) Lancer l'installateur

Double-cliquer sur l'installateur téléchargé et suivre l'assistant.

### 3) Lancer STM32CubeMX

CubeMX est ensuite accessible depuis le menu Démarrer.

---

## Installation sur Debian / WSL Ubuntu

STM32CubeMX est une application Java. Les versions récentes incluent en général l'environnement nécessaire, mais il reste important de suivre la procédure d'installation ST.

### 1) Télécharger l'archive Linux

Récupérer l'archive Linux (version 6.17.0) sur le site ST :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubemx.html)

- Version 6.17.0 locale : [cubeMX/](../Ressources/cubeMX/stm32cubemx-lin-v6-17-0.zip)

Exemple de fichier :

```text
stm32cubemx-lin-v6-17-0.zip
```

### 2) Extraire l'archive

Dans le dossier où se trouve le `.zip` :

```bash
unzip stm32cubemx-lin-v6-17-0.zip
```

### 3) Lancer l'installation

Lancer le script de setup graphique (installation par défaut dans `/usr/local/`) :

```bash
sudo ./SetupSTM32CubeMX-6.17.0
```

L'interface graphique d'installation va s'ouvrir. Accepter les conditions d'utilisation et suivre les étapes de l'assistant.

### 4) Créer un alias pour lancer CubeMX facilement

Ajouter l'alias dans `~/.bashrc` :

```bash
echo "alias cubeMX='/usr/local/STMicroelectronics/STM32Cube/STM32CubeMX/STM32CubeMX'" >> ~/.bashrc
```

Recharger la configuration :

```bash
source ~/.bashrc
```

### 5) Lancer STM32CubeMX

Depuis n'importe quel répertoire :

```bash
cubeMX
```

CubeMX doit s'ouvrir avec son interface graphique.