# STM32 — 24h du code (STM32MP1)

Découverte et prise en main de la carte **STM32MP157C-DK2** sous **Windows 11 + WSL 2 (Ubuntu 24.04)**

## Tutoriels locaux

- [Installer l’OS officiel (Starter Package) sur carte SD](05-installer-os-starter-package.md)
- [Compiler un programme C pour STM32MP1 via le SDK](06-compiler-c-sdk.md)
- [Exécuter un programme C sur la STM32P1](07-executer-prog-C.md)
- [Configurer les raccourcis dans WSL (bashrc)](01-wsl-raccourcis.md)
- [Passer le clavier de QWERTY à AZERTY (Weston)](02-clavier-azerty-weston.md)
- [Connexion Ethernet + SSH entre Windows et STM32MP1](03-ssh-windows-stm32.md)
- [Sauvegarder / restaurer une carte SD sous Windows (image .img)](04-sd-backup-restore-windows.md)

## Tutoriels externes

Cours OpenClassroom « Initiez-vous au machine learning »
https://openclassrooms.com/fr/courses/8063076-initiez-vous-au-machine-learning

Cours de l’Université Aix Marseille pour apprendre à utiliser la STM32MP1 : 
https://arnaud-fevrier.pedaweb.univ-amu.fr/IoT/STM32MP1.html

## Ressources locales

### Manuels et datasheets

- Manuel utilisateur [User-Manual-STM32MP157C-DK2.pdf](Ressources/User-Manual-STM32MP157C-DK2.pdf)

### Fichiers de STMicroelectronics déjà téléchargés

- L'OS officiel (Starter Package) de STMicroelectronics pour la STM32MP1  
[FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz](Ressources/FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz)

- Le SDK (Software Development Kit) pour cross-compilation des programmes en C  
[STM32MP1_Developer_Package_v2.0.0.tar.gz](Ressources/STM32MP1_Developer_Package_v2.0.0.tar.gz)

### Images de cartes SD prêtes à être flashées
- OS officiel (Starter Package)  
[FlashLayout_sdcard_stm32mp157c-dk2-optee.img](Ressources/Backup_FlashLayout_sdcard_stm32mp157c-dk2-optee.img) 

- Sauvegarde de la carte SD à la reception de la carte  
[backup_original_07-02-2026_OS_STM.zip](Ressources/Backup/backup_original_07-02-2026_OS_STM.zip)

- Sauvegarde du dernier état de la carte sur l'OS orginal (en français, SSH automatique)  
[Backup\backup_config_IP_enFR_07-02-2026_OS_STM.zip](Ressources/Backup/backup_config_IP_enFR_07-02-2026_OS_STM.zip)

## Liens externes

### STM32MP1 
*Compte STMicroelectronics requis pour le téléchargement*

- Starter Package STM32MP1 (OS ST)  
https://www.st.com/en/embedded-software/stm32mp1starter.html

- Developer Package / SDK STM32MP1  
https://www.st.com/en/embedded-software/stm32mp1dev.html#get-software

### Outils pour flash de cartes SD sous Windows

- BalenaEtcher (flash SD sous Windows)  
https://etcher.balena.io/#download-etcher

- `dd` pour Windows  
http://www.chrysocome.net/downloads/dd-0.5.zip