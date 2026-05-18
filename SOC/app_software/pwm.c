#include <stdio.h>
#include "system.h"
#include "sys/alt_alarm.h"
#include "altera_up_avalon_parallel_port.h"

/* PWM en nombre de ticks HAL (pas en microsecondes). */
#define PERIOD_TP 1000
#define ACTIVE_TA 300

static alt_alarm pwm_alarm;
static alt_up_parallel_port_dev* led_port;

alt_u32 pwm_callback(void* context)
{
	static int state = 0;
	(void)context;

	if (state == 0)
	{
		/* Transition BAS -> HAUT */
		alt_up_parallel_port_write_data(led_port, 0x01);
		state = 1;
		return (alt_u32)ACTIVE_TA;
	}
	else
	{
		/* Transition HAUT -> BAS */
		alt_up_parallel_port_write_data(led_port, 0x00);
		state = 0;
		return (alt_u32)(PERIOD_TP - ACTIVE_TA);
	}
}

int main(void)
{
	led_port = alt_up_parallel_port_open_dev(GREEN_LEDS_NAME);
	if (led_port == NULL)
	{
		printf("Erreur: impossible d'ouvrir %s\n", GREEN_LEDS_NAME);
		return -1;
	}

	if (alt_alarm_start(&pwm_alarm, 10, pwm_callback, NULL) < 0)
	{
		printf("Erreur: alt_alarm_start a echoue. Verifie ALT_SYS_CLK (actuellement none).\n");
		return -1;
	}

	while (1)
	{
	}

	return 0;
}
