#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "simu.h"

int main(void){
    int taille_simulation;
    printf("Entrez le nombre d'entités voulu dans la simulation : ");
    scanf("%i", &taille_simulation);
    AutoPath** AutoPath = initAutoPath_simu(taille_simulation);

    clock_t depart = clock();
    travelAutopath_simu(AutoPath, taille_simulation);
    clock_t fin = clock();

    long int time = (fin - depart) / CLOCKS_PER_SEC;
    
    //print_AutoPath_simu(AutoPath, taille_simulation)

    destroy_AutoPath_simu(AutoPath, taille_simulation);

    printf("\nTemps de chagement : %ldmin%lds\n", time/60, time-((time/60)*60));
}

