#ifndef __SIMU_H__
#define __SIMU_H__

#include "en_tete.h"

//FONCTIONS
//fonction qui initialise la simulation
extern AutoPath** initAutoPath_simu(int taille_simulation);

//fonction qui va simuler les trajets
extern carre** travelAutopath_simu(AutoPath** AutoPath, int taille_simulation);

//destruction du tableau de voiture
extern void destroy_cars_simu(car** cars);

//destruction tableau villes
extern void destroy_cities_simu(city** cities);

//destruction tableau entités
extern void destroy_AutoPath_simu(AutoPath** AutoPath, int taille_simulation);

//Fonction qui charge la liste des voitures:
extern car** load_cars_simu();

//fonction qui récupère les villes
extern city** load_cities_simu();

//fonction pour afficher des coordonnées
extern void print_coordinates_simu(coordinates coordinates);

//fonction pour afficher l'état
extern void print_etat_simu(conditions etat);

//fonction de tri des stations
extern carre** load_stations_simu();

//fonction qui donne le point à une certaine distance de A et situé sur la droite AB
extern coordinates point_on_line_simu(coordinates A, coordinates B, double distance);

//fonction donnant la distance en km entre deux points
extern double distance_simu(coordinates A, coordinates B);

//donne le temps de rechargement pour aller de batterie à 100%
extern int temps_recharge_simu(int batterie, int puissance, int capacite);

//trouve la staion la plus proche dans le même carré
extern station* find_nearest_station_simu(carre** tab, coordinates coords);

//fonction auxiliaire -> on l'utilise pour la récursivité de find_nearest_station
extern station* find_nearest_station_aux_simu(station* S, double distance_mini, coordinates coords, station* nearest_station);

//affichage des trajets
extern void print_AutoPath_simu(AutoPath** AutoPath, int taille_simulation);

//donne le carré en fonction des coordonnées
extern int coordtocarre_simu(coordinates coords);

//ajoute une station à la liste correspondant au carré donné
extern void station_append_simu(carre* carre, int power, coordinates coords, int nb_prises, char* identifiant);

//fonction auxiliaire -> on l'utilise pour la récursivité de station_append
extern void station_append_aux_simu(station* station, int power, coordinates coords, int nb_prises, char* identifiant);

//fonction qui affiche les stations débordées
void print_etat_station_simu(station* S, int duree_simulation, int debut_simulation);

//destruction du tableau de listes de stations
extern void destroy_tab_carre_simu(carre** tab);
extern void destroy_carre_simu(carre* carre);
extern void destroy_station_simu(station* station);

#endif
