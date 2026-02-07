# Installer l’OS ST (Starter Package) sur carte SD (Windows + WSL)

Objectif :
- Télécharger l’image officielle ST (Starter Package).
- Générer une image SD (`.raw` / `.img`) via le script ST.
- Flasher la carte SD sous Windows.

## Pré-requis

- Windows + WSL.
- Une carte SD.
- BalenaEtcher.

## 1) Télécharger le Starter Package

Télécharger l’archive `FLASH-*.tar.gz` (~1,2 Go) :
- https://www.st.com/en/embedded-software/stm32mp1starter.html

## 2) Décompresser sous WSL

Place-toi dans le dossier où est le `.tar.gz`, puis :

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

Exemple de résultat :

```text
./flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv
```

## 5) Générer l’image SD

```bash
./scripts/create_sdcard_from_flashlayout.sh \
  ./flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157c-dk2-optee.tsv
```

## 6) Récupérer le `.raw` généré

```bash
find . -name "*.raw"
```

Exemple :

```text
./FlashLayout_sdcard_stm32mp157c-dk2-optee.raw
```

Optionnel : renommer en `.img` (plus explicite pour Windows) :

```bash
mv ./FlashLayout_sdcard_stm32mp157c-dk2-optee.raw ./FlashLayout_sdcard_stm32mp157c-dk2-optee.img
```

## 7) Flasher sous Windows (BalenaEtcher)

- Télécharger : https://etcher.balena.io/#download-etcher
- Ouvrir Etcher
  1. Sélectionner le fichier `.img`
  2. Sélectionner la carte SD
  3. Lancer le flash

La carte SD peut ensuite être utilisée sur la STM32.

## Dépannage rapide

- Si Etcher refuse le fichier : vérifie l’extension `.img` et que le fichier n’est pas compressé.
- Si tu n’es pas sûr du bon `.tsv` : cherche le modèle exact de ta carte (DK1/DK2, optee ou non).
