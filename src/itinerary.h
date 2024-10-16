/**********************************************************************/
/*** en-tete de itinerary.c ***/
/**********************************************************************/

#ifndef ITINERARY_H
#define ITINERARY_H

#include "en_tete.h"

//fonction de tri des stations
carre** load_stations();

// coord => carre
int coordtocarre(double lat, double lon);

//Fonction qui convertit les degres en radians
double degtoradians(double deg);

//Fonction qui calcule la distance en kilomètres entre deux points géographique
double distance(double lat1,double lon1,double lat2,double lon2);

//Fonction qui recherche les coordonnées d'une ville
coordinates find_city(char* city_name);

//trouve la staion la plus proche dans le même carré
station* find_nearest_station(carre** tab, coordinates coords);

//fonction auxiliaire -> on l'utilise pour la récursivité de find_nearest_station
station* find_nearest_station_aux(station* S, double distance_mini, coordinates coords, station* nearest_station);

//ajoute une station à la liste correspondant au carré donné
void station_append(carre* carre, int power, coordinates coords);

//fonction auxiliaire -> on l'utilise pour la récursivité de station_append
void station_append_aux(station* station, int power, coordinates coords);


//Fonction qui donne un point situé à une distance donnée sur la droite AB
coordinates point_on_line(coordinates A, coordinates B, double dist);

//Fonction qui affiche le trajet
void print_itinerary(carre** list_station,coordinates A, coordinates B,  car* voiture, double batteryLevel, double batteryMin, int timeMax);

//Fonction qui charge la liste des voitures:
car** load_cars();

//Fonction qui trouve le vehicule dans la liste à partir de sa marque
int find_car_by_model(char model[100], car** listCar );

//Fonction qui calcule le niveau de la batterie à la fin du trajet
double battery_level(double batteryLevel,double distance, double range, double efficiency);

//Fonction qui calcule le temps d'attente à une station pour recharger son véhicule (en minutes)
double waiting_time(double powerStation, double batteryLevel, double capacity);

//Fonction qui calcule la distance que l'on peut parcourir avant d'atteindre la charge minimale de la batterie
double distance_before_battery_limit(double batteryLevel, double batteryMin, double capacity, double efficiency);

//Fonction qui calcule la distance que l'on peut parcourir en "time" minutes (avec une vitesse moyenne de 100km/h)
double distance_in_time(double time);

//Itinéaire qui affiche toute les x minutes la position de la voiture
void print_itinerary_by_time(carre ** tab_station, coordinates A, coordinates B,  car* voiture, double batteryLevel, double batteryMin, int time, int timeMax);

//Version 2.0 de l'itinéraire:
void itinerary();

//destruction du tableau de voiture
void destroy_cars(car** cars);

//destruction du tableau de station
void destroy_stations(carre** stations);



#endif /* ITINERARY_H */
