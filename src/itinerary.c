/**********************************************************************/
/*********** fonctions utilisée pour créer l'itinéraire ***************/
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "itinerary.h"



// CHARGEMENT DES STATIONS
carre** load_stations(){

    //création de la structure
    carre** tab_of_carre = malloc(NB_CARRES*sizeof(carre*));
    for(int i = 0 ; i < NB_CARRES ; i++){
        tab_of_carre[i] = malloc(sizeof(carre));
        tab_of_carre[i]->head = NULL;
        tab_of_carre[i]->nb_stations = 0;
    } 

    //ouverture et lecture du fichier
    FILE* file = fopen("../data/station_final2.csv", "r");
    char ligne[1000];
    fgets(ligne, 1000, file); //on saute la première ligne
    //exemple de ligne : FRELCPBLOHM;68042;[7.50290400,47.60821400];225;7.502904;47.608214;Blotzheim
    //exemple de ligne : FRS56E56053AB1G;[2.25,48.94];22.00;2.25;48.94;
    
    //préparation des variables qui vont servir
    int power, indice_carre;
    coordinates coords;

    //boucle sur la liste des stations
    while(fgets(ligne, 1000, file) != NULL){
        char* curseur = strtok(ligne, ";");
        curseur = strtok(NULL, ";");
        if (strchr(curseur, '[') != NULL) {
            curseur = strtok(NULL, ";"); // on saute les infos inutiles
            power = atoi(curseur);
            curseur = strtok(NULL, ";");
            coords.longitude = atof(curseur); // lon station
            curseur = strtok(NULL, ";");
            coords.latitude = atof(curseur); // lat station
        }
        else {
            curseur = strtok(NULL, ";");
            curseur = strtok(NULL, ";"); // on saute les infos inutiles
            power = atoi(curseur);
            curseur = strtok(NULL, ";");
            coords.longitude = atof(curseur); // lon station
            curseur = strtok(NULL, ";");
            coords.latitude = atof(curseur); // lat station
        }
        
        indice_carre = coordtocarre(coords.latitude, coords.longitude); // on récupère l'indice du carré

        if(power<1000) power *= 1000; // si la puissance est inférieure à 1000 c'est qu'elle est en kW donc on la passe en W
        if(indice_carre != -1) station_append(tab_of_carre[indice_carre], power, coords); // on prend que les stations en France et hors Corse
    }
    fclose(file);
    return tab_of_carre;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void station_append_aux(station* s, int power, coordinates coords){
    if(s->next == NULL){
        s->next = malloc(sizeof(station));
        s->next->coords = coords;
        s->next->next = NULL;
        s->next->power = power;
    }else{
        station_append_aux(s->next, power, coords);
    }
}

void station_append(carre* carre, int power, coordinates coords){
    if(carre->head == NULL){
        carre->head = malloc(sizeof(station));
        carre->head->coords = coords;
        carre->head->next = NULL;
        carre->head->power = power;
        carre->nb_stations++;
    }else{
        station_append_aux(carre->head, power, coords);
        carre->nb_stations++;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// coord => carré
int coordtocarre(double lat, double lon) {
    // Coordonnées des limites des carrés de la France
    double limiteslat[17] = {41,41.75, 42.5,43.25, 44,44.75, 45.5,46.25, 47,47.75, 48.5,49.25, 50,50.75, 51.5,52.25, 53}; // 1er carré ; de 41.2 à 42.5 .....
    double limiteslon[17] = {-5,-4.25, -3.5,-2.75, -2,-1, 0,1, 2,2.75, 3.5,4.25, 5,6, 7,8, 9}; // 1er carré ; de -5 à -3.5 .....
    // Trouver le carré dans lequel se trouve le point
    int carrelat = -1, carrelon = -1;
    for (int i = 0; i < 16; i++) {
        if (lat >= limiteslat[i] && lat < limiteslat[i+1]) {
            carrelat = i;
            break;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (lon >= limiteslon[i] && lon < limiteslon[i+1]) {
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

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui convertit un angle en degrés en radians
double degtoradians(double deg) {
    return (deg/180)*pi;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Calcule la distance en kilomètres entre deux points géographiques
double distance(double lat1, double lon1, double lat2, double lon2) {
    // Conversion des angles en radians
    double dlat = degtoradians(lat2-lat1);
    double dlon = degtoradians(lon2-lon1);

    // Calcul de la distance selon la formule de Haversine
    double a = sin(dlat/2)*sin(dlat/2)+cos(degtoradians(lat1))*cos(degtoradians(lat2))*sin(dlon/2)*sin(dlon/2);
    double c = 2*atan2(sqrt(a),sqrt(1 - a));
    double distance = R*c;

    return distance;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
coordinates find_city(char* city_name) {
    //Ouverture du fichier
    FILE* file = fopen("../data/cities.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    char line[1000];
    fgets(line, 1000, file); // on ignore la première ligne

    //on initialise les variables qui vont recevoir les villes
    char name[100];
    coordinates coords = {0,0};

    //boucle sur les lignes
    while(fgets(line, 1000, file) != NULL){
        char *curseur;
        curseur = strtok(line, ",");
        curseur = strtok(NULL, ",");
        curseur = strtok(NULL, ",");
        curseur = strtok(NULL, ","); // on saute les infos inutiles

        curseur = strtok(NULL, ","); // name
        if (curseur != NULL) {
                strcpy(name, curseur);
        }

        curseur = strtok(NULL, ","); // on saute les infos inutiles

        curseur = strtok(NULL, ","); // gps_lat
        if (curseur != NULL) {
            coords.latitude = atof(curseur);
        }

        curseur = strtok(NULL, ","); // gps_lng
        if (curseur != NULL) {
            coords.longitude = atof(curseur);
        }

        if (strcmp(name, city_name) == 0) { // Si la ville est trouvée, on retourne ses coordonnées GPS
            fclose(file);
            return coords;
        }
    }
    fclose(file);
    coords.latitude = 0;
    coords.longitude = 0;
    return coords;// Si la ville n'est pas trouvée, on retourne des coordonnées GPS nulles
    
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui donne la station la plus proche d'un point géographique:
station* find_nearest_station_aux(station* S, double distance_mini, coordinates coords, station* nearest_station){
    if(S == NULL){
        return nearest_station; // si on arrive à une station nulle c'est qu'on a parcouru toutes les stations, donc on renvoie la plus proche
    }
    else {
        int distTemp = distance(S->coords.latitude, S->coords.longitude, coords.latitude, coords.longitude);
        if(distTemp <= distance_mini){ // si station plus proche alors maj des variables contenant la distance mini et la station la plus proche
            distance_mini = distTemp;
            nearest_station = S;
        }
        return find_nearest_station_aux(S->next, distance_mini, coords, nearest_station); // appel récursif sur la station suivante dans la liste
    }
    
}

station* find_nearest_station(carre** tab, coordinates coords){
    int indice_carre = coordtocarre(coords.latitude, coords.longitude);
    if(indice_carre < 0 || indice_carre > 255 || tab[indice_carre]->nb_stations == 0) return NULL; // si bug d'indice -> on renvoie NULL
    else{
        double distance_mini = distance(coords.latitude, coords.longitude, tab[indice_carre]->head->coords.latitude, tab[indice_carre]->head->coords.longitude); // on initilaise la distance mini avec la première station du carré
        station* nearest_station = tab[indice_carre]->head; // on initialise la station la plus proche avec la première station du carré
        return  find_nearest_station_aux(tab[indice_carre]->head->next, distance_mini, coords, nearest_station);
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui donne un point situé à une distance donnée sur la droite AB
coordinates point_on_line(coordinates A, coordinates B, double dist){
    
    double latA = A.latitude;
    double lonA = A.longitude;
    double latB = B.latitude;
    double lonB = B.longitude;

    // Calcul de la distance entre A et B
    double dAB = distance(latA, lonA, latB, lonB);

    // Calcul des coefficients de la droite reliant A et B
    double dx = (lonB - lonA) / dAB;
    double dy = (latB - latA) / dAB;

    // Calcul des coordonnées du point C
    double lonC = lonA + (dx * dist);
    double latC = latA + (dy * dist);

    //on retourne les coordonnées du point voulu
    return (coordinates){latC, lonC};
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui charge la liste des voitures:
car** load_cars(){
    //Ouverture du fichier
    FILE *file = fopen("../data/voiture_data2.csv", "r");
    char line[1000];
    fgets(line, 1000, file); // Ignore la première ligne (en-tête)
    
    //Allocation de la mémoire pour la liste des voitures
    car** cars = malloc(sizeof(car)*299);
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
        cars[i] = c;
        //incrémentation
        i++;
    }
    fclose(file);
    return cars;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui détruit le tableau de voitures en fonction de la taille du tableau donné en entrée
void destroy_cars(car** cars){
    for (int i = 0; i < 299; i++){
        free(cars[i]->modelName);
        free(cars[i]);
    }
    free(cars);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//destruction du tableau de station
void destroy_stations(carre** tab_of_carre) {
    for (int i = 0; i < NB_CARRES; i++) {
        station* current = tab_of_carre[i]->head;
        station* temp;
        while (current != NULL) {
            temp = current->next;
            free(current);
            current = temp;
        }
        free(tab_of_carre[i]);
    }
    free(tab_of_carre);
}



/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui trouve le vehicule dans la liste à partir de sa marque
int find_car_by_model(char model[100], car** listCar ){
    for (int i = 0; i < 299; i++){
        if (strcmp(listCar[i]->modelName, model) == 0){
            return i;
        }
    }
    return -1;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui calcule le niveau de la batterie à la fin du trajet
double battery_level(double batteryLevel, double distance, double range, double efficiency){
    double capacityMax = range * efficiency; // calcul de la capacité de la batterie de la voiture (en Wh)
    double actualCapacity = capacityMax * (batteryLevel/100);
    double capacity = actualCapacity - (distance * efficiency); // calcul de la capacité restante de la batterie (en Wh)
    double battery = (capacity / capacityMax) * 100; // calcul du niveau de la batterie (en %)
    return battery;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui calcule le temps d'attente à une station pour recharger son véhicule (en minutes)
double waiting_time(double powerStation, double batteryLevel, double capacity){
    //power = puissance de recharge de la voiture (en kW), batteryLevel = niveau de la batterie (en %), capacity = capacité de la batterie (en Wh)
    
    capacity = capacity / 1000; //on converti notre capacité en kWh
    //calcul du temps d'attente en minutes:
    double time = ((capacity * ((100 - batteryLevel)/100)) / powerStation)*60;
    return time;
    
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui calcule la distance que l'on peut parcourir avant d'atteindre la charge minimale donnée de la batterie
double distance_before_battery_limit(double batteryLevel, double batteryMin, double capacity, double efficiency){
    //batteryLevel = niveau de la batterie (en %), capacity = capacité de la batterie (en Wh), efficiency = efficacité de la voiture (en Wh/km)
    double range = (capacity * (batteryLevel/100)) / efficiency; //reviens à donner l'autonomie de la voiture = range
    double distanceSurReserve = (capacity * (batteryMin/100)) / efficiency; //distance parcouru sous la charge minimale de la batterie
    
    return range - distanceSurReserve;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui calcule la distance que l'on peut parcourir en "time" minutes (avec une vitesse moyenne de 100km/h)
double distance_in_time(double time){
    return VitesseMoyenne * (time/60); //on oublie pas la conversion en heure de time
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Fonction qui affiche le trajet
void print_itinerary(carre** list_station, coordinates A, coordinates B, car* voiture, double batteryLevel, double batteryMin, int timeMax){
    double autonomy = voiture->range;
    double efficiency = voiture->efficiency;
    double capacity = autonomy * efficiency;
    printf("\nCalcul de votre itinéraire en cours ........\n \n");
    printf("Votre trajet fait %i km\n", (int)distance(A.latitude, A.longitude, B.latitude, B.longitude));
    printf("Votre voiture a une autonomie de %i km. \n \n", (int)autonomy);
    printf("Le chemin à suivre sera:\n\n");
    printf("    Départ: [%lf,%lf]\n", A.latitude, A.longitude);
    int test = 0;
    int i = 1;
    while (test == 0 && i < 30) {
        double dist = distance(A.latitude, A.longitude, B.latitude, B.longitude);
        double distMax = distance_before_battery_limit(batteryLevel, batteryMin, capacity, efficiency);
        if (dist < distMax) {
            printf("    Arrivée: [%lf,%lf]\n", B.latitude, B.longitude);
            test = 1;
        }
        else {
            coordinates C = point_on_line(A, B, distMax);
            station* stationC = find_nearest_station(list_station, C);
            double StationPower = stationC->power;
            if (StationPower >= 1000) {
                StationPower = StationPower / 1000;
            }
            batteryLevel = battery_level(batteryLevel, distMax, autonomy, efficiency);
            double waitTime = waiting_time(StationPower, batteryLevel, capacity);
            if (waitTime <= timeMax) { // Vérifie si le temps d'attente est inférieur ou égal au temps d'attente maximum autorisé
                C = stationC->coords;
                batteryLevel = 100;
                printf("    Rechargement %i à la station [%lf,%lf], pendant %i minutes et puissance de la station (%i W), niveau de la batterie: %i%%\n", i, C.latitude, C.longitude, (int)waitTime, stationC->power, (int)batteryLevel);
    
                A = C;
                i++;
            }
            else {
                C = stationC->coords;
                batteryLevel = ((timeMax* StationPower*1000) + (batteryLevel * capacity))/capacity;
                printf("    Rechargement %i à la station [%lf,%lf], pendant %i minutes et puissance de la station (%i W), niveau de la batterie: %i%%\n", i, C.latitude, C.longitude, (int)timeMax, stationC->power, (int)batteryLevel);
                A = C;
                i++;
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Itinéaire qui affiche toute les x minutes la position de la voiture
void print_itinerary_by_time(carre ** tab_station, coordinates A, coordinates B,  car* voiture, double batteryLevel, double batteryMin, int time, int timeMax){
    double autonomy = voiture->range;
    double efficiency = voiture->efficiency;
    double capacity = autonomy * efficiency;
    int tempsEcoule= 0;
    double distanceParcouru = 0;
    printf("Voici la position de votre véhicule toutes les %i minutes:\n \n",time);
    printf("    Départ: [%lf,%lf], Temps écoulé depuis départ: %i minutes\n", A.latitude, A.longitude, tempsEcoule);
    int test = 0;
    int i = 1;
    coordinates C;
    //on calcule la distance que l'on parcourt en "time" minutes
    double distTemps = distance_in_time(time);
    

    while (test== 0 && i<30){
        double distArrive = distance(A.latitude, A.longitude, B.latitude, B.longitude);
        if (distArrive < distTemps && battery_level(batteryLevel ,distArrive, autonomy, efficiency)>batteryMin){
            tempsEcoule = tempsEcoule + time;
            distanceParcouru = distanceParcouru + distArrive;
            printf("\n    Vous etes arrivée à votre destination : [%lf,%lf]\n", B.latitude,  B.longitude);
            test = 1;
        }
        else{
             
            if (battery_level(batteryLevel ,distTemps, autonomy, efficiency)>batteryMin){
                //cas où il y a encore assez de batterie pour parcourir la distance en time minute jusqu'au prochain point
                batteryLevel = battery_level(batteryLevel ,distTemps, autonomy, efficiency);
                C = point_on_line(A, B, distTemps);
                tempsEcoule = tempsEcoule + time;
                distanceParcouru = distanceParcouru + distTemps;
                printf("    La voiture se trouve à ces coordonnées: [%lf,%lf] avec %i %% de batterie, Temps écoulé depuis départ: %i minutes, Distance parcouru: %i km\n", C.latitude, C.longitude, (int)batteryLevel, tempsEcoule, (int)distanceParcouru);
            
            }
            else {
                //on fait le plein tout de suite si on peut pas atteindre le prochain point
                station* stationC = find_nearest_station(tab_station, C); //renvoie la station la plus proche du point Ca 
                double StationPower = stationC->power; //à modifier avec la puissance de la station quand la fonction find_nearest_station sera modifiée
                //On verifie que la puissance soit bien en kW:
                if (StationPower >= 1000){
                    StationPower = StationPower/1000;
                }
                double waitTime = waiting_time(StationPower, batteryLevel, capacity);
                if (waitTime< timeMax){
                    C = stationC->coords;
                    printf("\n    Il faut recharger la voiture à la station [%lf,%lf], pendant %i minutes, puissance station (%i W)\n", C.latitude, C.longitude, (int)waitTime, stationC->power);
                    int cycle = (int) waitTime/time;
                    for (int k =0; k< cycle; k++ ){
                        tempsEcoule = tempsEcoule + time;
                        printf("        Rechargement en cours....  \n");
                    }
                    printf("    Batterie rechargée!  Temps écoulé depuis départ: %i minutes\n \n", tempsEcoule);
                    batteryLevel = 100; //on a rechargé la voiture
                }
                else{
                    //on ne peut pas recharger la voiture à cette station car le temps d'attente est trop long
                    //On recharge la voiture en timeMax et on repart avec la batterie par entirement chargée:
                    C = stationC->coords;
                    printf("\n    Il faut recharger la voiture à la station [%lf,%lf], pendant %i minutes , puissance station (%i W)\n", C.latitude, C.longitude, timeMax, stationC->power);
                    int cycle = (int) timeMax/time;
                    for (int k =0; k< cycle; k++ ){
                        tempsEcoule = tempsEcoule + time;
                        printf("        Rechargement en cours....  \n");
                    }
                    //On calcule le pourcentage de batterie rechargé en timeMax minutes:
                    batteryLevel = ((timeMax* StationPower*1000) + (batteryLevel * capacity))/capacity;
                    printf("    Batterie rechargée à %i %%, temps écoulé depuis départ: %i minutes\n \n", (int)batteryLevel, tempsEcoule);
                }
            }
            
            
            
            A = C;
            i++;
            

        }
    }
    printf("\nVotre trajet aura durée %i minutes et vous aurez parcouru: %i km! \n", tempsEcoule, (int)distanceParcouru);
    
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Version 2.0 de l'itinéraire:
    //On demande à l'utilisateur de rentrer la ville de départ et d'arrivée, la voiture et renvoie l'itinéraire à suivre
void itinerary(){
    //On char le fichiers des voitures
    car ** list_of_car = load_cars();
    //on charge le fichier des stations
    carre** list_of_station = load_stations();

    // Demande à l'utilisateur d'entrer le nom de la ville de départ
    char city_name_A[100];
    printf("Entrez le nom de la ville de départ: ");
    fgets(city_name_A, 100, stdin);
    city_name_A[strcspn(city_name_A, "\n")] = 0; // Supprime le retour à la ligne
    coordinates coordsA = find_city(city_name_A);
    if (coordsA.latitude == 0 && coordsA.longitude == 0){
        printf("La ville de départ n'est pas dans la base de données\n");
        //on detruit la liste des voitures
        destroy_cars(list_of_car);
        //on detruit la liste des stations
        destroy_stations(list_of_station);
        return;
    }
    
    // Demande à l'utilisateur d'entrer le nom de la ville d'arrivée
    char city_name_B[100];
    printf("Entrez le nom de la ville d'arrivée: ");
    fgets(city_name_B, 100, stdin);
    city_name_B[strcspn(city_name_B, "\n")] = 0; // Supprime le retour à la ligne

    coordinates coordsB = find_city(city_name_B);
    if (coordsB.latitude == 0 && coordsB.longitude == 0){
        printf("La ville d'arrivée n'est pas dans la base de données\n");
        //on detruit la liste des voitures
        destroy_cars(list_of_car);
        //on detruit la liste des stations
        destroy_stations(list_of_station);
        return;
    }


    // Demande à l'utilisateur d'entrer le modèle de la voiture
    char model[100];
    printf("Entrez le modèle de votre voiture: ");
    fgets(model, 100, stdin);
    model[strcspn(model, "\n")] = 0; // Supprime le retour à la ligne
    int id = find_car_by_model(model, list_of_car);
    if (id == -1){
        printf("Le modèle de voiture n'est pas dans la base de données\n");
        destroy_cars(list_of_car);
        destroy_stations(list_of_station);
        return;
    }
    
    

    // Demande la charge de la batterie actuelle
    double charge;
    printf("Entrez le niveau de charge de votre batterie (en %%): ");
    scanf("%lf", &charge);
    if (charge < 0 || charge > 100){
        printf("Le niveau de charge de la batterie doit être compris entre 0 et 100\n");
        destroy_cars(list_of_car);
        destroy_stations(list_of_station);
        return;
    }
    
    // Demande le niveau de batterie min voulu
    double batteryMin;
    printf("Sous quel niveau de batterie ne voulez vous pas passer en dessous (en %%)? ");
    scanf("%lf", &batteryMin);
    if (batteryMin < 0 || batteryMin > 100){
        printf("Le niveau de batterie min doit être compris entre 0 et 100\n");
        destroy_cars(list_of_car);
        destroy_stations(list_of_station);
        return;
    }

    // Demande temps d'actualisation 
    double time;
    printf("Actualiser la position des voiture toute les x minutes (0 si non voulu): ");
    scanf("%lf", &time);
    if (time < 0){
        printf("Le temps d'actualisation doit être positif\n");
        destroy_cars(list_of_car);
        destroy_stations(list_of_station);
        return;
    }
    
    // Demande le temps d'attendre max à une station
    double timeMax;
    printf("Combien de temps maximum voulez vous attendre à une station (en minutes)? ");
    scanf("%lf", &timeMax);
    if (timeMax < 0){
        printf("Le temps d'attente max doit être positif\n");
        destroy_cars(list_of_car);
        destroy_stations(list_of_station);
        return;
    }
 
   
    printf("\n");
    //print_itinerary(list_of_station, coordsA, coordsB, list_of_car[id], charge, batteryMin);
    if (charge < batteryMin){
        printf("Vous ne pouvez pas partir avec une batterie inférieure à la batterie min\n");
        destroy_cars(list_of_car);
        destroy_stations(list_of_station);
        return;
    }
    if (time == 0){
        print_itinerary(list_of_station, coordsA, coordsB, list_of_car[id], charge, batteryMin, timeMax);
    }
    else{
        print_itinerary_by_time(list_of_station, coordsA, coordsB, list_of_car[id], charge, batteryMin, time, timeMax);
    }
    printf("\n");

    //on detruit la liste des voitures
    destroy_cars(list_of_car);
    //on detruit la liste des stations
    destroy_stations(list_of_station);
}
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/



/*
int main() {
    //itinerary();
    
}
*/
