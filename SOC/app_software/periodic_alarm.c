#include <sys/alt_alarm.h>
#include <stdio.h>

alt_alarm ma_structure_alarme;

alt_u32 ma_fonction_callback(void* context)
{
    (void)context;
    printf("Alarme declenchee !\n");
    return alt_ticks_per_second();
}

int main(void)
{
    if (alt_alarm_start(&ma_structure_alarme,
                        2 * alt_ticks_per_second(),
                        ma_fonction_callback,
                        NULL) < 0)
    {
        printf("Erreur : Pas de timer systeme configure.\n");
    }

    while (1)
    {
    }

    return 0;
}
