# Utiliser SSH et le terminal série ST-LINK

Objectif :
- Savoir ce qu'on peut faire rapidement en SSH.
- Configurer correctement une liaison série (`115200`, `CRLF`).
- Détecter le port COM du ST-LINK sous Windows.
- Ajouter un terminal série dans VS Code.

## Environnement

- Windows 10/11
- Carte STM32MP1 connectée en Micro USB (ST-LINK)
- VS Code

---

## 2) Paramètres série recommandés (ST-LINK VCP)

Quand vous ouvrez un terminal série vers la carte :

- `Baudrate` : `115200`
- `Data bits` : `8`
- `Parity` : `None`
- `Stop bits` : `1`
- `Flow control` : `None`

Configuration classique : `115200 8N1`.

### CRLF

Pour envoyer correctement les commandes shell, utiliser `CRLF` en fin de ligne.

- `CR` = Carriage Return (`\r`)
- `LF` = Line Feed (`\n`)
- `CRLF` = `\r\n`

Si le terminal envoie seulement `LF`, certaines consoles peuvent mal interpréter la touche Entrée.

---

## 3) Détecter le port COM sous Windows

Dans PowerShell, lister les ports série :

```powershell
Get-WMIObject Win32_SerialPort | Select-Object DeviceID, Description
```

Exemple de résultat :

```text
DeviceID Description
-------- -----------
COM3     STMicroelectronics STLink Virtual COM Port
```

Si la commande ne retourne rien :

- vérifier le câble Micro USB,
- vérifier que les drivers ST sont installés,
- débrancher/rebrancher la carte.

---

## 4) Ajouter un terminal série dans VS Code

### Ajouter l'extension "Serial Monitor"

Lien de l'extension : [Serial Monitor - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-serial-monitor)

Si le lien ne fonctionne pas :

1. Ouvrir VS Code.
2. Aller dans `Extensions` (`Ctrl+Shift+X`).
3. Rechercher `Serial Monitor`.
4. Installer l'extension de Microsoft.

Il est aussi possible d'installer d'autres logiciels de terminal série (ex : PuTTY, Tera Term, minicom) mais l'avantage de l'extension VS Code est d'avoir un terminal intégré à l'IDE.

### Ouvrir la vue moniteur série

Au choix : 
- Ouvrir la vue terminal avec `Ctrl+ù` et trouver l'onglet "Serial Monitor".
- Ouvrir la palette de commandes avec `Ctrl+Maj+P` et ouvrir la commande `User View Container: Focus on Serial Monitor View`.

### Paramétrer la connexion série 

Dans le menu du moniteur série, configurer les otptions suivantes :
- Monitor Mode : `Serial`
- View Mode : `Text`
- Port : Le port COMx idenifié plus haut
- Baud rate : `115200`
- Line ending `CRLF`
- Pour plus de facilité, activer le bouton `Toogle Terminal Mode`

### Tester la connexion série

Appuyer sur `Start Monitoring`

Pour vérifier, taper une commande Linux comme `pwd` qui devrait renvoyer un chemin.

Vous pouvez alors lire les logs de la carte et envoyer des commandes.
