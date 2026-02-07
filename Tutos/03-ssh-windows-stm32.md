# Connexion Ethernet + SSH entre Windows et STM32MP1

Objectif :
- Mettre un câble RJ45 entre le PC Windows et la STM32MP1.
- Mettre des IP statiques des deux côtés.
- Vérifier le ping.
- Se connecter en SSH et copier des fichiers en SCP.

## Schéma d’adressage (exemple)

- STM32 : `192.168.1.50/24`
- PC Windows : `192.168.1.10/24`

## 1) Côté STM32 : IP statique (temporaire)

```sh
ifconfig eth0 192.168.1.50 netmask 255.255.255.0 up
```

### Rendre l’IP persistante (systemd-networkd)

Éditer le fichier :

```sh
vi /etc/systemd/network/10-static-eth0.network
```

Contenu :

```ini
[Match]
Name=eth0

[Network]
Address=192.168.1.50/24
```

Quitter en sauvegardant : `:x`

#### Variante « 1 commande »

```sh
printf "[Match]\nName=eth0\n\n[Network]\nAddress=192.168.1.50/24\n" > /etc/systemd/network/10-static-eth0.network
```

## 2) Côté Windows : IP statique sur l’interface Ethernet

### Identifier le nom de l’interface

Dans PowerShell :

```powershell
Get-NetAdapter
```

Repère l’interface branchée (ex: `Ethernet 4`).

### Assigner une IP statique

```powershell
netsh.exe interface ip set address "Ethernet 4" static 192.168.1.10 255.255.255.0
```

## 3) Vérifier la connectivité

```powershell
ping 192.168.1.50
```

## 4) SSH vers la STM32

### Depuis PowerShell ou WSL

Si l’image n’accepte que `ssh-rsa`, utilise :

```powershell
ssh -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa root@192.168.1.50
```

Au premier accès : répondre `yes`.

### Si tu as l’alias WSL

Depuis WSL (cf. tuto bashrc) :

```bash
ssh-stm root@192.168.1.50
```

## 5) Copier un fichier (SCP)

Depuis WSL :

```bash
scp-stm fichier.txt root@192.168.1.50:/chemin_destination
```

## Revenir en DHCP sur Windows

Quand tu veux récupérer un accès Internet « normal » sur cette interface :

```powershell
netsh.exe interface ip set address "Ethernet 4" dhcp
```
