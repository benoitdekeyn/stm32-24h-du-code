# Passer le clavier de QWERTY à AZERTY (Weston)

Sur certaines images ST, l’interface graphique (Weston) démarre avec un clavier en QWERTY.

Objectif : passer le layout en `fr`.

## Pré-requis

- Accès shell sur la STM32 (local ou SSH).
- Weston en cours d’exécution (ou activable via systemd selon l’image).

## Étapes

### 1) Ouvrir la config Weston

```sh
vi /etc/xdg/weston/weston.ini
```

### 2) Modifier la section clavier

Dans le fichier, repère :

```ini
[keyboard]
keymap_layout=us
```

Remplace `us` par `fr` :

```ini
[keyboard]
keymap_layout=fr
```

### 3) Sauvegarder et quitter `vi`

Rappel `vi` :
- `i` : insérer/éditer
- `Échap` : sortir du mode édition
- `:x` : sauver + quitter

### 4) Appliquer les changements

```sh
systemctl restart weston
```

## Attention (piège QWERTY pendant l’édition)

Tant que tu n’as pas redémarré Weston, le clavier reste en QWERTY.
Exemples fréquents :
- `w` ↔ `z`
- `a` ↔ `q`
- `m` ↔ `,`

Si tu galères avec `vi`, fais-le depuis une connexion SSH (où le mapping clavier dépend plutôt de ton PC).
