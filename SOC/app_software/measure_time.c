#include "sys/alt_timestamp.h"
#include "alt_types.h"
#include <stdio.h>
#include <unistd.h> // Pour le fonction usleep()

int main(void) {
    alt_u32 t1, t2;
    float dt;
    // Initialisation du compteur (Timestamp)
    if (alt_timestamp_start() < 0) {
    printf("Erreur: Pas de timer de type 'timestamp' consulter BSP \n");
    return -1;
    }
    // Mesure nombre de ticks
    t1 = alt_timestamp();
    usleep(10000); // Simulation fonction
    t2 = alt_timestamp();
    // Calcul du temps en secondes : (Ticks / Frequence)
    // On convertit en float pour obtenir la précision décimale
    dt = (float)(t2 - t1) / (float)alt_timestamp_freq();

    // Affichage des résultats
    printf("Frequence du timer:%uHz\n", (unsigned int)alt_timestamp_freq());
    printf("Ticks f1 = %u \n", (unsigned int)(t2 - t1));
    printf("dt = %.6f s\n", dt);

    return 0;
}
