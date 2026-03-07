# Affichage sur l'écran STM32MP157

## État actuel

✅ **Serveur Flask** : Fonctionne correctement sur la STM32  
✅ **Interface Web** : Accessible via http://192.168.7.1:5000  
❌ **Navigateur kiosk** : Nécessite des bibliothèques supplémentaires

## Options pour l'affichage plein écran

### Option 1 : Installer les dépendances GTK WebKit (Recommandé)

Sur la STM32, exécutez :

```bash
# Se connecter à la STM32
ssh root@192.168.7.1

# Naviguer vers le répertoire
cd /root/serveur_led

# Rendre le script executable
chmod +x install_kiosk_deps.sh

# Installer les dépendances
./install_kiosk_deps.sh

# Lancer le mode kiosk
./start_kiosk.sh
```

Les paquets nécessaires :
- `python3-gi` (GObject Introspection pour Python)
- `gir1.2-gtk-3.0` (GTK 3)
- `gir1.2-webkit2-4.0` (WebKit2 pour GTK)

### Option 2 : Installer un navigateur léger

#### Chromium (si disponible)
```bash
opkg install chromium
# ou
apt-get install chromium-browser

# Lancer en mode kiosk
export DISPLAY=:0
export XDG_RUNTIME_DIR=/run/user/0
chromium-browser --kiosk --disable-infobars http://127.0.0.1:5000
```

#### Links2 (navigateur léger graphique)
```bash
opkg install links2
# ou
apt-get install links2

# Lancer en mode graphique
export DISPLAY=:0
links2 -g http://127.0.0.1:5000
```

### Option 3 : Utiliser Weston avec affichage distant

Si vous avez un autre ordinateur sur le même réseau :

1. Sur votre PC Windows, ouvrez un navigateur
2. Allez à : `http://192.168.7.1:5000`
3. L'interface fonctionne depuis n'importe quel appareil

### Option 4 : Créer une application Pygame simple

Une application personnalisée sans dépendance web (à implémenter) :

```python
# Application graphique simple avec Pygame
import pygame
import requests

# Afficher un gros bouton toggle
# Envoyer des requêtes POST à l'API
```

## Scripts fournis

### `start_kiosk.sh`
Lance automatiquement :
1. Le serveur Flask
2. Le compositeur Weston (si nécessaire)  
3. Le navigateur kiosk en plein écran

### `install_kiosk_deps.sh`
Installe automatiquement les dépendances GTK WebKit

### `kiosk_browser.py`
Application Python qui tente d'afficher le site web en plein écran.  
Essaie dans l'ordre :
1. PyQt5 WebEngine
2. GTK WebKit2
3. Affiche un message d'erreur avec instructions

## Commandes utiles

```bash
# Lancer manuellement le serveur
cd /root/serveur_led
python3 serveur.py

# Lancer le mode kiosk complet
./start_kiosk.sh

# Arrêter tous les processus
pkill -f serveur.py
pkill -f kiosk_browser.py

# Voir les logs
tail -f /root/serveur_led/serveur.log
tail -f /root/serveur_led/kiosk.log
```

## Statut actuel de votre déploiement

Après le dernier déploiement :
- ✅ Serveur Flask : **Démarré** (PID visible dans startup.log)
- ❌ Navigateur kiosk : **Erreur** - WebKit2 manquant

### Pour corriger rapidement

```bash
# Se connecter
ssh root@192.168.7.1

# Installer les dépendances (si apt-get disponible)
apt-get update
apt-get install -y python3-gi gir1.2-webkit2-4.0

# Relancer le kiosk
cd /root/serveur_led
./start_kiosk.sh
```

### ou utiliser opkg (Yocto)

```bash
opkg update
opkg install python3-pygobject webkit2gtk
```

## Alternative : Accès à distance uniquement

Si l'installation de bibliothèques est trop complexe, utilisez simplement :

1. Le serveur Flask tourne déjà sur la STM32
2. Depuis n'importe quel navigateur : http://192.168.7.1:5000
3. L'interface web fonctionne parfaitement

Pas besoin d'affichage local si vous pouvez accéder depuis un PC/tablette/téléphone !
