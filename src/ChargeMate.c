// INCLUSION LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

// INCLUSION FONCTIONS
#include "en_tete.h"
#include "simu.h"
#include "itinerary.h"

// FONCTIONS UTILES À L'APPLICATION
void viderBuffer(){
    int c=0;
    while (c != '\n' && c != EOF){
        c = getchar();
    }
}

// CORPS PRINCIPAL DE L'APPLICATION :
int main(void){
    int reponse = 0;
    int taille_simulation;

    printf("\n\n                          Bienvenue sur ChargeMate !\n\n");

    while(reponse != 4){
        if(reponse == 0){
            printf("\nChoisissez une option: \n\n");
            printf("    1 : Faire une simulation de votre trajet.\n");
            printf("    2 : Afficher le rendu d'une simulation (⚠ ne pas rentrer un nombre trop grand ⚠ ).\n");
            printf("    3 : Evaluer la charge du réseau pour un nombre d'entité choisi.\n");
            printf("    4 : Quitter l'application.\n");
            printf("\nVotre choix ? : ");
            scanf("%i", &reponse);
            printf("\n");
        }else if(reponse == 1){
            printf("\n                  Simulation d'un trajet\n\n");
            printf("Chargement des villes et voitures...\n\n");
            itinerary();
            printf("\n\nRetour au menu principal...\n\n");
            reponse = 0;
        }else if(reponse == 2){
            printf("Entrez le nombre d'entités voulu dans la simulation : ");
            scanf("%i", &taille_simulation);
            AutoPath** AutoPath = initAutoPath_simu(taille_simulation);
            clock_t depart = clock();
            carre** tab_of_station_by_square = travelAutopath_simu(AutoPath, taille_simulation);
            clock_t fin = clock();
            long int time = (fin - depart) / CLOCKS_PER_SEC;
            print_AutoPath_simu(AutoPath, taille_simulation);
            destroy_tab_carre_simu(tab_of_station_by_square);
            destroy_AutoPath_simu(AutoPath, taille_simulation);
            printf("\nTemps de chagement : %ldmin %lds\n", time/60, time-((time/60)*60));
            printf("\n\nRetour au menu principal...\n\n");
            reponse = 0;
        }else if (reponse == 3){
            printf("Entrez le nombre d'entités voulu dans la simulation : ");
            scanf("%i", &taille_simulation);
            AutoPath** AutoPath = initAutoPath_simu(taille_simulation);
            clock_t depart = clock();
            carre** tab_of_station_by_square = travelAutopath_simu(AutoPath, taille_simulation); // On récupère le tableau de station
            clock_t fin = clock();
            long int time = (fin - depart) / CLOCKS_PER_SEC;
            printf("\nTemps de chagement : %ldmin %lds\n", time/60, time-((time/60)*60));
            printf("\n");
            viderBuffer();

            int duree_simulation;
            printf("\nEntrez la durée (en heure) pendant laquelle vous souhaitez voir l'état du réseau (durée max acceptée : %dh. Attention : pour rappel, nous allons afficher l'état des stations françaises toutes les 10 minutes ! Ne mettez pas une durée de simulation trop grande !) : ", NB_HEURES_SIMULATION);
            scanf("%d", &duree_simulation);
            if(duree_simulation > NB_HEURES_SIMULATION){
                duree_simulation = NB_HEURES_SIMULATION;
                printf("Vous avez rentré une durée de simulation trop grande ! On la réduit à %dh !\n", NB_HEURES_SIMULATION);
            }
            printf("Durée simulation : %dh.\n", duree_simulation);

            int debut_simulation;
            printf("\nEntrez l'heure de début de la simulation (entre 0 et %dh) : ", (TAILLE_TAB_ETAPES-2)/6 - NB_HEURES_SIMULATION);
            scanf("%d", &debut_simulation);
            if(debut_simulation < 0 || debut_simulation > (TAILLE_TAB_ETAPES-2)/6 - NB_HEURES_SIMULATION){
                debut_simulation = 0;
                printf("Vous avez rentré une heure de début de simulation invalide ! On la met à 0 !\n");
            }
            printf("Heure de début de simulation : %dh.\n", debut_simulation);

            duree_simulation *= 6; // On multiplie par 6 car on veut afficher l'état du réseau toutes les 10 minutes (donc toutes les 6 itérations)
            debut_simulation *= 6; // On multiplie par 6 car on veut afficher l'étape correspondante à l'heure de début de simulation

            for(int i = 0 ; i < NB_CARRES ; i++){
                if(tab_of_station_by_square[i]->nb_stations > 0){
                    print_etat_station_simu(tab_of_station_by_square[i]->head, duree_simulation, debut_simulation);
                } 
            }

            destroy_tab_carre_simu(tab_of_station_by_square);
            destroy_AutoPath_simu(AutoPath, taille_simulation);
            printf("\n\nRetour au menu principal...\n\n");
            reponse = 0;
            printf("\n");
        }else if(reponse < 0 || reponse > 4){
            printf("Veuillez rentrer un choix valide !\n");
            reponse = 0;
            printf("Appuyez sur entrée pour revenir au menu !\n");
        }
        viderBuffer();
    }
}
