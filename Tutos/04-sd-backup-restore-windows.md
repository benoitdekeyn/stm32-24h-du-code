# Sauvegarder / restaurer une carte SD sous Windows (image .img)

Objectif :
- Créer une image `.img` depuis une carte SD.
- Restaurer une carte SD depuis une image.

## Attention

- Vérifie **deux fois** le disque ciblé : une erreur peut écraser un autre disque.
- Ferme les explorateurs/fenêtres qui accèdent à la carte SD.

## 1) Identifier le numéro de disque (DiskPart)

Dans PowerShell :

```powershell
diskpart
```

Puis :

```text
list disk
```

Repère le disque dont la taille correspond à la carte SD (ex : 16 Go ⇒ `Disk 1`).

Sortir :

```text
exit
```

## 2) Sauvegarder : créer un fichier image `.img`

### Option A (comme dans les notes) : `dd` pour Windows

Télécharger `dd` :
- http://www.chrysocome.net/downloads/dd-0.5.zip

Mettre `dd.exe` dans :
- `C:\Windows\System32`

Créer l’image (adapte `PhysicalDrive1` et le chemin) :

```powershell
dd if=\\.\PhysicalDrive1 of=C:\Users\<toi>\Backup\backup.img bs=1M --progress
```

Notes :
- Une erreur en fin de copie peut arriver (souvent liée au périphérique) ; si la taille est cohérente, l’image est généralement exploitable.

### Compression recommandée

Le fichier `.img` fait la taille de la carte SD. Compresse-le ensuite (zip) :
- clic droit → Envoyer vers → Dossier compressé

## 3) Restaurer une carte SD à partir d’une image `.img`

Méthode simple : BalenaEtcher.

- Télécharger : https://etcher.balena.io/#download-etcher
- Ouvrir Etcher
  1. **Flash from file** : sélectionner le `.img` (décompressé)
  2. **Select target** : sélectionner la carte SD
  3. **Flash!**
