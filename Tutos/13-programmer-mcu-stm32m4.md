# Programmer le MCU (Cortex-M4) de la STM32MP1

Objectif :
- Créer un projet CubeMX et le configurer pour le Cortex-M4.
- Générer le code d'initialisation via CubeMX.
- Importer le projet dans CubeIDE.
- Écrire et compiler un programme C pour le Cortex-M4.
- Gérer les interruptions (EXTI) et déboguer les problèmes courants.

## Prérequis

Debian/Ubuntu (ou WSL sur Windows)

- CubeMX installé (voir [Tuto - Installer CubeMX](./10-installer-cubemx.md))
- CubeIDE installé (voir [Tuto - Installer CubeIDE](./11-installer-cubeide.md))
- Connexion établie avec la carte (voir [Tuto - Connexion carte](./16-connexion-stm32-windows-wsl.md))
- (optionnel) SDK STM32MP1 installé (voir [Tuto - Compiler avec SDK](./06-compiler-c-sdk.md))
---

## Étape 1) Créer un projet CubeMX

Pour programmer le Cortex-M4, il faut d'abord configurer les broches qu'il utilisera.

Voir le tutoriel détaillé : [Tuto 09 - Configurer GPIO CubeMX](./09-configurer-gpio-mx.md)

### Résumé rapide pour un exemple (Bouton + LED RGB)

1. **Configurer les sorties** (LEDs sur `PH6`, `PE11`, `PE14`) :
   - Mode : `GPIO_Output`
   - Level : `Low` (éteint au démarrage)
   - Mode : `Output Push Pull`
   - User Label : `LED_ROUGE`, `LED_VERTE`, `LED_BLEUE`

2. **Configurer l'entrée** (Bouton sur `PA1`) :
   - Mode : `GPIO_EXTI1`
   - Front : `Rising edge trigger detection` (ou `Falling edge` selon votre montage)
   - Pull-up : selon votre câblage
   - User Label : `BOUTON_USER`

3. **Activer l'interruption** :
   - Aller dans `System Core > NVIC`.
   - Cocher `EXTI line 1 interrupt` (dépend de la broche choisie).

---

## Étape 3) Importer le projet dans CubeIDE

### 3.1) Ouvrir CubeIDE

```bash
cubeIDE
```

Si la commande `cubeIDE` n'est pas reconnue, aller voir le [Tuto - Installer CubeIDE](./11-installer-cubeide.md)

### 3.2) Créer un nouvel espace de travail

1. À l'ouverture, sélectionner un dossier pour l'espace de travail (workspace).
2. Cliquer sur `Launch`.

### 3.3) Importer le projet CubeMX

1. `File > Import`
2. Sélectionner `Import STM32 Project > STM32CubeMX1/STM32CubeIDE Project`.
3. Cliquer `Next`.
4. Dans `Import source` : parcourir jusqu'au dossier généré par CubeMX contenant le `.ioc`.
5. L'interface va scanner tout le dossier et détecter les projets disponibles (Global, CA7, CM4). Vérifier que les 3 projets apparaissent.
6. Cliquer sur `Finish`.

CubeIDE détecte automatiquement l'architecture et crée les configurations de build.

---

## Étape 4) Écrire le code du MCU

### 4.1) Ouvrir main.c du Cortex-M4

Dans l'explorateur de projets (gauche) :
- Développer le projet → `CM4` → `Core` → `Src` → `main.c`

### 4.2) Ajouter le code utilisateur

Le code doit être inséré entre les balises `/* USER CODE BEGIN */` et `/* USER CODE END */`. Il y a 5 balises pour le code utilisateur. Toutes ne sont pas nécessairement utilisées.

#### Exemple : Bouton avec debounce contrôlant une LED RGB

**Balise `USER CODE BEGIN 0` (avant le main):**

```c
typedef enum { OFF = 0, RED = 1, GREEN = 2, BLUE = 3 } ColorState;

ColorState selector = RED;
uint32_t last_interrupt_time = 0;  // Timestamp pour le debounce
```

**Balise `USER CODE BEGIN 4` (callback d'interruption):**

```c
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    // Vérifier que c'est bien le bouton qui a déclenché l'interruption
    if(GPIO_Pin == BOUTON_USER_Pin)
    {
        // Filtre debounce : ignorer les interruptions < 200ms après la précédente
        uint32_t interrupt_time = HAL_GetTick();
        if ((interrupt_time - last_interrupt_time) > 200)
        {
            // Changer la couleur
            selector = (ColorState)((selector + 1) % 4);

            // Éteindre toutes les LEDs d'abord
            HAL_GPIO_WritePin(LED_ROUGE_GPIO_Port, LED_ROUGE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_VERTE_GPIO_Port, LED_VERTE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_BLEUE_GPIO_Port, LED_BLEUE_Pin, GPIO_PIN_RESET);

            // Allumer la bonne LED selon la couleur sélectionnée
            switch(selector)
            {
                case RED:
                    HAL_GPIO_WritePin(LED_ROUGE_GPIO_Port, LED_ROUGE_Pin, GPIO_PIN_SET);
                    break;
                case GREEN:
                    HAL_GPIO_WritePin(LED_VERTE_GPIO_Port, LED_VERTE_Pin, GPIO_PIN_SET);
                    break;
                case BLUE:
                    HAL_GPIO_WritePin(LED_BLEUE_GPIO_Port, LED_BLEUE_Pin, GPIO_PIN_SET);
                    break;
                case OFF:
                default:
                    // Toutes les LEDs sont déjà éteintes
                    break;
            }

            // Mettre à jour le timestamp
            last_interrupt_time = interrupt_time;
        }
    }
}
```

---

## Étape 5) Compiler et déboguer

### 5.1) Compiler le projet

1. Sélectionner le projet CM4 dans l'explorateur 
2. Cliquer sur le marteau dans la barre du haut.
3. Vérifier dans la console qui s'est ouverte si le build indique bien `Build Finished. 0 errors, 0 warnings`

### 5.2) Programmer la carte

1. Connecter la carte via Micro USB (ST-LINK).
2. S'assurer que la connexion à WSL est bien établie (Cf. [Tuto - Rediriger ST-Link](./12-rediriger-stlink-wsl.md))
2. Faire un clic droit sur le projet CM4 → `Debug As > Debug Configurations`.
3. Sélectionner la configuration `STM32 Cortex-M4 Debug`.
4. Cliquer `Debug`.

CubeIDE compile, flash et lance le débogueur.

### 5.3) Ajouter des points d'arrêt

Pour vérifier que votre code fonctionne :

1. Cliquer sur le numéro de ligne où vous voulez un point d'arrêt.
2. Un point rouge apparaît.
3. Appuyer sur le bouton et observer si l'exécution s'arrête au point d'arrêt.

---

## Étape 6) Tester sur la carte

### 6.1) Flash et exécution sans déboguer

Si tout fonctionne en debug, vous pouvez aussi :

1. `Run > Run As > STM32 C/C++ Application` (ou `Ctrl+F11`).
2. La carte exécute le programme en continu.

### 6.2) Vérifier via SSH

Pour voir les logs ou arrêter le programme :

```bash
ssh root@192.168.7.1
# Sur la carte
ps aux | grep stm32
# Arrêter le programme
kill <PID>
```

Voir [Tuto 17 - SSH/SCP](./17-ssh-scp.md) pour plus d'infos.

---

## Problème connu : Conflit EXTI entre Cortex-A7 et Cortex-M4

### Symptôme

L'interruption est détectée (vous entrez dans `HAL_GPIO_EXTI_IRQHandler()`), mais les conditions `__HAL_GPIO_EXTI_GET_RISING_IT()` et `__HAL_GPIO_EXTI_GET_FALLING_IT()` sont toutes les deux fausses.

**Cause** : Le Cortex-A7 (Linux) traite l'interruption en même temps que le Cortex-M4 et efface le flag d'interruption avant que le M4 puisse le lire.

### Workaround temporaire

Modifier `HAL_GPIO_EXTI_IRQHandler()` dans le fichier `stm32mp1xx_hal_gpio.c` (CM4 project) :

```c
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
  // Forcer l'entrée même si le flag est déjà effacé par l'A7
  if (1==1)  // Condition toujours vraie
  {
    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);
    HAL_GPIO_EXTI_Rising_Callback(GPIO_Pin);
  }

  if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_Pin) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_FALLING_IT(GPIO_Pin);
    HAL_GPIO_EXTI_Falling_Callback(GPIO_Pin);
  }
}
```

**⚠️ Attention** : Ce workaround force toujours le callback à s'exécuter. À faire uniquement si vous êtes sûr que cette broche is assignée au M4 uniquement.

### Solution proper : Mettre à jour le Device Tree

La bonne solution est de configurer le kernel Linux (Cortex-A7) pour qu'il n'interfère pas avec les broches du M4.

Voir [Tuto 14 - Mettre à jour Device Tree](./14-mettre-a-jour-device-tree.md) (actuellement bloqué par un bug ST).

---

## Points de repère et conseils

### Utiliser les User Labels pour la lisibilité

Toujours ajouter un `User Label` dans CubeMX pour chaque broche :

```c
// Au lieu de :
HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET);

// Préférez :
HAL_GPIO_WritePin(LED_ROUGE_GPIO_Port, LED_ROUGE_Pin, GPIO_PIN_SET);
```

### Gestion du debounce

Les boutons physiques "rebondissent" (oscillent) pendant quelques millisecondes. Sans debounce, une seule pression déclenche plusieurs interruptions.

Solution simple : vérifier le timestamp (`HAL_GetTick()`) et ignorer les interruptions trop proches (ex : < 200ms).

```c
uint32_t last_interrupt_time = 0;

if ((interrupt_time - last_interrupt_time) > 200)
{
    // Traiter l'interruption
    last_interrupt_time = interrupt_time;
}
```

### Vérifier l'assignation au M4

Toujours faire un clic droit → `Pin Context Assignment` pour chaque broche :

```
☑ Cortex-M4
```

Sans cette étape, la broche appartient par défaut au Cortex-A7.

### Déboguer avec des breakpoints

Utiliser les breakpoints pour vérifier :

1. Que le callback est appelé.
2. Que les variables ont les bonnes valeurs.
3. Que HAL_GPIO_WritePin() executa bien.

---

## Résumé du flux global

1. **CubeMX** : Créer le projet, configurer les GPIOs, générer le code.
2. **CubeIDE** : Importer le projet, écrire le code C dans les balises USER CODE.
3. **Compiler/Debug** : Tester sur la carte, ajouter des breakpoints si nécessaire.
4. **Dépannage** : Si conflit EXTI, appliquer le workaround ou mettre à jour le Device Tree.

---

## Fichiers à consulter

- `CM4/Core/Src/main.c` : Code principal du M4.
- `CM4/Core/Inc/main.h` : Définitions générées par CubeMX (pins, etc.).
- `CM4/Drivers/STM32MP1xx_HAL_Driver/stm32mp1xx_hal_gpio.c` : Driver HAL (où modifier en cas de besoin).
- `CA7/manifest.prop` : Version du Device Tree (pour la solution proper).

---

## Dépannage

### Erreur de compilation

Vérifier que le projet CM4 est bien construit (pas le CA7 ou le projet global).

### Le code ne s'exécute pas après flash

1. Vérifier que le Cortex-M4 est présent et activé dans CubeMX.
2. Vérifier que le code a bien été flashé (voir les logs de CubeIDE).
3. Tester avec un breakpoint simple dans le `while(1)` du main.

### Interruption ne se déclenche pas

1. Vérifier que NVIC est activé pour la ligne d'interruption.
2. Vérifier que `Pin Context Assignment` inclut `Cortex-M4`.
3. Si tout est OK, appliquer le workaround du conflit EXTI (voir section 6).

### Comportement erratique (interruptions multiples)

Ajouter un debounce pour filtrer les rebonds du bouton (voir "Points de repère").
