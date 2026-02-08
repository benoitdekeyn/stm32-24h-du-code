# Passer le clavier de QWERTY à AZERTY (Weston)

Sur certaines images ST, l’interface graphique (Weston) démarre avec un clavier en QWERTY.

Objectif : passer le layout en AZERTY pour correspondre à un clavier français.

## Environnement

Shell sur la STM32 (via SSH ou terminal local) avec les droits root (super utilisateur).
Passer en mode super utilisateur (root) si nécessaire avec la commande suivante :
```sh
su root
```

## Attention (Correspondance QWERTY pendant l’édition)

Tant que Weston n'a pas redémarré, le clavier reste en QWERTY.
tableau de correpsondance :
| Dans l'OS en QWERTY | Sur le clavier AZERTY |
|--------|---------|
| `w`    | `z`     |
| `a`    | `q`     |
| `m`    | `,`     |
| `:`    | MAJ + M |

## Étapes

### 1) Ouvrir la config Weston

```sh
vi /etc/xdg/weston/weston.ini
```

### 2) Modifier la section clavier

Dans le fichier, repèrer :

```ini
[keyboard]
keymap_layout=us
```

Remplacer `us` par `fr` :

```ini
[keyboard]
keymap_layout=fr
```
Rappel `vi` :
- `i` : insérer/éditer
- `r` : remplacer un caractère

### 3) Sauvegarder et quitter `vi`

Rappel `vi` :
- `Échap` : sortir du mode édition
- `:x` : sauver + quitter

### 4) Appliquer les changements

```sh
systemctl restart weston
```