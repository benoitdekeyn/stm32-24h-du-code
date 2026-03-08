# Connecter la carte STM32MP1 à Windows et WSL

Objectif :
- Comprendre le rôle des 3 connexions USB de la carte STM32MP1.
- Installer les drivers Windows pour une reconnaissance automatique.
- Vérifier la connexion réseau et série de la carte.
- Rediriger le port série vers WSL si nécessaire.

## Environnement

- Windows 10/11
- Carte STM32MP1 (ex : STM32MP157C-DK2)
- Multiprise USB-A ou ports USB disponibles sur le PC

## Terminal à distance et transfert de fichiers

Une fois la connexion réseau établie, il est possible de se connecter à la carte en SSH pour exécuter des commandes à distance, ou de transférer des fichiers avec SCP.
Pour plus de détails voir le tutoriel dédié : [Tuto - SSH et SCP](./17-ssh-scp.md) 

## Commnunication série avec le ST-LINK

Le port série virtuel du ST-LINK permet de communiquer avec le Cortex-M4 pour afficher des messages de debug ou interagir avec le système. Il est accessible via le port COM de Windows une fois les drivers installés.
Pour plus de détails sur l'utilisation du port série, voir le tutoriel dédié : [Tuto - Port série ST-LINK](./18-serial-stlink.md)

---

## Les 3 ports USB de la carte STM32MP1

La carte STM32MP1 dispose de 3 connexions USB distinctes :

### 1) Port série Micro USB (ST-LINK)

- **Rôle** : Programmation, débogage et port série virtuel.
- **Protocole** : ST-LINK/V2.
- **Usage** : Communication série avec le Cortex-M4, flash de firmware, debug.

### 2) Réseau OTG USB-C

- **Rôle** : Interface réseau Ethernet over USB.
- **Adresse par défaut** : `192.168.7.1`.
- **Usage** : SSH, transfert de fichiers, connexion réseau sans câble Ethernet physique.

### 3) Alimentation USB-C

- **Rôle** : Alimentation de la carte.
- **Tension** : 5V.
- **Usage** : Alimenter la carte (côté prise RJ45 / carte SD).

**Note** : Les 3 ports peuvent être branchés simultanément sur une multiprise USB-A (avec adaptateurs si nécessaire).

---

## Installation des drivers Windows

Pour que Windows reconnaisse correctement les ports USB de la carte, il faut installer les drivers ST via **STM32CubeProgrammer** et **STM32CubeIDE**.

### 1) Installer STM32CubeProgrammer

Télécharger STM32CubeProgrammer depuis le site officiel :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeprog.html)

#### Installation

1. Lancer l'installateur téléchargé.
2. Suivre l'assistant d'installation.
3. **Étape critique** : Accepter l'installation des drivers USB qui apparaîtront en popup pendant l'installation.

Ces drivers permettent à Windows de reconnaître le ST-LINK et les autres interfaces USB ST.

### 2) Installer STM32CubeIDE

Télécharger STM32CubeIDE depuis le site officiel :

- [Site officiel STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeide.html)
- Voir aussi : [Tuto 11 - Installer CubeIDE](./11-installer-cubeide.md)

#### Installation

1. Lancer l'installateur téléchargé.
2. Suivre l'assistant d'installation.
3. **Étape critique** : Accepter l'installation des drivers USB qui apparaîtront en popup pendant l'installation.

### Résultat après installation

Une fois les deux logiciels installés :

- **USB-C (réseau OTG)** : reconnu comme serveur réseau.
- **Micro USB (ST-LINK)** : reconnu comme interface ST-LINK.

---

## Se connecter à la carte en réseau

La carte STM32MP1 offre plusieurs moyens de connexion réseau selon vos besoins.

### Option 1 : Via USB-C OTG (connexion directe sans câble réseau)

**Avantage** : Connexion rapide sans câble Ethernet ni configuration réseau.

#### Adresse par défaut

Par défaut, la carte STM32MP1 a pour adresse IP :

```text
192.168.7.1
```

#### Tester la connexion SSH

Dans PowerShell :

```powershell
ssh root@192.168.7.1
```

Si la connexion fonctionne, la carte est bien reconnue en réseau.

#### Lister les appareils connectés

Si l'adresse par défaut ne fonctionne pas, lister les appareils réseau :

```powershell
arp -a | sls "192.168."
```

**Note** : Les adresses finissant par `.255` ne sont pas des appareils (adresses de broadcast).

Exemple de sortie :

```text
192.168.7.1       00-11-22-33-44-55     dynamique
192.168.1.10      AA-BB-CC-DD-EE-FF     dynamique
```

Repérer l'adresse de la STM32MP1 dans cette liste.

### Option 2 : Via Ethernet direct (câble RJ45 PC ↔ Carte)

**Avantage** : Connexion stable avec IP statique configurée.

Brancher un câble RJ45 entre le port Ethernet de la carte et celui du PC, puis configurer une IP statique des deux côtés.

Voir le tutoriel dédié : [Tuto 03 - Connexion Ethernet + SSH](./03-ssh-windows-stm32.md)

Ce tuto explique :
- La configuration d'IP statique sur la carte (ex : `192.168.1.50`)
- La configuration d'IP statique sur Windows (ex : `192.168.1.10`)
- La vérification avec `ping` et connexion SSH

### Option 3 : Via réseau local (box / point d'accès)

**Avantage** : La carte rejoint le réseau local et est accessible depuis tous les appareils connectés.

#### Connexion Ethernet (via box / switch)

1. Brancher un câble RJ45 entre la carte et votre box / switch réseau.
2. La carte obtient automatiquement une IP via DHCP.
3. Trouver l'adresse IP de la carte :

```powershell
arp -a | sls "192.168."
```

4. Se connecter en SSH :

```powershell
ssh root@<adresse_ip_trouvée>
```

#### Connexion WiFi

Pour configurer la connexion WiFi automatique au démarrage de la carte :

Voir le tutoriel dédié : [Tuto 08 - Connexion WiFi automatique](./08-connexion-wifi-automatique.md)

Ce tuto explique :
- La configuration de `wpa_supplicant`
- L'ajout de plusieurs réseaux WiFi en fallback
- La vérification de la connexion

Une fois connectée au WiFi, trouver l'adresse IP avec `arp -a | sls "192.168."` puis se connecter en SSH.

---

## Vérifier la connexion série (Micro USB ST-LINK)

### Lister les ports série disponibles

Dans PowerShell :

```powershell
Get-WMIObject Win32_SerialPort | Select-Object DeviceID, Description
```

Exemple de sortie si la carte est détectée :

```text
DeviceID Description
-------- -----------
COM3     STMicroelectronics STLink Virtual COM Port
```

Si la commande ne retourne rien, la carte n'est pas détectée. Vérifier :

- Le câble Micro USB est bien branché.
- Les drivers ont bien été installés (voir section précédente).
- Le port USB fonctionne (tester avec un autre appareil).

---

## Vérifier la connexion avec STM32CubeProgrammer

STM32CubeProgrammer permet de tester rapidement la connexion ST-LINK.

### Procédure

1. Ouvrir STM32CubeProgrammer.
2. Dans le panneau de droite, sélectionner `ST-LINK` dans le menu déroulant.
3. À côté du menu déroulant `Port`, cliquer sur le bouton pour rafraîcher la liste des ports disponibles.
4. Sélectionner le port USB correspondant à la carte (ex : `USB1`)
3. Cliquer sur le bouton `Connect`.

### Résultats possibles

- **Connexion réussie** : La carte est correctement reconnue. Les informations de la puce s'affichent.
- **Bouton grisé ou connexion échouée** : La carte n'est pas bien reconnue. Vérifier :
  - Les drivers sont installés.
  - Le câble Micro USB est bien branché.
  - La carte est alimentée (via USB-C alimentation).

---

## Rediriger le port série ST-LINK vers WSL

Pour utiliser le ST-LINK dans WSL (par exemple pour CubeIDE sous WSL), il faut rediriger le port USB vers WSL avec `usbipd`.

Voir le tutoriel dédié : [Tuto 12 - Rediriger ST-LINK vers WSL](./12-rediriger-stlink-wsl.md)

### Résumé de la procédure

Dans PowerShell (administrateur) :

```powershell
# Installer usbipd-win
winget install --interactive --exact dorssel.usbipd-win

# Lister les périphériques USB
usbipd list

# Rediriger le ST-LINK vers WSL
usbipd attach --wsl --hardware-id 0483:3752 --auto-attach
```

---

## Résumé des commandes de vérification

```powershell
# Tester SSH via USB-C réseau (adresse par défaut)
ssh root@192.168.7.1

# Lister les appareils réseau (utile pour toutes les options réseau)
arp -a | sls "192.168."

# Vérifier les ports série (ST-LINK)
Get-WMIObject Win32_SerialPort | Select-Object DeviceID, Description
```

---

## Schéma récapitulatif des connexions

### Ports USB de la carte

| Port USB        | Type      | Rôle                              | Reconnaissance Windows          |
|-----------------|-----------|-----------------------------------|---------------------------------|
| Micro USB       | ST-LINK   | Programmation, debug, série       | Port série virtuel (COMx)       |
| USB-C (OTG)     | Réseau    | Ethernet over USB (192.168.7.1)   | Serveur réseau                  |
| USB-C (alim)    | Power     | Alimentation 5V                   | Non applicable                  |

### Options de connexion réseau

| Méthode              | Interface        | Configuration                | Tutoriel de référence |
|----------------------|------------------|------------------------------|-----------------------|
| USB-C OTG            | USB-C réseau     | Automatique (192.168.7.1)    | (Ce tuto)             |
| Ethernet direct      | RJ45             | IP statique manuelle         | [Tuto 03](./03-ssh-windows-stm32.md) |
| Ethernet via box     | RJ45             | DHCP automatique             | (Ce tuto)             |
| WiFi via box         | WiFi (wlan0)     | Configuration wpa_supplicant | [Tuto 08](./08-connexion-wifi-automatique.md) |

---

## Dépannage

### La carte n'est pas détectée

1. Vérifier que les 3 câbles USB sont bien branchés.
2. Réinstaller les drivers via CubeProgrammer et CubeIDE.
3. Débrancher et rebrancher la carte.
4. Tester sur un autre port USB du PC.

### Le port série n'apparaît pas

1. Vérifier que le câble Micro USB est bien branché.
2. Vérifier les drivers ST-LINK dans le Gestionnaire de périphériques Windows.
3. Brancher directement sur un port USB du PC (éviter les hubs USB).

### L'adresse 192.168.7.1 ne répond pas

1. Attendre 30 secondes après branchement de l'USB-C (temps de boot du système).
2. Vérifier que le système Linux est bien démarré (observer les LED de la carte).
3. Utiliser `arp -a | sls "192.168."` pour trouver l'adresse réelle.
4. Vérifier les paramètres réseau Windows (désactiver temporairement le pare-feu).
