# Dépendances Python - IoT LED Controller

Ce document liste toutes les dépendances Python nécessaires pour exécuter l'application sur la STM32MP157.

## Sur votre PC Windows (développement)

### Installation Flask (backend local)
```powershell
pip install flask
```

C'est tout ce qu'il faut pour tester localement.

## Sur la STM32MP157 (production)

### 1. Dépendances Flask (serveur web)

Flask est déjà installé sur votre carte (version 3.0.2).

Si besoin de réinstaller:
```bash
pip3 install flask
```

### 2. Dépendances affichage graphique (mode kiosk)

Pour afficher l'interface en plein écran sur l'écran de la STM32:

```bash
# Se connecter à la carte
ssh root@192.168.7.1

# Mettre à jour les dépôts
apt-get update

# Installer les bibliothèques graphiques Python
apt-get install -y python3-pygobject webkitgtk
```

**Détails:**
- `python3-pygobject` (3.46.0) : Bindings Python pour GObject Introspection
- `webkitgtk` (2.44.4) : Moteur de rendu web WebKit pour GTK

Ces paquets permettent à Python d'utiliser GTK 4.0 + WebKit 6.0 pour afficher des pages web.

### 3. Installation automatique

Un script d'installation est fourni:
```bash
ssh root@192.168.7.1 'cd /root/serveur_led && ./install_kiosk_deps.sh'
```

## Vérification des dépendances

### Sur PC Windows
```powershell
pip list | findstr flask
```

Devrait afficher:
```
Flask    3.x.x
```

### Sur STM32
```bash
# Vérifier Flask
python3 -c "import flask; print(flask.__version__)"

# Vérifier GTK + WebKit
python3 -c "import gi; gi.require_version('Gtk', '4.0'); gi.require_version('WebKit', '6.0'); print('OK')"
```

Si tout fonctionne, vous verrez:
```
3.0.2
OK
```

## Résumé des packages installés

| Package | Version | Utilisation | Où |
|---------|---------|-------------|-----|
| Flask | 3.0.2 | Serveur web API | PC + STM32 |
| python3-pygobject | 3.46.0 | Interface GTK | STM32 uniquement |
| webkitgtk | 2.44.4 | Rendu HTML | STM32 uniquement |

## Notes importantes

- **OpenSTLinux Scarthgap** : Les noms de paquets diffèrent des distributions Debian/Ubuntu classiques
- Les paquets comme `gir1.2-webkit2-4.0` n'existent pas sur cette image
- Utilisez toujours les noms courts : `webkitgtk` au lieu de `libwebkit2gtk-4.0-37`

## Dépannage

### Erreur "Namespace WebKit2 not available"
Normal. Votre carte utilise WebKit 6.0, pas WebKit2 4.0.

### Erreur "Failed to open display"
Le programme kiosk doit être lancé en tant qu'utilisateur `weston` avec les bonnes variables d'environnement. Utilisez le script `start_kiosk.sh`.

### Flask démarre mais pas le kiosk
Vérifiez que Weston tourne:
```bash
ps -ef | grep weston
```

Si absent, redémarrez la carte ou lancez Weston manuellement.
