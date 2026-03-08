# Rediriger le port série ST-Link de Windows vers WSL

Objectif :
- Rediriger la connexion USB série du ST-Link vers WSL.
- Permettre l'utilisation du port série de la carte depuis Linux (WSL).

## Environnement

- Windows 10/11
- PowerShell
- WSL déjà installé

## 1) Installer `usbipd-win`

Dans PowerShell (idéalement en administrateur), exécuter :

```powershell
winget install --interactive --exact dorssel.usbipd-win
```

## 2) Reconnecter la carte puis lister les périphériques USB

1. Débrancher puis rebrancher la carte.
2. Ouvrir un **nouveau** terminal PowerShell.
3. Lancer :

```powershell
usbipd list
```

Repérer l'identifiant `VID:PID` du ST-Link 
Dans ce tuto, on prendra :

- `0483:3752`

## 3) Attacher le ST-Link à WSL

Après avoir lancé WSL, exécuter dans PowerShell :

```powershell
usbipd attach --wsl --hardware-id 0483:3752 --auto-attach
```

Si tout se passe bien, la commande retourne un message contenant `successfully attached`.

## 4) Détacher / arrêter le pont USB

Pour couper la redirection :

1. Fermer le processus PowerShell qui maintient l'attachement.
2. Débrancher puis rebrancher l'appareil.

Le ST-Link redevient alors visible côté Windows tant qu'il n'est pas rattaché à WSL.

## Résumé des commandes

```powershell
# Installation (une seule fois)
winget install --interactive --exact dorssel.usbipd-win

# Vérification des périphériques
usbipd list

# Attacher le ST-Link à WSL
usbipd attach --wsl --hardware-id 0483:3752 --auto-attach
```
