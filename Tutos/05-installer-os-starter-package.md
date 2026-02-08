# Installer l’OS ST (Starter Package) sur carte SD (Windows + WSL)

## Version Plug and Play :
- Télécharger l’image SD de la version 6.6 déjà générée  
[FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.img](../Ressources/FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.img)
- Flasher la carte SD avec cette image (étape 7 du tuto).

Autrement, suivre les étapes ci-dessous pour générer l’image SD soi-même à partir d'une nouvelle version du Starter Package de ST.

## Version coomplète 

Objectif :
- Télécharger l’image officielle ST (Starter Package).
- Générer une image SD (`.raw` / `.img`) via le script ST.
- Flasher la carte SD sous Windows.

## Environnement :
- WSL (Ubuntu ou autre distro) pour exécuter les scripts de ST.
- Windows 10/11 ou autre OS pour flasher la carte SD.

## 1) Télécharger le Starter Package

Obtenir l’archive `FLASH-*.tar.gz` (~1,2 Go) :

- [Sur le site de STMicroelectronics](https://www.st.com/en/embedded-software/stm32mp1starter.html)
- [Version 6.6 déjà téléchargée](../Ressources/FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz)


## 2) Décompresser l’archive

Se placer dans le dossier où est le `.tar.gz` à l'aide de la commande `cd`

```bash
tar -xf FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz
```

## 3) Aller dans le dossier des images

```bash
cd stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1
```

## 4) Trouver le bon `FlashLayout` (.tsv)

Exemple pour une STM32MP157C-DK2 avec OP-TEE :

```bash
find . -name "*stm32mp157c-dk2-optee.tsv"
```

Exemple de résultat à utiliser pour la commande suivante :

```text
./flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv
```

## 5) Générer l’image SD

```bash
./scripts/create_sdcard_from_flashlayout.sh ./flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv
```

## 6) Récupérer le `.raw` généré

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

## 7) Flasher sous Windows (BalenaEtcher)

BalenaEtcher est un outil gratuit pour flasher des images sur des cartes SD disponible pour tous les systèmes d’exploitation.
Aucune contrainte particulière pour l’utilisation de Windows.

- Télécharger : https://etcher.balena.io/#download-etcher
- Ouvrir Etcher
  1. Sélectionner le fichier `.img`
  2. Sélectionner la carte SD
  3. Lancer le flash (environ 10 minutes)

La carte SD peut ensuite être utilisée sur la STM32.

