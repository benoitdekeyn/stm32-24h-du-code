# Installer STM32CubeIDE sur Windows ou Debian (WSL)

Objectif :
- Installer STM32CubeIDE pour développer et déboguer des programmes pour le Cortex-M4.
- Configurer l'environnement pour utiliser le ST-LINK.

## Environnement

Au choix (linux conseillée)
- **Windows 10/11** : installation graphique classique.
- **Debian/Ubuntu (WSL)** : installation via script `.sh`.

---

## Installation sur Windows

### 1) Télécharger STM32CubeIDE

Récupérer l'installateur pour Windows (`.exe`) :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeide.html)

### 2) Lancer l'installateur

Double-cliquer sur le fichier `.exe` téléchargé et suivre l'assistant d'installation.

Chemin d'installation par défaut :

```text
C:\ST\STM32CubeIDE_<version>
```

### 3) Configurer les pilotes ST-LINK

Lors de l'installation, accepter l'installation des pilotes USB ST-LINK. Ils sont nécessaires pour la programmation et le débogage de la carte.

### 4) Lancer STM32CubeIDE

L'IDE est accessible depuis le menu Démarrer ou le raccourci bureau.

---

## Installation sur Debian / WSL Ubuntu

### Phase préparatoire : Pont matériel et système

Avant d'installer CubeIDE, il faut préparer l'environnement WSL.

#### A) Rediriger le ST-LINK vers WSL

Voir le tuto complet : [Tuto 12 - Rediriger ST-LINK vers WSL](./12-rediriger-stlink-wsl.md)

#### B) Configurer Bash comme shell par défaut

*Dans le terminal WSL*

Ubuntu utilise par défaut `dash`, un interpréteur léger. Les scripts de ST et du Yocto Project nécessitent `bash`. Changer le shell système par défaut :

```bash
sudo ln -sf /bin/bash /bin/sh
```
Vérifier en tapant `echo $SHELL` qui doit retourner `/bin/bash`.

### 1) Télécharger STM32CubeIDE

Récupérer l'archive d'installation pour Debian/Ubuntu (fichier `.sh` ou `.deb_bundle.sh`) :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeide.html)
- Version 2.1.0 locale : [cubeIDE/](../Ressources/cubeIDE/st-stm32cubeide_2.1.0_27993_20260219_1630_amd64.deb_bundle.sh.zip)

Exemple de fichier (version 2.1.0) :

```text
st-stm32cubeide_2.1.0_27993_20260219_1630_amd64.deb_bundle.sh
```

### 2) Rendre l'installateur exécutable (seulement si l'éxecution est impossible)

```bash
chmod +x st-stm32cubeide_2.1.0_15082_20240214_1402_amd64.deb_bundle.sh
```

### 3) Lancer l'installation

Exécuter l'installateur avec les droits administrateur (il installe dans `/opt/st/`) :

```bash
sudo ./st-stm32cubeide_2.1.0_15082_20240214_1402_amd64.deb_bundle.sh
```

L'installation prend quelques minutes, il faut accepter les conditions d'utilisation et suivre les étapes de l'assistant.

### 4) Créer un alias pour lancer CubeIDE facilement

Ajouter un raccourci dans `~/.bashrc` :

Remplacer `<version>` par la version exacte installée (ex: `2.1.0`) dans la commande suivante :

```bash
echo "alias cubeIDE='/opt/st/stm32cubeide_<version>/stm32cubeide'" >> ~/.bashrc
```

Recharger la configuration :

```bash
source ~/.bashrc
```

### 5) Lancer STM32CubeIDE

Depuis n'importe quel répertoire dans WSL :

```bash
cubeIDE
```
l'IDE STM32CubeIDE doit se lancer avec une interface graphique.
Il est maintenant prêt à être utilisé pour développer et déboguer des programmes pour la carte STM32MP1.