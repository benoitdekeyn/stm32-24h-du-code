#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

/* IMPORTANT : A lancer en ADMINISTRATEUR */

#define MAX_ADAPTERS 20
#define BUFFER_SIZE 512
#define STM32_DEFAULT_IP "192.168.1.50"

// Codes couleurs pour la console Windows (fond noir)
#define COLOR_RESET      7   // Blanc/Gris par défaut
#define COLOR_INFO       11  // Cyan - Informations
#define COLOR_SUCCESS    10  // Vert - Succès
#define COLOR_WARNING    14  // Jaune - Avertissements
#define COLOR_ERROR      12  // Rouge - Erreurs
#define COLOR_CHOICE     13  // Magenta - Menus/Sélections
#define COLOR_TITLE      15  // Blanc brillant - Titres

typedef struct {
    char name[256];
    char description[256];
} Adapter;

// Définit la couleur du texte dans la console
void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void pause_exit() {
    set_color(COLOR_RESET);
    printf("\nAppuyez sur Entrée pour fermer...");
    getchar();
}

int run_command(const char *cmd) {
    return system(cmd);
}

// Vérifie si une interface spécifique a l'accès Internet (IPv4)
int check_internet_access(const char *interfaceName) {
    char cmd[512];
    
    // On demande à PowerShell le statut de connectivité IPv4 pour cette interface précise
    // On filtre avec findstr pour voir si le mot "Internet" apparaît dans le résultat.
    // >nul cache la sortie texte.
    snprintf(cmd, sizeof(cmd), "powershell -Command \"Get-NetConnectionProfile -InterfaceAlias '%s' | findstr Internet\" >nul 2>&1", interfaceName);
    
    // si system() renvoie 0, c'est que findstr a trouvé le mot "Internet"
    if (system(cmd) == 0) {
        return 1; // VRAI : Connecté au web
    }
    return 0; // FAUX : Pas d'internet (ou LocalNetwork uniquement)
}

// Vérifie si le Ping a vraiment reçu une réponse (TTL)
int check_ping_robust(const char *stm32IP) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ping %s -n 2 | find \"TTL=\"", stm32IP);
    int pingResult = system(cmd);

    if (pingResult == 0) {
        // Si on entre ici, c'est que "TTL=" a été trouvé dans la réponse.
        // C'est la preuve absolue que la connexion est bonne.
        return 1; // Succès
    } else {
        // Si on est ici, c'est soit "Délai dépassé", soit "Unreachable".
        // Dans les deux cas, find a renvoyé 1 car il n'a pas vu "TTL".
        
        return 0; // Échec
    }
}

// Détecte et classifie toutes les interfaces réseau actives
// Retourne le nombre total d'interfaces disponibles (sans internet)
int detect_interfaces(Adapter *ethernetUSB, int *usbCount, 
                     Adapter *ethernet, int *ethCount, 
                     Adapter *wifi, int *wifiCount) {
    FILE *fp;
    char line[BUFFER_SIZE];
    int interfaceCount = 0;
    int hasInternet[MAX_ADAPTERS] = {0};
    
    // Pour l'affichage des interfaces détectées
    Adapter allPhysicalInterfaces[MAX_ADAPTERS];
    int allPhysicalCount = 0;
    
    *usbCount = 0;
    *ethCount = 0;
    *wifiCount = 0;
    
    set_color(COLOR_INFO);
    printf("Recherche des interfaces réseau par priorité...\n");
    set_color(COLOR_RESET);
    
    // Récupération de toutes les interfaces actives
    fp = _popen("powershell -Command \"Get-NetAdapter | Where-Object Status -eq 'Up' | ForEach-Object { $_.Name + '|' + $_.InterfaceDescription }\"", "r");
    
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp) != NULL && interfaceCount < MAX_ADAPTERS) {
            char *token = strtok(line, "|");
            if (token != NULL) {
                char name[256];
                strcpy(name, token);
                token = strtok(NULL, "\r\n");
                char description[256] = "Inconnu";
                if (token != NULL) strcpy(description, token);
                
                // Conversion en minuscules pour comparaison
                char nameLower[256];
                strcpy(nameLower, name);
                for (int i = 0; nameLower[i]; i++) nameLower[i] = tolower(nameLower[i]);
                
                char descLower[256];
                strcpy(descLower, description);
                for (int i = 0; descLower[i]; i++) descLower[i] = tolower(descLower[i]);
                
                // Détection des interfaces virtuelles (à ignorer)
                int isVirtual = (strstr(descLower, "virtualbox") != NULL || 
                               strstr(descLower, "vmware") != NULL ||
                               strstr(descLower, "hyper-v") != NULL ||
                               strstr(nameLower, "vethernet") != NULL);
                
                if (isVirtual) {
                    interfaceCount++;
                    continue; // Ignorer les interfaces virtuelles
                }
                
                // Stocker l'interface physique pour l'affichage
                strcpy(allPhysicalInterfaces[allPhysicalCount].name, name);
                strcpy(allPhysicalInterfaces[allPhysicalCount].description, description);
                
                // Vérifier si l'interface a une connexion internet
                hasInternet[allPhysicalCount] = check_internet_access(name);
                
                // Détection du type dans le NOM
                int isEthernetName = (strstr(nameLower, "ethernet") != NULL || 
                                    strstr(nameLower, "eth") != NULL);
                int isWifiName = (strstr(nameLower, "wi-fi") != NULL || 
                                strstr(nameLower, "wifi") != NULL);
                
                // Détection USB dans la DESCRIPTION
                int isUSB = (strstr(descLower, "usb") != NULL);
                
                // Classement par priorité (seulement si PAS de connexion internet)
                if (!hasInternet[allPhysicalCount]) {
                    if (isEthernetName && isUSB) {
                        // PRIORITE 1 : Ethernet USB sans internet
                        strcpy(ethernetUSB[*usbCount].name, name);
                        strcpy(ethernetUSB[*usbCount].description, description);
                        (*usbCount)++;
                    } else if (isEthernetName && !isUSB) {
                        // PRIORITE 2 : Ethernet classique sans internet
                        strcpy(ethernet[*ethCount].name, name);
                        strcpy(ethernet[*ethCount].description, description);
                        (*ethCount)++;
                    } else if (isWifiName) {
                        // PRIORITE 3 : WiFi sans internet
                        strcpy(wifi[*wifiCount].name, name);
                        strcpy(wifi[*wifiCount].description, description);
                        (*wifiCount)++;
                    }
                }
                
                allPhysicalCount++;
                interfaceCount++;
            }
        }
        _pclose(fp);
    }
    
    // Afficher toutes les interfaces physiques détectées
    if (allPhysicalCount > 0) {
        set_color(COLOR_TITLE);
        printf("\n=== INTERFACES PHYSIQUES ACTIVES DÉTECTÉES ===\n");
        set_color(COLOR_RESET);
        printf("%-25s %-50s %-15s\n", "Nom", "Description", "Internet");
        printf("%-25s %-50s %-15s\n", "-------------------------", "--------------------------------------------------", "---------------");
        
        for (int i = 0; i < allPhysicalCount; i++) {
            char internetStatus[20];
            if (hasInternet[i]) {
                strcpy(internetStatus, "[CONNECTÉ]");
            } else {
                strcpy(internetStatus, "[HORS LIGNE]");
            }
            
            // Tronquer la description si trop longue
            char shortDesc[51];
            strncpy(shortDesc, allPhysicalInterfaces[i].description, 50);
            shortDesc[50] = '\0';
            
            printf("%-25s %-50s %-15s\n", 
                   allPhysicalInterfaces[i].name, 
                   shortDesc, 
                   internetStatus);
        }
        printf("\n");
    }
    
    // Vérifier si toutes les interfaces ont internet
    int totalInterfaces = *usbCount + *ethCount + *wifiCount;
    
    if (totalInterfaces == 0) {
        int hasInterfaceWithInternet = 0;
        for (int i = 0; i < allPhysicalCount; i++) {
            if (hasInternet[i]) {
                hasInterfaceWithInternet = 1;
                break;
            }
        }
        
        if (hasInterfaceWithInternet) {
            set_color(COLOR_WARNING);
            printf("Toutes les interfaces détectées sont connectées à internet !\n");
            printf("Pour établir une connexion directe avec la STM32 :\n");
            printf("  1. Débranchez le câble Ethernet de votre box/routeur\n");
            printf("  2. Branchez-le directement sur la STM32\n");
            printf("  3. OU désactivez temporairement la passerelle par défaut\n");
            set_color(COLOR_RESET);
        } else {
            set_color(COLOR_ERROR);
            printf("\nAucune interface réseau active détectée !\n");
            printf("Vérifiez vos connexions réseau et réessayez.\n");
            set_color(COLOR_RESET);
        }
    }
    
    return totalInterfaces;
}

// Sélectionne l'interface selon la priorité (auto ou manuel)
// Retourne 1 si une interface a été sélectionnée, 0 pour réessayer
int select_interface(Adapter *ethernetUSB, int usbCount,
                     Adapter *ethernet, int ethCount,
                     Adapter *wifi, int wifiCount,
                     char *selectedInterface) {
    Adapter *selectedCategory = NULL;
    int selectedCount = 0;
    char categoryName[50] = "";
    
    // Déterminer la catégorie prioritaire disponible
    if (usbCount > 0) {
        selectedCategory = ethernetUSB;
        selectedCount = usbCount;
        strcpy(categoryName, "ETHERNET USB");
    } else if (ethCount > 0) {
        selectedCategory = ethernet;
        selectedCount = ethCount;
        strcpy(categoryName, "ETHERNET");
    } else if (wifiCount > 0) {
        selectedCategory = wifi;
        selectedCount = wifiCount;
        strcpy(categoryName, "WIFI");
    } else {
        return 0; // Aucune interface disponible
    }
    
    // Si une seule interface dans la catégorie prioritaire : sélection auto
    if (selectedCount == 1) {
        strcpy(selectedInterface, selectedCategory[0].name);
        set_color(COLOR_SUCCESS);
        printf("Interface sélectionnée (%s) : %s\n", categoryName, selectedInterface);
        set_color(COLOR_RESET);
        Sleep(1000);
        return 1;
    }
    
    // Plusieurs interfaces : demander le choix
    set_color(COLOR_TITLE);
    printf("\n=== SELECTION INTERFACE %s ===\n", categoryName);
    set_color(COLOR_RESET);
    printf("%-4s %-25s %s\n", "N°", "Nom", "Description");
    printf("%-4s %-25s %s\n", "---", "-------------------------", "-----------------------------------------------------");
    
    for (int i = 0; i < selectedCount; i++) {
        printf("%-4d %-25s %s\n", i + 1, selectedCategory[i].name, selectedCategory[i].description);
    }
    
    set_color(COLOR_CHOICE);
    printf("\n[R] Réessayer la détection\n");
    printf("\nVotre choix : ");
    set_color(COLOR_RESET);
    
    char choiceBuf[10];
    fgets(choiceBuf, sizeof(choiceBuf), stdin);
    
    if (choiceBuf[0] == 'r' || choiceBuf[0] == 'R') {
        return 0; // Réessayer
    }
    
    int choice = atoi(choiceBuf);
    if (choice >= 1 && choice <= selectedCount) {
        strcpy(selectedInterface, selectedCategory[choice - 1].name);
        return 1;
    }
    
    set_color(COLOR_ERROR);
    printf("Choix invalide.\n");
    set_color(COLOR_RESET);
    Sleep(2000);
    return 0;
}

// Configure une IP disponible sur l'interface
// Retourne l'IP configurée, ou 0 en cas d'échec
int configure_ip(const char *interfaceName, int stm32HostID) {
    char cmd[BUFFER_SIZE];
    int hostID = stm32HostID + 1; // On commence juste après l'IP de la STM32
    
    set_color(COLOR_INFO);
    printf("\nTentative de configuration sur : %s\n", interfaceName);
    set_color(COLOR_RESET);
    
    // On commence juste après l'IP de la STM32 et on va jusqu'à .254 max
    while (hostID <= 254) {
        // SECURITE : On ne prend surtout pas l'IP de la STM32 !
        if (hostID == stm32HostID) {
            hostID++;
            continue;
        }
        
        printf("   -> Tentative avec l'IP 192.168.1.%d ... ", hostID);
        
        // On essaie de configurer cette IP
        snprintf(cmd, sizeof(cmd), "netsh interface ip set address \"%s\" static 192.168.1.%d 255.255.255.0 >nul 2>&1", 
                interfaceName, hostID);
        int result = system(cmd);
        
        if (result == 0) {
            set_color(COLOR_SUCCESS);
            printf("Acceptée !\n");
            set_color(COLOR_RESET);
            return hostID; // Succès
        } else {
            set_color(COLOR_WARNING);
            printf("(Objet existe déjà)\n");
            set_color(COLOR_RESET);
            hostID++;
        }
    }
    
    set_color(COLOR_ERROR);
    printf("\nImpossible de trouver une IP libre.\n");
    printf("Il y a un problème grave avec Windows.\n");
    set_color(COLOR_RESET);
    return 0; // Échec
}

// Teste la connexion avec la STM32
int test_connection(const char *stm32IP) {
    set_color(COLOR_INFO);
    printf("\nTentative de Ping vers la STM32 (%s)...\n", stm32IP);
    set_color(COLOR_RESET);
    Sleep(1000); // On laisse une seconde à l'interface pour monter
    return check_ping_robust(stm32IP);
}

// Extrait le dernier octet d'une adresse IP
int extract_last_octet(const char *ip) {
    int a, b, c, d;
    if (sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) == 4) {
        return d;
    }
    return -1;
}

// Valide et demande l'adresse IP de la STM32
int configure_stm32_ip(char *stm32IP) {
    char choice[10];
    
    set_color(COLOR_CHOICE);
    printf("L'adresse testée pour la STM32 est %s\n", stm32IP);
    set_color(COLOR_RESET);
    printf("Entrez y/n pour valider/modifier l'adresse... ");
    
    fgets(choice, sizeof(choice), stdin);
    
    if (choice[0] == 'n' || choice[0] == 'N') {
        while (1) {
            set_color(COLOR_INFO);
            printf("\nEntrez le nouveau numéro d'hôte X (où X est entre 1 et 254) : ");
            set_color(COLOR_RESET);
            
            char newIP[20] = "192.168.1.";
            fgets(newIP + strlen(newIP), sizeof(newIP) - strlen(newIP), stdin);
            newIP[strcspn(newIP, "\n")] = 0; // Enlever le \n
            
            // Validation du format
            int a, b, c, d;
            if (sscanf(newIP, "%d.%d.%d.%d", &a, &b, &c, &d) == 4) {
                if (a == 192 && b == 168 && c == 1 && d >= 1 && d <= 254) {
                    strcpy(stm32IP, newIP);
                    set_color(COLOR_CHOICE);
                    printf("L'adresse testée pour la STM32 est %s\n", stm32IP);
                    set_color(COLOR_RESET);
                    Sleep(1000);
                    return extract_last_octet(stm32IP);
                }
            }
            
            set_color(COLOR_ERROR);
            printf("Adresse invalide. Format requis : 192.168.1.X (X entre 1 et 254)\n");
            set_color(COLOR_RESET);
        }
    }    
    return extract_last_octet(stm32IP);
}

int main() {
    // Configuration de l'encodage pour afficher correctement les caractères français
    SetConsoleOutputCP(65001); // UTF-8
    setlocale(LC_ALL, "fr_FR.UTF-8");
    
    char currentInterface[256] = {0};
    char stm32IP[20];
    strcpy(stm32IP, STM32_DEFAULT_IP);
    int stm32HostID = 0;
    int configSuccess = 0;
    int firstRun = 1;
    
    Adapter ethernetUSB[MAX_ADAPTERS];
    Adapter ethernet[MAX_ADAPTERS];
    Adapter wifi[MAX_ADAPTERS];
    int usbCount = 0, ethCount = 0, wifiCount = 0;

    while (!configSuccess) {
        system("cls");
        set_color(COLOR_TITLE);
        printf("--- CONFIGURATION LIEN DIRECT STM32 ---\n\n");
        set_color(COLOR_RESET);
        
        // Configuration de l'adresse IP de la STM32 (une seule fois)
        if (firstRun) {
            stm32HostID = configure_stm32_ip(stm32IP);
            if (stm32HostID < 0) {
                printf("Erreur lors de la configuration de l'adresse IP.\n");
                pause_exit();
                return 1;
            }
            printf("\n");
            firstRun = 0;
        }

        // ---------------------------------------------------------
        // 1. DETECTION ET SELECTION D'INTERFACE
        // ---------------------------------------------------------
        if (strlen(currentInterface) == 0) {
            // Détecter toutes les interfaces réseau
            int totalInterfaces = detect_interfaces(ethernetUSB, &usbCount, 
                                                   ethernet, &ethCount, 
                                                   wifi, &wifiCount);
            
            if (totalInterfaces == 0) {
                printf("\nAppuyez sur Entrée pour réessayer...");
                getchar();
                continue;
            }
            
            // Sélectionner une interface selon la priorité
            if (!select_interface(ethernetUSB, usbCount, 
                                 ethernet, ethCount, 
                                 wifi, wifiCount, 
                                 currentInterface)) {
                continue; // Réessayer
            }
        }

        // ---------------------------------------------------------
        // 2. CONFIGURATION IP
        // ---------------------------------------------------------
        int hostID = configure_ip(currentInterface, stm32HostID);
        
        if (hostID == 0) {
            pause_exit();
            return 1; // Erreur critique
        }

        // ---------------------------------------------------------
        // 3. TEST DE CONNEXION
        // ---------------------------------------------------------
        int isConnected = test_connection(stm32IP);

        if (isConnected) {
            set_color(COLOR_SUCCESS);
            printf("\nConnexion établie !\n");
            set_color(COLOR_CHOICE);
            printf("\nVotre PC utilise l'IP : 192.168.1.%d\n", hostID);
            set_color(COLOR_RESET);
            configSuccess = 1;
        } else {
            set_color(COLOR_ERROR);
            printf("\nL'IP est configurée mais la STM32 ne répond pas.\n");
            set_color(COLOR_WARNING);
            printf(" -> Vérifiez que la STM32 est allumée et branchée.\n");
            printf("\nAppuyez sur Entrée pour redémarrer le programme...\n");
            set_color(COLOR_RESET);
            
            getchar();
            currentInterface[0] = 0;
        }
    }

    printf("\nProgramme terminé avec succès.\n");
    pause_exit();
    return 0;
}