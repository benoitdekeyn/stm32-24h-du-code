# Compiler un programme C pour STM32MP1 via le SDK (WSL)

Objectif :
- Installer le SDK ST.
- Charger l’environnement de cross-compilation.
- Compiler un binaire ARM exécutable sur la STM32.

## Pré-requis

- WSL (Ubuntu ou équivalent).
- Archive du SDK ST correspondant à l’architecture de la machine de build (ici : PC x86_64).

Téléchargement SDK :
- https://www.st.com/en/embedded-software/stm32mp1dev.html#get-software

## 1) Préparer le dossier d’installation

```bash
mkdir -p ~/Developer-Package/SDK
```

## 2) Décompresser l’archive du SDK

Adapte le nom exact du fichier :

```bash
tar -xf SDK-x86_64-stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11.tar.gz -C ~/Developer-Package/SDK
```

## 3) Lancer l’installateur

```bash
cd ~/Developer-Package/SDK/stm32mp1-openstlinux-*/sdk
./st-image-weston-openstlinux-*.sh -d ~/Developer-Package/SDK/Installation_Finale
```

## 4) Charger l’environnement de cross-compilation

À faire avant chaque compilation :

```bash
source ~/Developer-Package/SDK/Installation_Finale/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
```

### Option : le faire automatiquement à l’ouverture de WSL

Ajoute à la fin de `~/.bashrc` :

```bash
echo -e "\nsource ~/Developer-Package/SDK/Installation_Finale/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi\n" >> ~/.bashrc
```

## 5) Compiler un fichier C (exemple)

Depuis le repo (ex : `Code/hello.c`) :

```bash
$CC Code/hello.c -o Code/hello_arm
```

## 6) Exécuter sur la STM32

Copier le binaire (exemple en SSH sur `192.168.1.50`) :

```bash
scp-stm Code/hello_arm root@192.168.1.50:/root/
```

Puis sur la STM32 :

```sh
chmod +x /root/hello_arm
/root/hello_arm
```

## Vérification

Sur WSL, le binaire doit être ARM (et pas x86_64) :

```bash
file Code/hello_arm
```
