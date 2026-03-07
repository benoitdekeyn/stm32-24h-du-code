# Connexion WiFi automatique sur STM32MP1

Objectif :
- Configurer la connexion WiFi pour qu'elle se lance automatiquement au démarrage
- Ajouter plusieurs réseaux WiFi en fallback
- Rendre persistant les paramètres après redémarrage

## Prérequis

- Accès au shell de la STM32MP1 (via port série ST-Link ou SSH)
- L'interface WiFi `wlan0` disponible
- Les outils `wpa_supplicant`, `iw` et `udhcpc` installés

## Architecture des fichiers

La connexion WiFi automatique fonctionne en 2 étapes :

1. **wpa_supplicant** : gère la connexion au réseau WiFi sécurisé
2. **systemd-networkd** : gère l'obtention et la configuration de l'adresse IP

## Étape 1 : Créer le fichier de configuration WiFi

**Fichier à créer :** `/etc/wpa_supplicant/wpa_supplicant-wlan0.conf`

**Contenu :**
```ini
ctrl_interface=/var/run/wpa_supplicant

network={
    ssid="Livebox-D7D0"
    psk="VOTRE_MOT_DE_PASSE_ICI"
    key_mgmt=WPA-PSK
}
```

**Important :**
- `ssid` : le nom exact de votre réseau WiFi (entre guillemets)
- `psk` : votre mot de passe WiFi (entre guillemets)
- `key_mgmt=WPA-PSK` : type de sécurité (WPA2)

**Création via commande :**
```sh
mkdir -p /etc/wpa_supplicant
cat > /etc/wpa_supplicant/wpa_supplicant-wlan0.conf << 'EOF'
ctrl_interface=/var/run/wpa_supplicant

network={
    ssid="Livebox-D7D0"
    psk="VOTRE_MOT_DE_PASSE_ICI"
    key_mgmt=WPA-PSK
}
EOF

chmod 600 /etc/wpa_supplicant/wpa_supplicant-wlan0.conf
```

## Étape 2 : Configurer systemd-networkd pour DHCP

**Fichier à créer :** `/etc/systemd/network/20-wifi.network`

**Contenu :**
```ini
[Match]
Name=wlan0

[Network]
DHCP=yes
```

**Explanation:**
- `[Match] Name=wlan0` : cible l'interface WiFi
- `DHCP=yes` : obtient automatiquement une IP via DHCP

**Création via commande :**
```sh
cat > /etc/systemd/network/20-wifi.network << 'EOF'
[Match]
Name=wlan0

[Network]
DHCP=yes
EOF
```

## Étape 3 : Activer les services au démarrage

```sh
# Activer wpa_supplicant pour l'interface wlan0
systemctl enable wpa_supplicant@wlan0

# Activer systemd-networkd
systemctl enable systemd-networkd

# Redémarrer les services
systemctl restart wpa_supplicant@wlan0
systemctl restart systemd-networkd
```

## Étape 4 : Vérifier que tout fonctionne

```sh
# Vérifier le statut de wpa_supplicant
systemctl status wpa_supplicant@wlan0

# Vérifier le statut du réseau
systemctl status systemd-networkd

# Voir l'IP obtenue
ip addr show wlan0

# Tester la connexion internet
ping -c 4 8.8.8.8
```

## Ajouter plusieurs réseaux WiFi

Si vous voulez que la STM32 se connecte automatiquement au meilleur réseau disponible, vous pouvez ajouter plusieurs réseaux dans le même fichier.

**Fichier à modifier :** `/etc/wpa_supplicant/wpa_supplicant-wlan0.conf`

**Contenu avec 2 réseaux :**
```ini
ctrl_interface=/var/run/wpa_supplicant

network={
    ssid="Livebox-D7D0"
    psk="MOT_DE_PASSE_LIVEBOX"
    key_mgmt=WPA-PSK
    priority=2
}

network={
    ssid="Autre_Reseau_WiFi"
    psk="MOT_DE_PASSE_AUTRE"
    key_mgmt=WPA-PSK
    priority=1
}

network={
    ssid="Reseau_OpenWifi"
    key_mgmt=NONE
    priority=0
}
```

**Explication :**
- `priority` : plus le chiffre est élevé, plus le réseau est prioritaire
- La STM32 essaiera d'abord de se connecter à Livebox (priority=2)
- Si elle n'est pas disponible, elle essaiera le 2e réseau (priority=1)
- `key_mgmt=NONE` : pour un réseau WiFi ouvert sans mot de passe

## Vérifier la connexion

```sh
# Voir quel réseau WiFi est actuellement connecté
iw wlan0 link

# Voir l'adresse IP reçue
ip addr show wlan0

# Voir les logs de wpa_supplicant
journalctl -u wpa_supplicant@wlan0 -n 20
```

## Dépannage

### Le WiFi ne se connecte pas après redémarrage

Vérifiez que le fichier de config est au bon chemin :
```sh
ls -la /etc/wpa_supplicant/wpa_supplicant-wlan0.conf
```

Les permissions doivent être `600` :
```sh
chmod 600 /etc/wpa_supplicant/wpa_supplicant-wlan0.conf
```

Vérifiez les logs :
```sh
systemctl status wpa_supplicant@wlan0
journalctl -u wpa_supplicant@wlan0
```

### Pas d'adresse IPv4 (seulement IPv6)

Attendez quelques secondes après le redémarrage que DHCP obtienne une IP :
```sh
sleep 5
ip addr show wlan0
```

Vous pouvez aussi forcer dhclient manuellement :
```sh
udhcpc -i wlan0
```

## Résumé des fichiers critiques

| Fichier | Chemin | Rôle |
|---------|--------|------|
| Configuration WiFi | `/etc/wpa_supplicant/wpa_supplicant-wlan0.conf` | Stocke le SSID et mot de passe |
| Configuration réseau | `/etc/systemd/network/20-wifi.network` | Demande une IP via DHCP |
| Service wpa_supplicant | Géré par systemd | Lance automatiquement au démarrage |
| Service networkd | Géré par systemd | Gère les interfaces réseau |
