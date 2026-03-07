# STM32 — 24h du code (STM32MP1)

Découverte et prise en main de la carte **STM32MP157C-DK2** sous **Windows 11 + WSL 2 (Ubuntu 24.04)**

## Tutoriels locaux

- [Installer l’OS officiel (Starter Package) sur carte SD](Tutos/05-installer-os-starter-package.md)
- [Compiler un programme C pour STM32MP1 via le SDK](Tutos/06-compiler-c-sdk.md)
- [Exécuter un programme C sur la STM32P1](Tutos/07-executer-prog-C.md)
- [Configurer les raccourcis dans WSL (bashrc)](Tutos/01-wsl-raccourcis.md)
- [Passer le clavier de QWERTY à AZERTY (Weston)](Tutos/02-clavier-azerty-weston.md)
- [Connexion Ethernet + SSH entre Windows et STM32MP1](Tutos/03-ssh-windows-stm32.md)
- [Sauvegarder / restaurer une carte SD sous Windows (image .img)](Tutos/04-sd-backup-restore-windows.md)
- [Connexion WiFi automatique sur STM32MP1](Tutos/08-connexion-wifi-automatique.md)

## Tutoriels externes

Cours OpenClassroom « Initiez-vous au machine learning »
https://openclassrooms.com/fr/courses/8063076-initiez-vous-au-machine-learning

Cours de l’Université Aix Marseille pour apprendre à utiliser la STM32MP1 : 
https://arnaud-fevrier.pedaweb.univ-amu.fr/IoT/STM32MP1.html

## Ressources locales

### Manuels

- Manuel utilisateur   
[User-Manual-STM32MP157C-DK2.pdf](Ressources/User-Manual-STM32MP157C-DK2.pdf)

- Mail de T. Grandpierre pour la configuration de la carte   
[Mail T. Grandpierre](Ressources/Mail-tutoriel-Thierry-Grandpierre.pdf)

### Fichiers de STMicroelectronics déjà téléchargés

- L'OS officiel (Starter Package) de STMicroelectronics pour la STM32MP1  
[FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz](Ressources/Starter%20Package/FLASH-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz)

- Le SDK (Software Development Kit) pour cross-compilation des programmes en C  
[SDK-x86_64-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz](Ressources/SDK/SDK-x86_64-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v26.02.18.tar.gz)

- Le logiciel cubeMX pour Debian 
[cubeMX_6.3.0_20240212_ubuntu2204.deb](Ressources/cubeMX/cubeMX_6.3.0_20240212_ubuntu2204.deb)

- Le logiciel cubeIDE pour Debian
[st-stm32cubeide_2.1.0_27993_20260219_1630_amd64.deb_bundle.sh.zip](Ressources/cubeIDE/st-stm32cubeide_2.1.0_27993_20260219_1630_amd64.deb_bundle.sh.zip)

- Le logiciel STM32CubeMX pour Debian
[stm32cubemx-lin-v6-17-0.zip](Ressources/cubeMX/stm32cubemx-lin-v6-17-0.zip)

### Images de cartes SD prêtes à être flashées
- OS officiel (Starter Package)  
[backup_28-02-2026_OpenSTLinux_v26.02.18_raw.img.zip](Backup/16%20Go%20SD%20card/backup_28-02-2026_OpenSTLinux_v26.02.18_raw.img.zip) 

- Sauvegarde de la carte SD à la reception de la carte  
[backup_07-02-2026_original_state.img.zip](Backup/16%20Go%20SD%20card/backup_07-02-2026_original_state.img.zip)

## Liens externes

### STM32MP1 
*Compte STMicroelectronics requis pour le téléchargement*

- Starter Package STM32MP1 (OS ST)  
https://www.st.com/en/embedded-software/stm32mp1starter.html

- Developer Package / SDK STM32MP1  
https://www.st.com/en/embedded-software/stm32mp1dev.html#get-software

- CubeMX (configuration de projets STM32)  
https://www.st.com/en/development-tools/stm32cubemx.html

- CubeIDE (environnement de développement intégré pour STM32)  
https://www.st.com/en/development-tools/stm32cubeide.html

### Outils pour flash de cartes SD sous Windows

- BalenaEtcher (flash SD sous Windows)  
https://etcher.balena.io/#download-etcher

- `dd` pour Windows  
http://www.chrysocome.net/downloads/dd-0.5.zip