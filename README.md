# STM32 — 24h du code (STM32MP1)

Repo de travail et tutoriels « pas-à-pas » pour prendre en main une carte **STM32MP157C-DK2** sous **Windows + WSL** : réseau, SSH, flash d’OS sur carte SD, et cross-compilation C via le SDK ST.

## Matériel / cible

- Carte : **STM32MP157C-DK2** (Discovery Kit)
- Image / layout : **OP-TEE** (voir le nom des fichiers `*optee*` dans les ressources)

## Démarrage rapide

1. Lire le point d’entrée des tutos : [Tutos/README.md](Tutos/README.md)
2. Flasher l’OS sur carte SD : [Tutos/05-installer-os-starter-package.md](Tutos/05-installer-os-starter-package.md)
3. Mettre le lien Ethernet + SSH : [Tutos/03-ssh-windows-stm32.md](Tutos/03-ssh-windows-stm32.md)
4. Compiler et exécuter un binaire ARM : [Tutos/06-compiler-c-sdk.md](Tutos/06-compiler-c-sdk.md)

## Tutoriels

- Index : [Tutos/README.md](Tutos/README.md)
- WSL (raccourcis + alias) : [Tutos/01-wsl-raccourcis.md](Tutos/01-wsl-raccourcis.md)
- Clavier AZERTY sous Weston : [Tutos/02-clavier-azerty-weston.md](Tutos/02-clavier-azerty-weston.md)
- Windows ↔ STM32 (Ethernet + SSH/SCP) : [Tutos/03-ssh-windows-stm32.md](Tutos/03-ssh-windows-stm32.md)
- Backup / restore carte SD : [Tutos/04-sd-backup-restore-windows.md](Tutos/04-sd-backup-restore-windows.md)
- Installer l’OS ST (Starter Package) : [Tutos/05-installer-os-starter-package.md](Tutos/05-installer-os-starter-package.md)
- Compiler en C via le SDK : [Tutos/06-compiler-c-sdk.md](Tutos/06-compiler-c-sdk.md)

## Ressources locales (dans ce repo)

### Images / fichiers ST (OpenSTLinux)

Le starter package décompressé est présent ici :
- [Ressources/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1](Ressources/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1)

Fichiers utiles (déjà prêts) :
- Image SD (DK2, OP-TEE) :
	- [Ressources/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1/FlashLayout_sdcard_stm32mp157c-dk2-optee.img](Ressources/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1/FlashLayout_sdcard_stm32mp157c-dk2-optee.img)
- Notes ST (update / flash layout) :
	- [Ressources/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1/FlashLayout_sdcard_stm32mp157c-dk2-optee.how_to_update.txt](Ressources/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1/FlashLayout_sdcard_stm32mp157c-dk2-optee.how_to_update.txt)

### Code d’exemple

- Exemple C : [Code/hello.c](Code/hello.c)
- Exemple binaire compilé (si présent) : [Code/hello_arm](Code/hello_arm)

### Sauvegardes

- Dossier prévu pour les images de cartes SD : [Backup](Backup)

## Ressources externes

- Starter Package STM32MP1 (OS ST) : https://www.st.com/en/embedded-software/stm32mp1starter.html
- Developer Package / SDK STM32MP1 : https://www.st.com/en/embedded-software/stm32mp1dev.html#get-software
- BalenaEtcher (flash SD sous Windows) : https://etcher.balena.io/#download-etcher
- `dd` pour Windows (si tu préfères une image `.img` en ligne de commande) : http://www.chrysocome.net/downloads/dd-0.5.zip

## Notes

- Les commandes SSH des tutos ajoutent parfois des options `ssh-rsa` pour compatibilité. Si ton environnement SSH est plus récent, tu peux essayer sans.
