#include "sys/alt_timestamp.h"
#include <sys/alt_alarm.h>
#include <stdio.h>

alt_alarm ma_structure_alarme;
static unsigned int dernier_timestamp = 0;
static int premiere_mesure = 1;

unsigned int ma_fonction_callback(void* context)
{
    unsigned int timestamp_courant;
    unsigned int delta_ticks;
    float dt;

    (void)context;

    timestamp_courant = alt_timestamp();

    if (premiere_mesure)
    {
        premiere_mesure = 0;
        printf("Premiere mesure periodique recue.\n");
    }
    else
    {
        delta_ticks = timestamp_courant - dernier_timestamp;
        dt = (float)delta_ticks / (float)alt_timestamp_freq();
        printf("Mesure periodique: %u ticks, dt = %.6f s\n", (unsigned int)delta_ticks, dt);
    }

    dernier_timestamp = timestamp_courant;
    return alt_ticks_per_second();
}

int main(void)
{
    // Initialisation du compteur timestamp
    if (alt_timestamp_start() < 0) {
        printf("Erreur: Pas de timer de type 'timestamp' consulter BSP \n");
        return -1;
    }

    // Initialisation de la reference de temps pour la premiere mesure
    dernier_timestamp = alt_timestamp();

    if (alt_alarm_start(&ma_structure_alarme,
                        2 * alt_ticks_per_second(),
                        ma_fonction_callback,
                        NULL) < 0)
    {
        printf("Erreur : Pas de timer systeme configure pour alt_alarm.\n");
        return -1;
    }

    while (1)
    {
    }

    return 0;
}
