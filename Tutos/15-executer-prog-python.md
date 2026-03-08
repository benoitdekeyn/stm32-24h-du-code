# Exécuter un programme Python sur STM32

Objectif : exécuter un programme Python sur la STM32.

## Prérequis
- Pouvoir accéder à la STM32 en SSH (Cf tuto [Connexion STM32 à Windows](16-connexion-stm32-windows-wsl.md)).
- Avoir un programme Python à exécuter (ex : `hello_world.py`)

## 1) Copier le programme Python sur la STM32

```bash
scp hello_world.py root@192.168.7.1:/home/root/
```
Remplacer `192.168.7.1` par l'adresse IP de votre STM32.

## 2) Se connecter en SSH à la STM32

```bash
ssh root@192.168.7.1
``` 
Remplacer `192.168.7.1` par l'adresse IP de votre STM32.

## 3) Exécuter le programme Python

```bash
python3 hello_world.py
```

## Vérification
Si tout s’est bien passé, le message « Hello, STM32! » devrait s’afficher dans le terminal de la STM32.