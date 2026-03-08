# Utiliser SSH et SCP pour contrôler et transférer des fichiers

Objectif :
- Se connecter à la carte STM32MP1 via SSH.
- Exécuter des commandes à distance.
- Transférer des fichiers entre le PC et la carte (SCP).
- Gérer les empreintes SSH après changement d'OS.

## Environnement

- Windows 10/11 (PowerShell) ou WSL (Bash)
- Connexion réseau établie (voir [Tuto 16 - Connexion carte](./16-connexion-stm32-windows-wsl.md))
- Connaître l'adresse IP de la carte STM32MP1 (ex: `192.168.7.1`)

---

## Se connecter en SSH

### Connexion basique

*Dans PowerShell ou WSL*

```bash
ssh root@192.168.7.1
```

Remplacer `192.168.7.1` par l'adresse IP réelle de la carte.

### Première connexion

Lors de la première connexion, SSH demande confirmation :

```text
The authenticity of host '192.168.7.1' can't be established.
RSA key fingerprint is SHA256:xxxxxxxxxxxxxxxxxxxxxxxxxxxxx.
Are you sure you want to continue connecting (yes/no)?
```

Répondre `yes` pour accepter et enregistrer l'empreinte.

### Se connecter avec un utilisateur spécifique

```bash
ssh nom_utilisateur@192.168.7.1
```

Par défaut sur STM32MP1, l'utilisateur est `root`.

---

## Gérer les empreintes SSH (Problème de clé modifiée)

### Problème : OS de la carte changé

Si l'OS de la carte a été réinstallé ou modifié, SSH refuse la connexion avec ce message :

```text
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
```

**Cause** : L'empreinte SSH enregistrée ne correspond plus à celle de la nouvelle installation.

### Solution : Supprimer l'ancienne empreinte

#### Sous Windows (PowerShell)

```powershell
ssh-keygen -R 192.168.7.1
```

#### Sous WSL ou Linux

```bash
ssh-keygen -f '/root/.ssh/known_hosts' -R '192.168.7.1'
```

Ou si le fichier est dans le dossier utilisateur :

```bash
ssh-keygen -f '~/.ssh/known_hosts' -R '192.168.7.1'
```

Après suppression, retenter la connexion SSH. L'empreinte sera redemandée.

---

## Exécuter une commande à distance sans session interactive

Au lieu d'entrer en session SSH complète, il est possible d'exécuter directement une commande.

### Syntaxe

```bash
ssh root@192.168.7.1 "commande"
```

Exemple :

```bash
ssh root@192.168.7.1 "ls -l /home/root"
```

### Exécuter plusieurs commandes

```bash
ssh root@192.168.7.1 "commande 1 && commande 2 && commande 3"
```

Exemple :

```bash
ssh root@192.168.7.1 "cd /home/root && echo 'Hello' > message.txt && cat message.txt"
```

### Stocker le résultat dans un fichier local

```bash
ssh root@192.168.7.1 "commande" > fichier.txt
```

Exemple :

```bash 
ssh root@192.168.7.1 "uptime" > uptime.txt
```

---

## Transférer des fichiers avec SCP

SCP (Secure Copy Protocol) permet de copier des fichiers via SSH.

### Syntaxe générale

```bash
scp [options] source user@IP:destination
```

---

## Copier un fichier local vers la carte (Upload)

### Fichier unique

```bash
scp fichier.txt root@192.168.7.1:/home/root/
```

### Fichier avec nouveau nom

```bash
scp fichier.txt root@192.168.7.1:/home/root/nouveau_nom.txt
```

### Plusieurs fichiers

```bash
scp fichier1.txt fichier2.c fichier3.sh root@192.168.7.1:/home/root/
```

---

## Copier un fichier distant vers la machine locale (Download)

### Fichier unique

```bash
scp root@192.168.7.1:/home/root/config.txt .
```

Le `.` signifie "répertoire courant".

### Fichier vers un dossier spécifique

```bash
scp root@192.168.7.1:/var/log/syslog C:\Users\public\Downloads\
```

Ou sous WSL :

```bash
scp root@192.168.7.1:/var/log/syslog ~/Downloads/
```

### Avec renommage

```bash
scp root@192.168.7.1:/home/root/data.log ./sauvegarde_data.log
```

---

## Copier un dossier complet (récursif)

### Copier dossier local vers la carte

```bash
scp -r mon_dossier/ root@192.168.7.1:/home/root/
```

L'option `-r` active le mode récursif.

### Copier dossier distant vers la machine locale

```bash
scp -r root@192.168.7.1:/home/root/projet/ ./sauvegarde_projet/
```

### Écraser complètement un dossier distant

SCP écrase automatiquement les fichiers existants. Pour forcer la copie complète :

```bash
scp -r mon_dossier/* root@192.168.7.1:/home/root/destination/
```

**Attention** : Ceci écrase tous les fichiers portant le même nom dans le dossier destination.

---

## Options SCP utiles

### Préserver les permissions et dates

```bash
scp -p fichier.txt root@192.168.7.1:/home/root/
```

### Mode verbeux (afficher la progression)

```bash
scp -v fichier.txt root@192.168.7.1:/home/root/
```

### Compresser pendant le transfert (plus rapide sur réseau lent)

```bash
scp -C gros_fichier.bin root@192.168.7.1:/home/root/
```

### Limiter la bande passante (en Kbit/s)

```bash
scp -l 1000 fichier.txt root@192.168.7.1:/home/root/
```

Limite à 1000 Kbit/s (~125 Ko/s).

### Spécifier un port SSH personnalisé

Si SSH n'écoute pas sur le port 22 par défaut :

```bash
scp -P 2222 fichier.txt root@192.168.7.1:/home/root/
```

**Attention** : `-P` majuscule pour SCP (différent de `-p` pour ssh).

### Combiner plusieurs options

```bash
scp -rCv mon_dossier/ root@192.168.7.1:/home/root/
```

- `-r` : récursif
- `-C` : compression
- `-v` : verbeux

---

## Compatibilité avec anciennes clés SSH (ssh-rsa)

Certaines images STM32 n'acceptent que les anciennes clés `ssh-rsa`. Si la connexion échoue, ajouter les options :

```bash
ssh -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa root@192.168.7.1
```

Pour SCP :

```bash
scp -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa fichier.txt root@192.168.7.1:/home/root/
```

Pour plus de détails sur cette imcompatibilité, se référer au [Tuto - SSH Windows-STM32](./03-ssh-windows-stm32.md).

---

## Commandes SSH avancées

### Tunnel SSH (Port Forwarding)

Rediriger un port distant vers local :

```bash
ssh -L 8080:localhost:80 root@192.168.7.1
```

Accéder ensuite à `http://localhost:8080` sur le PC pour atteindre le port 80 de la carte.

### Exécuter un script local sur la carte distante

```bash
ssh root@192.168.7.1 'bash -s' < mon_script.sh
```

### Copier via SSH avec pipe (sans SCP)

```bash
cat fichier.txt | ssh root@192.168.7.1 'cat > /home/root/fichier.txt'
```

### Monter un système de fichiers distant (SSHFS)

Nécessite `sshfs` installé :

```bash
sshfs root@192.168.7.1:/home/root ~/stm32_mount
```

Pour démonter :

```bash
fusermount -u ~/stm32_mount
```

---

## Exemples pratiques complets

### Déployer un programme compilé et l'exécuter

```bash
# Compiler localement
$CC mon_programme.c -o mon_programme_arm

# Copier vers la carte
scp mon_programme_arm root@192.168.7.1:/home/root/

# Rendre exécutable et lancer
ssh root@192.168.7.1 "chmod +x /home/root/mon_programme_arm && /home/root/mon_programme_arm"
```

### Sauvegarder un dossier de logs

```bash
# Créer l'archive sur la carte
ssh root@192.168.7.1 "tar -czf /tmp/logs_backup.tar.gz /var/log"

# Télécharger l'archive
scp root@192.168.7.1:/tmp/logs_backup.tar.gz ./

# Nettoyer
ssh root@192.168.7.1 "rm /tmp/logs_backup.tar.gz"
```

### Synchroniser un dossier (rsync via SSH)

Nécessite `rsync` installé :

```bash
rsync -avz --delete mon_dossier/ root@192.168.7.1:/home/root/mon_dossier/
```

Options :
- `-a` : archive (préserve permissions, dates, etc.)
- `-v` : verbeux
- `-z` : compression
- `--delete` : supprime les fichiers distants absents localement

---

## Résumé des commandes principales

```bash
# SSH - Connexion
ssh root@192.168.7.1

# SSH - Supprimer empreinte
ssh-keygen -R 192.168.7.1

# SSH - Commande directe
ssh root@192.168.7.1 "commande"

# SCP - Upload fichier
scp fichier.txt root@192.168.7.1:/home/root/

# SCP - Download fichier
scp root@192.168.7.1:/home/root/fichier.txt ./

# SCP - Upload dossier
scp -r dossier/ root@192.168.7.1:/home/root/

# SCP - Download dossier
scp -r root@192.168.7.1:/home/root/dossier/ ./

# SCP - Options combinées
scp -rCv dossier/ root@192.168.7.1:/home/root/
```

---

## Dépannage

### Permission refusée (Permission denied)

Vérifier que l'utilisateur a les droits sur le dossier de destination :

```bash
ssh root@192.168.7.1 "ls -ld /home/root"
```

### Timeout de connexion

1. Vérifier que la carte est bien accessible : `ping 192.168.7.1`
2. Vérifier qu'aucun pare-feu ne bloque le port 22
3. Vérifier que le service SSH est actif sur la carte : `ssh root@192.168.7.1 "systemctl status sshd"`

### Transfert très lent

1. Utiliser l'option `-C` (compression) pour SCP
2. Vérifier la qualité du réseau
3. Utiliser `rsync` au lieu de `scp` pour reprendre les transferts interrompus