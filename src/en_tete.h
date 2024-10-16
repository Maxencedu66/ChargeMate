#ifndef __EN_TETE_H__
#define __EN_TETE_H__

//MACROS :
#define NB_VOITURES 299 // nombre de voiture dans le fichier
#define NB_VILLES 35350 // Nombre de villes en France métropolitaine
#define NB_STATIONS 54874 // nombre de station en France (hors Corse et bugs)
#define NB_STATIONS_SIMU 11886 // nombre de stations en France (hors Corse et bugs) et sans les doublons
#define NB_CARRES 256
#define VITMOY 100 // vitesse moyenne en km/h
#define VitesseMoyenne 100 //Vitesse moyenne en km/h
#define DISTMOY VITMOY/6.0 // distance (en km) parcourue en 10 min à une vitesse de VITMOY
#define R 6371 // Rayon moyen de la Terre en kilomètres
#define pi 3.14159265359 // Valeur de pi
#define EFFICACITE_RECHARGEMENT 0.95 // efficacité de rechargement
#define TAILLE_TAB_ETAPES 200 // taille du tableau contenant les étapes
#define NB_HEURES_SIMULATION 12 // durée maximale de la simulation

#define DEGTORAD(omega) (((omega)/180.0)*pi)
#define BATTERIE_RESTANTE(distance, batterie, efficiency, range) ((batterie) - (100*(efficiency)*(distance))/((efficiency)*(range)))
#define TEMPS_RECHARGEMENT(batterie, distance, capacite) (((100-(batterie))*(capacite))/(100*(puissance)*EFFICACITE_RECHARGEMENT))*60
#define MIN(a,b) ((a<b)?a:b)

//STRUCTURES DE DONNÉES
enum conditions_t{ROULE, ARRET, EN_CHARGE};

typedef struct car_t car;
typedef struct AutoPath_t AutoPath;
typedef struct coordinates_t coordinates;
typedef struct city_t city;
typedef enum conditions_t conditions;
typedef struct journey_t journey;
typedef struct station_t station;
typedef struct carre_t carre;

//structure pour les coordonnées
struct coordinates_t{
    double latitude;
    double longitude;
};

//structure pour les voitures
struct car_t{
    int id;
    char* modelName;
    int range; //autonomie en Km
    int efficiency; //consommation en Wh/km
};

//structure contenant les données de chaque entités de la simulation
struct AutoPath_t {
    char* ville_depart;
    char* ville_arrivee;
    car* voiture;
    int batterie_init; // pourcentage de batterie initiale -> on ne le touche pas celui la
    int batterie; // pourcentage de batterie au fur et à mesure du trajet
    int batterie_min; // pourcentage de batterie en dessous duquel on ne veut pas descendre
    journey* trajet;
    int est_arrive; // booléen
    int duree_trajet; // durée du trajet en minutes
    int temps_attente_max; // temps d'attente maximum pour le chargement de la voiture
};

//structure pour les villes
struct city_t{
    char* nom;
    coordinates* coordonnees;
};

//structure du trajet
struct journey_t{
    coordinates* etapes; //tableau de coordonées -> etapes[0] = départ et etapes[TAILLE_TAB_ETAPES - 1] = arrivée
    conditions* etats; //tableau d'états
    int* batterie; //donne la batterie à chaque début début d'étape
    int nb_etapes; //compteur d'étapes = tranches de 10 min entre le départ et l'arrivée
};

//structure d'un carrée -> liste chainée
struct carre_t{
    station* head;
    int nb_stations;
};

//structure d'une station
struct station_t{
    char* id;
    coordinates coords;
    int power;
    int nb_prises;
    int* nb_vehicules; //tableau de taille TAILLE_TAB_ETAPES
    station* next;
};
#endif
