# Configurer les broches GPIO dans STM32CubeMX

## Caractéristiques
- Langage du projet : C
- Carte électronique : STM32MP157X-DK2
- Logiciel utilisé : CubeMX de ST
- Objectif :
	- Définir le rôles des broches à utiliser dans un projet Hardware.
	- Générer la base du projet en C qui sera ensuite codé sur un IDE.

## Prérequis

- CubeMX installé [Cf. Tuto - Installer CubeMX](./10-installer-cubemx.md)
- Carte reliée et fonctionnelle [Cf. Tuto - Connexion STM32 au PC et WSL](./16-connexion-stm32-windows-wsl.md)
- Connaître l'identifiant des broches utilisées dans le projet (ex : PH7, PE12, ...) [CF. Manuel Utilisateur](../Ressources/User-Manual-STM32MP157C-DK2.pdf)

## 3 types de configuration GPIO

Chaque broche peut être configurée en différents modes selon l'usage :
- `GPIO_Output` : pour piloter des LED, relais, etc.
- `GPIO_Input` : pour lire l'état d'un capteur ou bouton en boucle.
- `GPIO_EXTI` : pour appeler une fonction instantanément lorsque la tension du signal change[^1].
- beaucoup d'autres modes non abordés dans ce tutoriel

---

## Étape 1) Créer un projet dans CubeMX

1. Lancer CubeMX.
2. Cliquer sur le bouton `ACCESS TO BOARD SELECTOR`.
3. Dans la barre de recherhe `Commercial Part Number`, taper `DK2`.
4. Sélectionner la carte qui se rapproche le plus de la `STM32MP157C-DK2`[^2].
5. Dans la zone des résultats, sélectionner la carte correspondante.
6. Cliquer sur le bouton `Start Project`.
7. Au message `Initialize all peripherals with their default Mode ?`, choisir `Yes`.

## ÉTAPE 2) Paramétrer le projet

1. Aller dans l'onglet `Project Manager > Project`.
2. Donner un nom au projet (Un dossier sera créé avec ce nom et contiendra tous les fichiers).
3. Sélectionner l'emplacement de ce dossier.
4. Dans `Toolchain/IDE`, sélectionner l'outils pour lequel le projet sera construit (ex: STM32 IDE).

## Étape 3) Activer les broches utilisées

1. Aller dans l'onglet ouvert par défaut `Pinout view` avec la carte visuelle des broches.
2. Identifier la broche à configurer (ex : `PH6`) grâce à la barre de recherche.
3. Double-cliquer sur la broche représentée par une pastille.
4. Choisir la fonction souhaitée (ex : `GPIO_Output`).

PS : La pastille devient verte pour montrer que la broche est activée. L'état désactivé est représenté en gris, et correspond au `Reset_State`.

---

## Étape 4) Configurer les GPIO

1. Aller dans `Pinout & Configuration > Categories > System Core > GPIO`.
2. Dans la fenêtre qui s'est ouverte, sélectionner l'onglet `GPIO`. 
3. Sélectionner la ligne de la broche à configurer (seules les broches actives apparaissent).
4. Rennommer la broche avec un `User Label` (ex : `LED_ROUGE`) pour générer des symboles lisibles dans le code. (optionnel) 
5. Assigner la broche au `Cortex-M4 FW` dans `Pin Context Assignment` (explications[^3]).
6. Puis régler les autres paramètres selon sa fonction

### Pour un GPIO Output

Exemple : LED rouge sur `PH6`.

- `GPIO output level` : `Low` (LED éteinte au démarrage)
- `GPIO mode` : `Output Push Pull`
- `GPIO Pull-up/Pull-down` : `No pull-up and no pull-down` (cas standard LED)

### Pour un GPIO Input

Exemple : bouton sur `PH7`.

- `GPIO mode` : `Input`
- `GPIO Pull-up/Pull-down` : Selon le câblage électrique de l'entrée (aide[^4]).

### Pour un EXTI

Exemple : bouton utilisateur sur `PA1`.

- `GPIO mode` : Un des 3 `External Interrupt Mode` selon la détection souhaitée (aide[^5]).
- `GPIO Pull-up/Pull-down` : selon le câblage électrique de l'entrée (aide[^4]).

Pour les broches EXTI, il faut également activer l'interruption :  

1. Aller dans `System Core > Configurations > NVIC`.
2. Dans la section ouverte, sélectionner l'onglet `NVIC`(ouverte par défaut).
2. À la ligne d'interruption `EXTI line X interrupt` , cocher la case de la colonne `Enable`.

---

## Construction du projet.

1. Cliquer sur `GENERATE CODE`.
2. Si un WARNINGS demande :
	```
	The following pins are free and must be assigned to a context: PA6,PA10,PA13,PA14,... 
	Do you still want to generate code ?
	```
	Accepter du moment que toutes les broches utilisés sont bien assignées au Cortex-M4.  

3. Choisir l'action à éxécuter à la fin de la génération (par défaut : Close).

## Notes

[^1]: L'avantage des GPIO_EXTI est de ne pas avoir à surveiller en permanence l'état d'une broche dans une boucle infinie. Le microcontrôleur peut rester en veille et ne se réveiller que lorsqu'un événement se produit sur la broche configurée, permettant ainsi une gestion plus efficace de l'énergie et des ressources. 
	Il existe uniquement 16 lignes d'interruption EXTI disponibles, partagées entre les broches. Il faut donc les utiliser judicieusement.

[^2]: La carte `STM32MP157C-DK2` n'est plus référencée dans CubeMX, au profit de sa grande sœur la `STM32MP157F-DK` qui peut être utilisée sans problème pour la configuration des broches.

[^3]: Sur STM32MP1 (architecture hybride Cortex-A7 + Cortex-M4), il faut indiquer à quel cœur appartient la broche, sinon il peut y avoir des conflits d'accès entre les deux cœurs.

[^4]: Pour un bouton un capteur passif (ex: bouton), il est essentiel d'ajouter une résistance de tirage :
	- `Pull-up` si la broche est ramenée à `GND` quand on appuie.
	- `Pull-down` si la broche est ramenée à `VCC` quand on appuie.
	- `No pull` si une résistance externe est déjà présente

[^5]: Les 3 modes de détection proposés pour un `External Interrupt` sont :
	- `Rising edge` Déclenche l'interruption si la broche passe de `GND` à `VCC`.
	- `Falling edge` Déclenche l'interruption si la broche passe de `VCC` à `GND`.
	- `Rising/Falling edge` Déclenche l'interruption dans les 2 cas ci-dessus.