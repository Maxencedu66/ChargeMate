#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "itinerary.h" // Remplacez "your_header_file" par le nom de votre fichier d'en-tête

void test_distance() {
    //coordonnées de Paris:
    double lat1 = 48.8566;
    double lon1 = 2.3522;
    //coordonnées de Marseille:
    double lat2 = 43.2965;
    double lon2 = 5.3698;

    double dist = distance(lat1, lon1, lat2, lon2);

    assert(fabs(dist - 660.71) < 1); // La distance entre Paris et Marseille est d'environ 660,71 km (https://fr.distance.to/Paris/Marseille). On autorise une erreur de 1 km.
}



void test_coordtocarre() {
    double lat_paris = 48.8566;
    double lon_paris = 2.3522;
    int expected_indice_paris = 168; //indice_carre = index_latitude * 16 + index_longitude donc indice_carre = 10 * 16 + 8 =168
    int actual_indice_paris = coordtocarre(lat_paris, lon_paris);
    assert(expected_indice_paris == actual_indice_paris);

    double latMars = 43.2965;
    double lonMars = 5.3698;
    int expected_indice_mars = 60; //indice_carre = index_latitude * 16 + index_longitude donc indice_carre = 3 * 16 + 12 =60 
    int actual_indice_mars = coordtocarre(latMars, lonMars);
    assert(expected_indice_mars == actual_indice_mars);
}

void test_load_stations() {
    carre **tab_of_carre = load_stations();
    assert(tab_of_carre != NULL);
    assert(tab_of_carre[154]->nb_stations > 0); //car dans le carre de Paris
    assert(tab_of_carre[0]->nb_stations ==0); //car dans l'océan

    for (int i = 0; i < NB_CARRES; i++) {
        station *current = tab_of_carre[i]->head;
        station *temp;
        while (current != NULL) {
            temp = current->next;
            free(current);
            current = temp;
        }
        free(tab_of_carre[i]);
    }
    free(tab_of_carre);

}


void test_station_append() {
    carre *c = malloc(sizeof(carre));
    c->head = NULL;
    c->nb_stations = 0;

    coordinates coords1 = {48.8566, 2.3522};
    station_append(c, 10000, coords1);
    assert(c->nb_stations == 1);
    assert(c->head != NULL);
    assert(c->head->power == 10000);
    assert(c->head->coords.latitude == coords1.latitude);
    assert(c->head->coords.longitude == coords1.longitude);

    free(c->head);
    free(c);

}

void test_find_city() {
    // Test avec une ville existante
    char city1[] = "Paris";
    coordinates coords1 = find_city(city1);
    assert(fabs(coords1.latitude - 48.8566) <= 0.1 );
    assert(fabs(coords1.longitude - 2.3522) <= 0.1);

    // Test avec une ville inexistante
    char city2[] = "Nonexistent";
    // On utilise la fonction assert avec la macro NULL pour vérifier que find_city renvoie NULL
    coordinates coords2 = find_city(city2);
    assert(coords2.latitude == 0);
    assert(coords2.longitude == 0);
}

void test_find_nearest_station() {
    // Création des stations de test
    station* station1 = malloc(sizeof(station));
    station1->coords.latitude = 48.8566;
    station1->coords.longitude = 2.3522;
    station1->next = NULL;

    station* station2 = malloc(sizeof(station));
    station2->coords.latitude = 43.2965;
    station2->coords.longitude = 5.3698;
    station2->next = NULL;

    // Création du tableau de carrés
    carre** tab_of_carre = malloc(NB_CARRES * sizeof(carre*));
    for (int i = 0; i < NB_CARRES; i++) {
        tab_of_carre[i] = malloc(sizeof(carre));
        tab_of_carre[i]->head = NULL;
        tab_of_carre[i]->nb_stations = 0;
    }

    // Ajout des stations dans les carrés correspondants
    tab_of_carre[168]->head = station1;
    tab_of_carre[168]->nb_stations = 1;

    tab_of_carre[60]->head = station2;
    tab_of_carre[60]->nb_stations = 1;

    // Coordonnées de la ville de test
    coordinates coords = {48.8566, 2.3522};

    // Appel de la fonction find_nearest_station pour trouver la station la plus proche
    station* nearest_station = find_nearest_station(tab_of_carre, coords);

    // Vérification des résultats
    assert(nearest_station != NULL);
    assert(nearest_station->coords.latitude == coords.latitude);
    assert(nearest_station->coords.longitude == coords.longitude);

    // Libération de la mémoire
    free(station1);
    free(station2);

    for (int i = 0; i < NB_CARRES; i++) {
        free(tab_of_carre[i]);
    }
    free(tab_of_carre);
}

void test_point_on_line() {
    // Coordonnées des points A et B
    coordinates A = {48.8566, 2.3522};
    coordinates B = {43.2965, 5.3698};

    // Distance depuis A pour obtenir le point C
    double dist = 100.0;

    // Appel de la fonction point_on_line pour obtenir les coordonnées du point C
    coordinates C = point_on_line(A, B, dist);

    // Coordonnées attendues du point C (Montargis )
    double expected_latC = 47.997290;
    double expected_lonC = 2.736291;

    // Vérification des résultats
    assert(fabs(C.latitude - expected_latC) < 0.1);
    assert(fabs(C.longitude - expected_lonC) < 0.1);
}


void test_load_cars() {
    // Appel de la fonction load_cars pour charger la liste des voitures
    car** cars = load_cars();

    // Vérification du premier élément de la liste
    assert(cars[0]->id == 0);
    assert(strcmp(cars[0]->modelName, "Tesla Model Y Long Range Dual Motor") == 0);
    assert(cars[0]->range == 435);
    assert(cars[0]->efficiency == 172);

    // Vérification du dernier élément de la liste
    assert(cars[298]->id == 298);
    assert(strcmp(cars[298]->modelName, "Citroen e-SpaceTourer XL 50 kWh") == 0);
    assert(cars[298]->range == 175);
    assert(cars[298]->efficiency == 257);

    // Libération de la mémoire allouée pour les voitures
    for (int i = 0; i < 299; i++) {
        free(cars[i]->modelName);
        free(cars[i]);
    }
    free(cars);
}

void test_find_car_by_model() {
    // Chargement de la liste des voitures
    car** cars = load_cars();

    // Recherche de la première voiture de la liste
    int index = find_car_by_model("Tesla Model Y Long Range Dual Motor", cars);
    assert(index == 0);
    assert(strcmp(cars[index]->modelName, "Tesla Model Y Long Range Dual Motor") == 0);


    // Recherche de la dernière voiture de la liste
    index = find_car_by_model("Citroen e-SpaceTourer XL 50 kWh", cars);
    assert(index == 298);
    assert(strcmp(cars[index]->modelName, "Citroen e-SpaceTourer XL 50 kWh") == 0);

    // Recherche d'une voiture inexistante
    int mauvaisIndex = find_car_by_model("Ford Mustang", cars);
    assert(mauvaisIndex == -1);

    // Libération de la mémoire allouée pour les voitures
    destroy_cars(cars);
}

void test_battery_level() {
    // Calcul du niveau de batterie avec une batterie à 100% et la distance maximale
    double batteryLevel = 100.0;
    double distance = 400.0;
    double range = 400.0;
    double efficiency = 0.2; //en kWh/km

    double result = battery_level(batteryLevel, distance, range, efficiency);
    assert(fabs(result - 0.0) < 0.01); // On autorise une marge d'erreur de 0.01
    // La batterie doit être à 0% car elle est entièrement déchargée après le trajet maximal

    // Calcul du niveau de batterie avec une batterie à 100% et une distance de 0 km
    batteryLevel = 100.0;
    distance = 0.0;
    range = 400.0;
    efficiency = 0.2;

    result = battery_level(batteryLevel, distance, range, efficiency);
    assert(fabs(result - 100.0) < 0.01); 
    // La batterie doit rester à 100% car aucune distance n'a été parcourue

    // Calcul du niveau de batterie avec une batterie à 100% et une distance de la moitié de la range
    batteryLevel = 100.0;
    distance = 200.0;
    range = 400.0;
    efficiency = 0.2;

    result = battery_level(batteryLevel, distance, range, efficiency);
    assert(fabs(result - 50.0) < 0.01); 
    // La batterie doit être à 50% après avoir parcouru  la moitié de l'autonomie

}

void test_waiting_time() {
    double powerStation = 250.0; // Puissance de recharge de la station (en kW)
    double batteryLevel = 50.0; // Niveau de la batterie à 100% (en %)
    double capacity = 435.0 * 172.0 ; // Capacité de la batterie (en Wh) = autonomie (en km) * efficacité (en Wh/km)
    double expectedTime = ((capacity*0.001 * ((100 - batteryLevel) / 100)) / powerStation) * 60.0; // Temps d'attente attendu (en minutes)
    double actualTime = waiting_time(powerStation, batteryLevel, capacity);
    assert(fabs(actualTime - expectedTime) < 0.01); // On autorise une marge d'erreur de 0.01 minute
}

void test_distance_before_battery_limit() {
    double batteryLevel = 80.0; // Niveau de la batterie à 80% (en %)
    double batteryMin = 20.0; // Charge minimale de la batterie à atteindre (en %)
    double capacity = 435.0 * 172.0; // Capacité de la batterie (en Wh) = autonomie (en km) * efficacité (en Wh/km)
    double efficiency = 172.0; // Efficacité de la voiture (en Wh/km)
    
    double expectedDistance = ((capacity * (batteryLevel / 100.0)) / efficiency) - ((capacity * (batteryMin / 100.0)) / efficiency);
    double actualDistance = distance_before_battery_limit(batteryLevel, batteryMin, capacity, efficiency);
    
    assert(fabs(actualDistance - expectedDistance) < 0.01); // On autorise une marge d'erreur de 0.01 km
}



int main () {

    /*
    test_distance();
    test_coordtocarre();
    test_load_stations();
    test_station_append();
    test_find_city();
    test_find_nearest_station();
    test_point_on_line();
    test_load_cars();
    test_find_car_by_model();
    test_battery_level();
    test_waiting_time();
    test_distance_before_battery_limit();
    printf("Tests passed successfully\n");
    */

    //Fonction principale:
    itinerary();

    return 0;
}
