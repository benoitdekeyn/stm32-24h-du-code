# Compiler un programme C pour STM32MP1 via le SDK (WSL)

Objectif :
- Installer le SDK ST.
- Charger l’environnement de cross-compilation.
- Compiler un binaire ARM exécutable sur la STM32.

## Environnement :

- WSL (Ubuntu ou équivalent).
- Archive du SDK ST correspondant à l’architecture de la machine de build (ici : PC x86_64).

## 1) Obtenir le SDK

Au choix :   

- Télécharger l’archive du SDK depuis le site de STMicroelectronics
https://www.st.com/en/embedded-software/stm32mp1dev.html#get-so

- Version 25.06.11 déjà téléchargée  
[SDK-x86_64-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz](../Ressources/SDK-x86_64-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz

## 2) Créer le dossier d’installation

```bash
mkdir -p ~/Developer-Package/SDK
```

## 3) Décompresser l’archive du SDK

Adapter le nom exact en fonction du fichier téléchargé

```bash
tar -xf SDK-x86_64-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz -C ~/Developer-Package/SDK
```

## 4) Lancer l’installateur

```bash
cd ~/Developer-Package/SDK/stm32mp1-openstlinux-*/sdk
./st-image-weston-openstlinux-*.sh -d ~/Developer-Package/SDK/Installation_Finale
```

## 5) Charger l’environnement de cross-compilation

À faire avant chaque compilation :

```bash
source ~/Developer-Package/SDK/Installation_Finale/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
```

### Conseil pratique : le faire automatiquement à l’ouverture de WSL

Aller voir le tuto sur la configuration de raccourcis dans WSL pour éviter de retaper cette commande à chaque fois : [Tuto 1 - Raccourcis WSL](./01-wsl-raccourcis.md) ou simplement exécuter cette commande :

```bash
echo -e "\nsource ~/Developer-Package/SDK/Installation_Finale/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi\n" >> ~/.bashrc
```

## 5) Compiler un fichier C (exemple)

la variable d’environnement `$CC` est automatiquement définie par le script d’installation du SDK pour utiliser le bon compilateur de cross-compilation (ex : `arm-linux-gnueabi-gcc`).

Depuis le repo (ex : `Code/hello.c`) :

```bash
$CC Code/hello.c -o Code/hello_arm
```

## 6) Exécuter sur la STM32

Aller voir le tuto sur l’exécution de programmes C sur la STM32 : [Tuto 7 - Exécuter un programme C](./07-executer-prog-C.md)
