# Configurer des raccourcis dans WSL (bashrc)

Objectif :
- Ouvrir WSL directement dans le bon dossier du repo.
- Avoir des alias pratiques pour `ssh`/`scp` vers la STM32.
- Sourcer automatiquement l’environnement du SDK pour compiler sans souci.

## Environnement :
- WSL (Ubuntu ou autre distro) sur Windows 10/11.

## Étapes

### Le fichier `~/.bashrc`

Dans WSL, le fichier `~/.bashrc` contient des lignes de commande qui sont exécutées à chaque ouverture d’un terminal.  
On va l’éditer avec l'éditeur nano pour ajouter nos configurations.

```bash
nano ~/.bashrc
```

### Texte à copier à la fin du fichier

```bash
# se placer dans un dossier au lancement de wsl
cd "/mnt/c/Users/benoi/Documents/GitHub/stm32-24h-du-code"

# activer le SDK STM32 pour la compilation
source ~/Developer-Package/SDK/Installation_Finale/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

# SSH / SCP STM32 (compatibilité ssh-rsa si nécessaire)
alias ssh-stm='ssh -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa'
alias scp-stm='scp -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa'
```

### Recharger la configuration

```bash
source ~/.bashrc
```