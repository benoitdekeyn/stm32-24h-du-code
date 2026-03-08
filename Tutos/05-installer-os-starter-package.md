# Installer l’OS ST (Starter Package) sur carte SD (Windows + WSL)

Pour ce tutoriel, il y a 3 méthodes : 

- [Version logicielle](#version-logicielle) (recommandée) : télécharger la dernière version du Starter Package de ST, puis graver la carte via la connexion USB via le logiciel STM32 Cube Programmer.
- [Version Plug and Play locale](#version-plug-and-play) : télécharger une image SD de la version 25.06.11 déjà générée et la flasher la carte SD.
- [Version complète](#version-complète) : télécharger la dernière version du Starter Package de ST et le kit de développement pour wsl, pour générer une image SD à partir du script de ST, puis flasher la carte SD sous Windows.

Le transfert de la version logicielle prend 20 minutes contre 6 minutes pour la version Plug and Play.

Cependant, la version plug-and-plpay nécessite que la version complète ait été réalisé une première fois pour la version de l'OS désirée.

La version complète est beaucoup plus longue que la version logicielle, mais permet de stocker des fichiers images de moins de 5 Go pour une version d'OS donnée, flashable sur toute taille de carte SD via la version plug-and-play.

## Version logicielle 

Objectif :
- Installer STM32 Cube Programmer pour flasher la carte SD.
- Télécharger le Starter Package de ST.
- Graver la carte via la connexion USB.

### 1) Préparer la STM32

1. Mettre les switchs DIP `BOOT0` et `BOOT2` situés au dos de la carte, en position `OFF` (boot sur USB).
2. Insérer la carte SD dans le slot de la STM32MP157C-DK2.
3. Alimenter la carte via le port USB-C `CN6 PWR_IN`.
4. Connecter la carte au PC via le port USB-C `CN7 USB`.

### 2) Préparer le Starter Package

1. Obtenir l’archive `FLASH-*.tar.gz` pour x86 (Architecture Intel x64)
  - Sur le site de STMicroelectronics [ST.com](https://www.st.com/en/embedded-software/stm32mp1starter.html)
  - Verison 26.02.18 locale [FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz](../Ressources/Starter%20Package/FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz)
2. Déplacer l'archive dans un dossier à la racine du disque (ex: C:\ST\) car les chemins ont une taille limitée dans le logiciel.
3. Décompresser l'archive

### 3) Installer STM32 Cube Programmer

Télécharger STM32CubeProgrammer depuis le site officiel :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeprog.html)

#### Installation

1. Lancer l'installateur téléchargé.
2. Suivre l'assistant d'installation.
3. **Étape critique** : Accepter l'installation des drivers USB qui apparaîtront en popup pendant l'installation.

Ces drivers permettent à Windows de reconnaître le ST-LINK et les autres interfaces USB ST.

### 4) Connecter la carte dans STM32 Cube Programmer

1. Ouvrir STM32CubeProgrammer.
2. Dans le panneau de droite, sélectionner `ST-LINK` dans le menu déroulant.
3. À côté du menu déroulant `Port`, cliquer sur le bouton pour rafraîcher la liste des ports disponibles.
4. Sélectionner le port USB correspondant à la carte (ex : `USB1`)
3. Cliquer sur le bouton `Connect`.

### 5) Flasher la carte dans STM32 Cube Programmer

1. Aller dans l'onglet `Open File`.
2. Sélectionner le chemin qui contient les fichiers `*.tsv` (ex: `C:\ST\OS\stm32mp1_v26.02.18\flashlayout_st-image-weston\optee`)
3. Sélectionner et ouvrir le fichier `FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv`.
4. S'assurer que le chemin `Binaries path` soit bien le chemin vers le dossier racine contenant tous les fichiers du Starter package (ex: `C:\ST\OS\stm32mp1_v26.02.18`).
5. Cliquer sur le bouton `Download` et attendez 15 minutes que l'OS soit falshé sur la carte STM32.

### 6) Vérification

1. Une fois le flash terminé, débrancher l'alimentation de la carte et la connexion USB au PC.
2. Mettre les switchs DIP `BOOT0` et `BOOT2` situés au dos de la carte, en position `ON` (boot sur SD).
3. Rebrancher l'alimentation de la carte.
4. Observer les LEDs de la carte qui clignotent, signe que l'OS est bien lancé. Si la LED bleue `LD8` clignote, c'est que l'OS est bien lancé (c'est le heartbeat de l'OS).
5. Le premier démarrage peut prendre jusqu'à 5min. (1 min ensuite).

--- 

## Version Plug and Play

ICI, uniquement pour la version 25.06.11 de l'OS, pour une carte SD de 16Go !

- Télécharger l’image SD de la version 25.06.11 de l'OS déjà générée  
[FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.img.zip](../Backup/OpenSTLinux_v25.06.11_FlashLayout_sdcard_stm32mp157c-dk2-optee.img.zip)
- Flasher la carte SD avec cette image en suivant [la dernière étape](#7-flasher-sous-windows-balenaetcher) du tutoriel de la version complète.

Si la version de l'OS ou la taille de la carte n'est pas celle voulu, il faut suivre les étapes de la version complète pour re-générer l’image SD, ou bien suivre la version logicielle qui sera plus rapide.

--- 

## Version complète 

Objectif :
- Télécharger l’image officielle ST (Starter Package).
- Générer une image SD (`.raw` / `.img`) via le script ST.
- Flasher la carte SD sous Windows.

### Environnement :
- WSL (Ubuntu ou autre distro) pour exécuter les scripts de ST.
- Windows 10/11 ou autre OS pour flasher la carte SD.

### 1) Télécharger le Starter Package

Obtenir l’archive `FLASH-*.tar.gz` :

- Sur le site de STMicroelectronics [ST.com](https://www.st.com/en/embedded-software/stm32mp1starter.html)
- Verison 26.02.18 locale [FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz](../Ressources/Starter%20Package/FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz)

### 2) Décompresser l’archive

Se placer dans le dossier où est le `.tar.gz` à l'aide de la commande `cd`

```bash
tar -xf FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz
```

### 3) Aller dans le dossier des images

```bash
cd stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1
```

### 4) Trouver le bon `FlashLayout` (.tsv)

Exemple pour une STM32MP157C-DK2 avec OP-TEE :

```bash
find . -name "*stm32mp157c-dk2-optee.tsv"
```

Exemple de résultat à utiliser pour la commande suivante :

```text
./flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv
```

### 5) Générer l’image SD

```bash
./scripts/create_sdcard_from_flashlayout.sh ./flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv
```

### 6) Récupérer le `.raw` généré

```bash
find . -name "*.raw"
```

Exemple  de résultat à utiliser pour la commande suivante :

```text
./FlashLayout_sdcard_stm32mp157c-dk2-optee.raw
```

Optionnel : renommer en `.img` (plus explicite pour Windows) :

```bash
mv ./FlashLayout_sdcard_stm32mp157c-dk2-optee.raw ./FlashLayout_sdcard_stm32mp157c-dk2-optee.img
```

### 7) Flasher sous Windows (BalenaEtcher)

BalenaEtcher est un outil gratuit pour flasher des images sur des cartes SD disponible pour tous les systèmes d’exploitation.
Aucune contrainte particulière pour l’utilisation de Windows.

- Télécharger : https://etcher.balena.io/#download-etcher
- Ouvrir Etcher
  1. Sélectionner le fichier `.img`
  2. Sélectionner la carte SD
  3. Lancer le flash (environ 10 minutes)

La carte SD peut ensuite être utilisée sur la STM32.

