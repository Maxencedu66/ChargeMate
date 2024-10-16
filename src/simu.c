#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "simu.h"

//---------- FONCTIONS DE CHARGEMENT DES DONNÉES ----------
// CHARGEMENT DES STATIONS
carre** load_stations_simu(){

    //création de la structure
    carre** tab_of_carre = malloc(NB_CARRES*sizeof(carre*));
    for(int i = 0 ; i < NB_CARRES ; i++){
        tab_of_carre[i] = malloc(sizeof(carre));
        tab_of_carre[i]->head = NULL;
        tab_of_carre[i]->nb_stations = 0 ; 
    } 

    //ouverture et lecture du fichier
    FILE* file = fopen("../data/station_final_final.csv", "r");
    char ligne[1000];
    fgets(ligne, 1000, file); //on saute la première ligne
    //exemple de ligne : FRELCPBLOHM;68042;[7.50290400,47.60821400];225;7.502904;47.608214;Blotzheim
    //exemple de ligne : FRS56E56053AB1G;[2.25,48.94];22.00;2.25;48.94;
    
    //préparation des variables qui vont servir
    int power, indice_carre, nb_prises;
    coordinates coords;
    char* identifiant;

    //boucle sur la liste des stations
    while(fgets(ligne, 1000, file) != NULL){
        char* curseur = strtok(ligne, ";"); // identifiants

        identifiant = curseur;
        size_t len = strlen(identifiant);
        char* copy_id = malloc(len + 1);
        strcpy(copy_id, identifiant);

        curseur = strtok(NULL, ";"); // code postal ou coordonnées en []
        if (strchr(curseur, '[') != NULL) { // cas où on a pas de code postal
            curseur = strtok(NULL, ";"); // puissance
            power = atoi(curseur);
            curseur = strtok(NULL, ";"); // lon station
            coords.longitude = atof(curseur); 
            curseur = strtok(NULL, ";"); // lat station
            coords.latitude = atof(curseur); 
            curseur = strtok(NULL, ";"); // on saute les trois ;;; pour arriver au nombre de prises
            int n = strlen(curseur);
            curseur[n-1] = '\0';
            nb_prises = atoi(curseur);
        }else { // cas où on a un code postal
            curseur = strtok(NULL, ";"); // code postal
            curseur = strtok(NULL, ";"); // puissance
            power = atoi(curseur);
            curseur = strtok(NULL, ";"); // lon station
            coords.longitude = atof(curseur); 
            curseur = strtok(NULL, ";"); // lat station
            coords.latitude = atof(curseur);
            curseur = strtok(NULL, ";"); // soit prise, soit ville
            int n = strlen(curseur);
            curseur[n-1] = '\0';
            if(atoi(curseur) == 0){
                curseur = strtok(NULL, ";"); // si on est sur une ville il faut passer au suivant
                int n = strlen(curseur);
                curseur[n-1] = '\0';
            } 
            nb_prises = atoi(curseur);
        }
        
        indice_carre = coordtocarre_simu(coords); // on récupère l'indice du carré

        if(power == 0) power+=250000; // si la puissance est nulle on la met à 250kW
        if(power<1000) power *= 1000; // si la puissance est inférieure à 1000 c'est qu'elle est en kW donc on la passe en W
        if(indice_carre != -1 && power) station_append_simu(tab_of_carre[indice_carre], power, coords, nb_prises, copy_id); // on prend que les stations en France et hors Corse
        if(indice_carre == -1) free(copy_id);
    }
    fclose(file);
    return tab_of_carre;
}

void station_append_aux_simu(station* s, int power, coordinates coords, int nb_prises, char* identifiant){
    if(s->next == NULL){
        s->next = malloc(sizeof(station));
        s->next->coords = coords;
        s->next->next = NULL;
        s->next->power = power;
        s->next->nb_prises = nb_prises;
        s->next->nb_vehicules = malloc(TAILLE_TAB_ETAPES*sizeof(int));
        s->next->id = identifiant;
        for(int i = 0 ; i < TAILLE_TAB_ETAPES ; i++){
            s->next->nb_vehicules[i] = 0;
        }
    }else{
        station_append_aux_simu(s->next, power, coords, nb_prises, identifiant);
    }
}

void station_append_simu(carre* carre, int power, coordinates coords, int nb_prises, char* identifiant){
    if(carre->head == NULL){
        carre->head = malloc(sizeof(station));
        carre->head->coords = coords;
        carre->head->next = NULL;
        carre->head->power = power;
        carre->head->nb_prises = nb_prises;
        carre->head->nb_vehicules = malloc(TAILLE_TAB_ETAPES*sizeof(int));
        carre->head->id = identifiant;
        for(int i = 0 ; i < TAILLE_TAB_ETAPES ; i++){
            carre->head->nb_vehicules[i] = 0;
        }
        carre->nb_stations++;
    }else{
        station_append_aux_simu(carre->head, power, coords, nb_prises, identifiant);
        carre->nb_stations++;
    }
}

// CHARGEMENT DES VILLES
city** load_cities_simu(){
    //Ouverture du fichier
    FILE* file = fopen("../data/cities.csv", "r");
    char line[1000];
    fgets(line, 1000, file);

    //on initialise les variables qui vont recevoir les villes
    city** cities = malloc(NB_VILLES*sizeof(city*));
    for(int i = 0 ; i < NB_VILLES ; i++){
        cities[i] = malloc(sizeof(city));
        cities[i]->coordonnees = malloc(sizeof(coordinates));
    }
    int i = 0;

    //boucle sur les lignes
    while((fgets(line, 1000, file) != NULL) && i < NB_VILLES){
        char *curseur;
        curseur = strtok(line, ",");
        curseur = strtok(NULL, ",");
        curseur = strtok(NULL, ",");
        curseur = strtok(NULL, ","); // on saute les infos inutiles

        curseur = strtok(NULL, ","); // name
        //vu que strdup ne veut pas fonctionner on l'a fait à la main
        int len_name = strlen(curseur);
        char* copy_name = malloc(len_name + 1);
        strcpy(copy_name, curseur);

        curseur = strtok(NULL, ","); // on saute les infos inutiles

        curseur = strtok(NULL, ","); // gps_lat
        int len_gps_lat = strlen(curseur);
        char* copy_gps_lat = malloc(len_gps_lat + 1);
        strcpy(copy_gps_lat, curseur);

        curseur = strtok(NULL, ","); // gps_lng
        int len_gps_lng = strlen(curseur);
        char* copy_gps_lng = malloc(len_gps_lng + 1);
        strcpy(copy_gps_lng, curseur);

        if(atof(copy_gps_lat) >= 41 && atof(copy_gps_lat) <= 53 && atof(copy_gps_lng) >= -5 && atof(copy_gps_lng) <= 9){ //on choisit que les villes qui sont dans la France
            //on recupere le nom de la ville
            cities[i]->nom = copy_name;
            cities[i]->coordonnees->latitude = atof(copy_gps_lat);
            cities[i]->coordonnees->longitude = atof(copy_gps_lng);
            i++;
        }else free(copy_name);
        
        free(copy_gps_lat);
        free(copy_gps_lng);
    }
    fclose(file);
    return cities;
}

// CHARGEMENT DES VOITURES
car** load_cars_simu(){
    //Ouverture du fichier
    FILE *file = fopen("../data/voiture_data2.csv", "r");
    char line[1000];
    fgets(line, 1000, file); // Ignore la première ligne (en-tête)
    
    //Allocation de la mémoire pour la liste des voitures
    car** cars = malloc(sizeof(car*)*NB_VOITURES);
    int i = 0;
    //Parcours du fichier
    while (fgets(line, 1000, file) != NULL) {
        // Sépare chaque champ de la ligne en utilisant ";" comme délimiteur
        char *modelName, *range, *efficiency;
        
        modelName = strtok(line, ";");
        range = strtok(NULL, ";");
        efficiency = strtok(NULL, ";");
        
        //vu que strdup ne veut pas fonctionner on l'a fait à la main
        size_t len = strlen(modelName);
        char* copy = malloc(len + 1);
        strcpy(copy, modelName);
        
        //Création de la voiture
        car* c = malloc(sizeof(car));
        c->id = i;
        c->modelName = copy;
        c->range = atoi(range);
        c->efficiency = atoi(efficiency);
        srand(time(NULL));
        cars[i] = c;
        //incrémentation
        i++;
    }
    fclose(file);
    return cars;
}


//----------FONCTIONS AUXILIAIRES----------
int coordtocarre_simu(coordinates coords) {
    // Coordonnées des limites des carrés de la France
    double limiteslat[17] = {41,41.75, 42.5,43.25, 44,44.75, 45.5,46.25, 47,47.75, 48.5,49.25, 50,50.75, 51.5,52.25, 53}; // 1er carré ; de 41.2 à 42.5 .....
    double limiteslon[17] = {-5,-4.25, -3.5,-2.75, -2,-1, 0,1, 2,2.75, 3.5,4.25, 5,6, 7,8, 9}; // 1er carré ; de -5 à -3.5 .....
    // Trouver le carré dans lequel se trouve le point
    int carrelat = -1, carrelon = -1;
    for (int i = 0; i < 16; i++) {
        if (coords.latitude >= limiteslat[i] && coords.latitude < limiteslat[i+1]) {
            carrelat = i;
            break;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (coords.longitude >= limiteslon[i] && coords.longitude < limiteslon[i+1]) {
            carrelon = i;
            break;
        }
    }
    int carre=0;
    // Calculer l'indice du carré
    if (carrelat == -1 || carrelon == -1) {
        return -1; // point en dehors de la France
    } else {
        carre = carrelat * 16 + carrelon; // calcul de l'indice du carré
        return carre;
    }
}

double distance_simu(coordinates A, coordinates B){
    double dlat = DEGTORAD(B.latitude - A.latitude);
    double dlon = DEGTORAD(B.longitude - A.longitude);

    double a = sin(dlat/2)*sin(dlat/2)+cos(DEGTORAD(A.latitude))*cos(DEGTORAD(B.latitude))*sin(dlon/2)*sin(dlon/2); // formule de Haversine
    double c = 2*atan2(sqrt(a),sqrt(1 - a));
    double distance = R*c;

    return distance;
}

coordinates point_on_line_simu(coordinates A, coordinates B, double dist){
    double dAB = distance_simu(A,B);
    double dx = (B.longitude - A.longitude) / dAB;
    double dy = (B.latitude - A.latitude) / dAB;
    coordinates C;
    C.latitude = A.latitude + (dy*dist);
    C.longitude = A.longitude + (dx*dist);
    return C;
}

station* find_nearest_station_aux_simu(station* S, double distance_mini, coordinates coords, station* nearest_station){
    if(S == NULL) return nearest_station; // si on arrive à une station nulle c'est qu'on a parcouru toutes les stations, donc on renvoie la plus proche
    else if(distance_simu(S->coords, coords) <= distance_mini){ // si station plus proche alors maj des variables contenant la distance mini et la station la plus proche
        distance_mini = distance_simu(S->coords, coords);
        nearest_station = S;
    }
    return find_nearest_station_aux_simu(S->next, distance_mini, coords, nearest_station); // appel récursif sur la station suivante dans la liste
}

station* find_nearest_station_simu(carre** tab, coordinates coords){
    int indice_carre = coordtocarre_simu(coords);
    if(indice_carre < 0 || indice_carre > 255 || tab[indice_carre]->nb_stations == 0) return NULL; // si bug d'indice -> on renvoie NULL
    else{
        double distance_mini = distance_simu(coords, tab[indice_carre]->head->coords); // on initilaise la distance mini avec la première station du carré
        station* nearest_station = tab[indice_carre]->head; // on initialise la station la plus proche avec la première station du carré
        return find_nearest_station_aux_simu(tab[indice_carre]->head->next, distance_mini, coords, nearest_station);
    }
}


//----------FONCTIONS D'AFFICHAGE----------
void print_coordinates_simu(coordinates coordinates){
    printf("[%f ; %f]", coordinates.latitude, coordinates.longitude);
}

void print_etat_simu(conditions etat){
    char* liste_etat[] = {"ROULE", "ARRET", "EN CHARGE"};
    printf("%s", liste_etat[etat]);
}

void print_AutoPath_simu(AutoPath** AutoPath, int taile_simulation){
    for(int i = 0 ; i < taile_simulation ; i++){
        int duree_trajet_min = 0;
        int duree_trajet_h = 0;
        printf("Trajet n°%d : %s -> %s. Batterie au départ du trajet : %d%%. Batterie à la fin du trajet : %d%%. Batterie minimum : %d%%. Voiture : %s. Temps d'attente maximum à une station : %dmin. Nombre d'étapes : %d. Durée du trajet : %dh%dmin.\n\n", i+1, AutoPath[i]->ville_depart, AutoPath[i]->ville_arrivee, AutoPath[i]->batterie_init, AutoPath[i]->batterie, AutoPath[i]->batterie_min, AutoPath[i]->voiture->modelName, AutoPath[i]->temps_attente_max, AutoPath[i]->trajet->nb_etapes, AutoPath[i]->duree_trajet/60, AutoPath[i]->duree_trajet - (AutoPath[i]->duree_trajet/60)*60);
        if(AutoPath[i]->trajet->nb_etapes >= 1){
            for(int j = 1 ; j <= AutoPath[i]->trajet->nb_etapes ; j++){
                duree_trajet_min += 10;
                if(duree_trajet_min < 60){
                    printf("          Temps passé depuis le départ : %dmin -> ", duree_trajet_min);
                }else{
                    duree_trajet_h = duree_trajet_min / 60;
                    printf("          Temps passé depuis le départ : %dh%dmin -> ", duree_trajet_h, duree_trajet_min - (duree_trajet_h*60));
                }
                print_coordinates_simu(AutoPath[i]->trajet->etapes[j]);
                printf(" - ");
                print_etat_simu(AutoPath[i]->trajet->etats[j]);
                printf(" - Niveau de batterie : %d%%", AutoPath[i]->trajet->batterie[j]);
                printf("\n");
            }
        }
        if(AutoPath[i]->est_arrive == 1) printf("\nUtilisateur arrivé à destination !\n");
        else printf("\nErreur dans la recherche de station ou trop d'étapes !");
        printf("\n========================================\n\n");
    }
}

void print_etat_station_simu(station* S, int duree_simulation, int debut_simulation){
    if(S != NULL){
        int test_debordement = 0;
        for(int i = debut_simulation ; i < duree_simulation + debut_simulation + 1 ; i++) if(S->nb_vehicules[i] > 2*S->nb_prises) test_debordement = 1;
        if(test_debordement == 1) {printf("\nIdentifiant de la station : %s. Nombre de prises disponibles : %d. Coordonnées : ", S->id, S->nb_prises); print_coordinates_simu(S->coords); printf("\n");}
        for(int i = debut_simulation ; i < duree_simulation  + debut_simulation + 1 ; i++) if(S->nb_vehicules[i] > 2*S->nb_prises) printf("     Trop de véhicules à l'instant t = %dmin ! Il y a %d véhicules en attente !\n", i*10, S->nb_vehicules[i] - S->nb_prises);
        print_etat_station_simu(S->next, duree_simulation, debut_simulation);
    }
}

//----------FONCTIONS DE LIBÉRATION DE LA MÉMOIRE----------
void destroy_station_simu(station* station){
    free(station->nb_vehicules);
    free(station->id);
    if(station->next != NULL) destroy_station_simu(station->next);
    station->next = NULL;
    free(station);
}

void destroy_carre_simu(carre* carre){
    if(carre->head != NULL) destroy_station_simu(carre->head);
    free(carre);
}

void destroy_tab_carre_simu(carre** tab){
    for(int i = 0 ; i < NB_CARRES ; i++) destroy_carre_simu(tab[i]);
    free(tab);
}

void destroy_cars_simu(car** cars){
    for (int i = 0; i < NB_VOITURES; i++){
        free(cars[i]->modelName);
        free(cars[i]);
    }
    free(cars);
}

void destroy_cities_simu(city** cities){
    for(int i = 0 ; i < NB_VILLES ; i++){
        free(cities[i]->nom);
        free(cities[i]->coordonnees);
        free(cities[i]);
    }
    free(cities);
}

void destroy_AutoPath_simu(AutoPath** AutoPath, int taille_simulation){
    for(int i = 0 ; i < taille_simulation ; i++){
        //destruction partie voiture
        free(AutoPath[i]->voiture->modelName);
        free(AutoPath[i]->voiture);

        //destruction partie ville
        free(AutoPath[i]->ville_arrivee);
        free(AutoPath[i]->ville_depart);

        //destruction partie journey
        free(AutoPath[i]->trajet->etapes);
        free(AutoPath[i]->trajet->etats);
        free(AutoPath[i]->trajet->batterie);
        free(AutoPath[i]->trajet);

        //destruction case
        free(AutoPath[i]);
    }
    free(AutoPath);
}


//----------FONCTIONS PRINCIPALES----------
AutoPath** initAutoPath_simu(int taille_simulation){
    //on initialise le rand
    srand(time(NULL));

    //initialisation de la structure
    AutoPath** tab_init = malloc(taille_simulation*sizeof(AutoPath*));
    for(int i = 0 ; i < taille_simulation ; i++){
        tab_init[i] = malloc(sizeof(AutoPath));
        tab_init[i]->voiture = malloc(sizeof(car));
        tab_init[i]->trajet = malloc(sizeof(journey));
        tab_init[i]->trajet->etapes = malloc(TAILLE_TAB_ETAPES*sizeof(coordinates));
        tab_init[i]->trajet->etats = malloc(TAILLE_TAB_ETAPES*sizeof(conditions));
        tab_init[i]->trajet->batterie = malloc(TAILLE_TAB_ETAPES*sizeof(int));
        tab_init[i]->trajet->nb_etapes = 0;
        tab_init[i]->duree_trajet = 0;
    }

    //on charge le tableau des véhicules et des villes
    car** car_tmp = load_cars_simu();
    city** city_tmp = load_cities_simu();

    //initialisation des variables qui vont contenir les indices aléatoires
    int indice;
    int depart;
    int arrivee;

    //on remplit les cases
    for(int i = 0 ; i < taille_simulation ; i++){
        //on commence par les voitures
        indice = rand() % NB_VOITURES;

        //nouveau strdup a la main
        int len = strlen(car_tmp[indice]->modelName);
        char* copy = malloc(len + 1);
        strcpy(copy, car_tmp[indice]->modelName);

        //on remplit chaque champs
        tab_init[i]->voiture->id = car_tmp[indice]->id;
        tab_init[i]->voiture->modelName = copy;
        tab_init[i]->voiture->range = car_tmp[indice]->range;
        tab_init[i]->voiture->efficiency = car_tmp[indice]->efficiency;


        //on prend ensuite au hasard un niveau de batterie de départ, un niveau de batterie minimum et le temps d'attente maximum à une station
        tab_init[i]->batterie_init = rand() % 51 + 50;
        tab_init[i]->batterie = tab_init[i]->batterie_init;
        tab_init[i]->batterie_min = rand() % 16 + 5;
        int nombres[] = {20, 30, 40, 50, 60, 70, 80, 90};
        int indice_aleatoire = rand() % 8;
        tab_init[i]->temps_attente_max = nombres[indice_aleatoire];
        tab_init[i]->trajet->batterie[0] = tab_init[i]->batterie_init;


        //on choisit ensuite le départ et l'arrivée
        depart = rand() % NB_VILLES;
        arrivee = rand() % NB_VILLES;

        //encore un strdup a la main...
        int len_dep = strlen(city_tmp[depart]->nom);
        char* copy_dep = malloc(len_dep + 1);
        strcpy(copy_dep, city_tmp[depart]->nom);

        int len_arr = strlen(city_tmp[arrivee]->nom);
        char* copy_arr = malloc(len_arr + 1);
        strcpy(copy_arr, city_tmp[arrivee]->nom);

        tab_init[i]->ville_depart = copy_dep;
        tab_init[i]->ville_arrivee = copy_arr;

        //on gère ensuite la partie trajet (=journey) de la structure AutoPath
        tab_init[i]->trajet->etapes[0].latitude = city_tmp[depart]->coordonnees->latitude;
        tab_init[i]->trajet->etapes[0].longitude = city_tmp[depart]->coordonnees->longitude;
        tab_init[i]->trajet->etapes[TAILLE_TAB_ETAPES - 1].latitude = city_tmp[arrivee]->coordonnees->latitude;
        tab_init[i]->trajet->etapes[TAILLE_TAB_ETAPES - 1].longitude = city_tmp[arrivee]->coordonnees->longitude;
        tab_init[i]->trajet->etats[0] = ARRET;
        tab_init[i]->trajet->etats[TAILLE_TAB_ETAPES - 1] = ARRET;

        tab_init[i]->est_arrive = 0;
    }
    destroy_cars_simu(car_tmp);
    destroy_cities_simu(city_tmp);
    return tab_init;
}

carre** travelAutopath_simu(AutoPath** AutoPath, int taille_simulation){
    //comptage des cas d'erreurs
    int cas_erreur1 = 0; // nombre d'utilisateurs dans un carré sans stations
    int cas_erreur2 = 0; // nombre d'utilisateurs qui n'ont pas assez de batterie pour aller à la station suivante

    // préparation des variables utiles
    double distance_restante, distance_avant_prochaine_etape; // distance avant arrivée, distance avant prochain point
    int cpt, puissance, duree_rechargement, capacite, batterie, nb_cycle, nb_arrive, nb_cycle_max; // limitation du nombre d'étapes, puissance de la station, temps de rechargement en minutes, capacité de la voiture en Wh, batterie en %, nombre de cycle de 10 minutes de rechargement à faire, nombre d'utilisateurs arrivés sans bugs, nombre maximum de cycles qu'on peut faire avant que l'utilisateur ne veuille partir
    station* S;

    // on charge les stations
    carre** tab_of_carre = load_stations_simu();

    nb_arrive = 0;

    //affichage de la barre de chargement :
    int progress = 0;
    char barre[101];
    memset(barre, '.', 100);
    barre[100] = '\0';


    for(int i = 0 ; i < taille_simulation ; i++){
        //remise à 0 du compteur
        cpt = 0; 

        //calcul de la capacité de la voiture
        capacite = AutoPath[i]->voiture->efficiency*AutoPath[i]->voiture->range;

        //boucle sur le trajet
        while((AutoPath[i]->est_arrive != 1) && (cpt < (TAILLE_TAB_ETAPES - 10))){
            //on utilise le nombre d'étapes pour savoir où on en est dans le tableau :
            //quand nb_etapes = 0, on est au départ, cad la première case du tableau des étapes. A chaque étapes, on augmente nb_etapes de 1, ce qui va nous permettre d'avancer dans le tableau

            //on calcule la distance restante avant l'arrivée
            distance_restante = distance_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes], AutoPath[i]->trajet->etapes[TAILLE_TAB_ETAPES - 1]);
            
            //on calcule la distance restante avant le prochain point
            distance_avant_prochaine_etape = distance_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes], point_on_line_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes], AutoPath[i]->trajet->etapes[TAILLE_TAB_ETAPES - 1], DISTMOY));
            
            //si l'arrivée est assez proche et qu'on a assez de batterie -> pas d'étape, on est directement arrivé
            if((distance_restante <= DISTMOY) && (BATTERIE_RESTANTE(distance_restante, AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range) >= AutoPath[i]->batterie_min)){
                AutoPath[i]->batterie = BATTERIE_RESTANTE(distance_restante, AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range); // on met à jour la batterie
                AutoPath[i]->est_arrive = 1;
                nb_arrive++;
                AutoPath[i]->trajet->batterie[TAILLE_TAB_ETAPES - 1] = AutoPath[i]->batterie;
            }
            
            //si on a assez de batterie pour arriver au prochain point
            else if(BATTERIE_RESTANTE(distance_avant_prochaine_etape, AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range) >= AutoPath[i]->batterie_min){
                AutoPath[i]->batterie = BATTERIE_RESTANTE(distance_avant_prochaine_etape, AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range); // on met à jour la batterie
                AutoPath[i]->trajet->nb_etapes++; // on incrémente le nombre d'étapes
                AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes] = point_on_line_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes-1], AutoPath[i]->trajet->etapes[TAILLE_TAB_ETAPES - 1], DISTMOY); // les coordonées correspondantes à l'étape sont calculées à partir du point où on était avant, sachant qu'on va vers l'arrivée et qu'on parcourt une certaine distance
                AutoPath[i]->trajet->etats[AutoPath[i]->trajet->nb_etapes] = ROULE; // on roule, donc on met à jour l'état dans lequel on est
                cpt++; // incrémentation du compteur car on a fait une étape
                AutoPath[i]->duree_trajet += 10; // on augmente la durée de 10 min
                AutoPath[i]->trajet->batterie[AutoPath[i]->trajet->nb_etapes] = AutoPath[i]->batterie;

            }
            
            //sinon on recharge
            else{
                S = find_nearest_station_simu(tab_of_carre, AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes]);
                if(S == NULL) {cpt = TAILLE_TAB_ETAPES - 10;cas_erreur1++;} // si on reçoit pas de station, on arrête tout
                else if (BATTERIE_RESTANTE(distance_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes], S->coords), AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range) <= 0){
                    //printf("%f\n", BATTERIE_RESTANTE(distance_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes], S->coords), AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range));
                    cpt = TAILLE_TAB_ETAPES - 10; // si on n'a pas assez de batterie pour aller à la station la plus proche on arrête le trajet
                    cas_erreur2++;
                }
                else{
                    AutoPath[i]->batterie = BATTERIE_RESTANTE(distance_simu(AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes], S->coords), AutoPath[i]->batterie, AutoPath[i]->voiture->efficiency, AutoPath[i]->voiture->range); // on met à jour la batterie
                    AutoPath[i]->trajet->nb_etapes++;
                    AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes] = S->coords;
                    AutoPath[i]->trajet->etats[AutoPath[i]->trajet->nb_etapes] = EN_CHARGE;
                    cpt++;
                    AutoPath[i]->duree_trajet += 10;
                    AutoPath[i]->trajet->batterie[AutoPath[i]->trajet->nb_etapes] = AutoPath[i]->batterie;
                    S->nb_vehicules[AutoPath[i]->trajet->nb_etapes]++;

                    // on récupère les données dont on a besoin
                    puissance = S->power;
                    batterie = AutoPath[i]->batterie;

                    //on calcule le temps qu'on va passer à la station (par tranche de 10 minutes)
                    duree_rechargement = TEMPS_RECHARGEMENT(batterie, puissance, capacite);
                    nb_cycle = (duree_rechargement / 10) + 1;

                    //si le nombre de cycle est trop grand on le limite
                    nb_cycle_max = AutoPath[i]->temps_attente_max/10; // on définit le nombre maximum de cycle
                    if(nb_cycle > nb_cycle_max) nb_cycle = nb_cycle_max;
                    for(int j = 0 ; j < nb_cycle ; j++){ // pour chaque cycle de 10 min on ajoute une étape
                        AutoPath[i]->trajet->nb_etapes++;
                        AutoPath[i]->trajet->etapes[AutoPath[i]->trajet->nb_etapes] = S->coords;
                        AutoPath[i]->trajet->etats[AutoPath[i]->trajet->nb_etapes] = EN_CHARGE;
                        cpt++;
                        AutoPath[i]->duree_trajet += 10;
                        AutoPath[i]->batterie += (int) ((puissance*(1/6.0)*EFFICACITE_RECHARGEMENT*100)/capacite)+1; // calcul du taux de charge après un cycle -> 1 cycle (10min) = 1/6.0 h. On ajoute 1 à la fin pour arrondir à l'excès
                        AutoPath[i]->trajet->batterie[AutoPath[i]->trajet->nb_etapes] = AutoPath[i]->batterie;
                        S->nb_vehicules[AutoPath[i]->trajet->nb_etapes]++;
                    }
                    if(AutoPath[i]->trajet->nb_etapes+1 < TAILLE_TAB_ETAPES-2) if(S->nb_vehicules[AutoPath[i]->trajet->nb_etapes+1] != 0) S->nb_vehicules[AutoPath[i]->trajet->nb_etapes+1]--; // on enlève le véhicule de la station si il y en a un    
                }
            }
        }

        //barre de chargement
        if(taille_simulation >= 100) if(i%((taille_simulation/100)) == 0){
            if(progress < 100) barre[progress] = '#';
            printf("Chargement : [%s] - %d/%d trajet(s) traité(s).\r", barre, i%taille_simulation + taille_simulation/100, taille_simulation);
            fflush(stdout);
            progress++;
        }
    }
    printf("\n\n");
    printf("Nombre d'utilisateurs arrivés : %d/%d.\n\n", nb_arrive, taille_simulation);
    printf("Nombre d'utilisateurs dans un carré sans station : %d. Nombre d'utilisateurs qui n'ont pas assez de batterie pour aller à la station suivante : %d. Nombre d'utilisateurs ayant trop d'étapes : %d.\n", cas_erreur1, cas_erreur2, taille_simulation - cas_erreur1 - cas_erreur2 - nb_arrive);

    return tab_of_carre;
}
