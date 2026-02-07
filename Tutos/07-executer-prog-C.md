# Exécuter un programme C sur la STM32

Objectif : compiler un programme en C sur WSL, le copier sur la STM32 et l’exécuter.

## Prérequis
- Avoir suivi le tuto précédent pour installer le SDK et configurer l’environnement de cross-compilation. (Cf tuto [Cross-Compilation](06-compiler-c-sdk.md))
- Avoir un programme C à compiler (ex : `Code/hello.c`).
- Avoir une connexion SSH fonctionnelle entre WSL et la STM32 (Cf tuto [SSH](03-ssh-windows-stm32.md)).

## 1) Compiler le programme en C sur WSL

Depuis le repo (ex : `Code/hello.c`) :

```bash
$CC Code/hello.c -o Code/hello_arm
```

## 2) Copier le binaire sur la STM32

Copier le binaire (exemple en SSH sur `root@192.168.1.50`) :

```bash
scp Code/hello_arm root@192.168.1.50:/home/root/
```
## 3) Exécuter le programme sur la STM32
Se connecter en SSH à la STM32 :

```bash
ssh root@192.168.1.50
```
Puis sur la STM32 :

```sh
./hello_arm
```
## Vérification
Si tout s’est bien passé, tu devrais voir le message « Hello, STM32! » s’afficher dans le terminal de la STM32.