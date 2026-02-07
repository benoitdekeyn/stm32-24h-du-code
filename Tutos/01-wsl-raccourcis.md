# Configurer des raccourcis dans WSL (bashrc)

Objectif :
- Ouvrir WSL directement dans le bon dossier du repo.
- Avoir des alias pratiques pour `ssh`/`scp` vers la STM32.
- (Optionnel) Sourcer automatiquement l’environnement du SDK.

## Pré-requis

- WSL installé.
- Un shell bash (par défaut sur Ubuntu WSL).

## Étapes

### 1) Éditer `~/.bashrc`

Dans WSL :

```bash
nano ~/.bashrc
```

### 2) Ajouter des lignes à la fin

Ajoute à la fin du fichier (adapte le chemin si nécessaire) :

```bash
# se placer dans un dossier au lancement de wsl
cd "/mnt/c/Users/benoi/Documents/GitHub/stm32-24h-du-code"

# (optionnel) activer le SDK STM32 (voir le tuto compilation)
# source ~/Developer-Package/SDK/Installation_Finale/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

# SSH / SCP STM32 (compatibilité ssh-rsa si nécessaire)
alias ssh-stm='ssh -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa'
alias scp-stm='scp -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa'
```

### 3) Recharger la configuration

```bash
source ~/.bashrc
```

## Vérification rapide

```bash
type ssh-stm
type scp-stm
pwd
```

## Notes

- Les options `ssh-rsa` sont parfois nécessaires avec des images/serveurs configurés avec des algorithmes plus anciens. Si ta configuration SSH est plus récente, essaie d’abord sans options.
