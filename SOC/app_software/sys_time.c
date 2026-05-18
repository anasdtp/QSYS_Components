#include <stdio.h>
#include <sys/time.h> // Pour struct timeval et settimeofday
#include <alt_types.h> // Types spécifiques Altera
#include <unistd.h> // Pour sleep()
int main(void) 
{ 
    struct timeval now; 
    struct timeval tv_verif;
    int rc;
    printf("--- Configuration de l'heure système (Nios II) ---\n");
    /* Initialisation timeval : * 4jours= 4*86400s,*23heures=23*3600s*58minutes=58*60s,*50s*/
    now.tv_sec = (4 * 86400) + (23 * 3600) + (58 * 60) + 50; 
    now.tv_usec = 0; 
    rc = settimeofday(&now, NULL); 
    if (rc == 0) {
    
    } else {
        printf("Erreur : settimeofday a echoue (verifiez votre BSP).\n");
        return -1;
    }
    /* On affiche l'heure qui s'écoule */
    while(1) {
        // Récupération de l'heure actuelle mise à jour par l'Interval Timer
        gettimeofday(&tv_verif, NULL);
        printf("Temps actuel : %ld secondes (%ld jours passes)\n", 
        tv_verif.tv_sec, tv_verif.tv_sec / 86400);
        sleep(1); // Pause de 1 seconde (utilise les ticks de l'Interval Timer)
    } 
    return 0;
}