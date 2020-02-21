#include <stdio.h>
#include "44b.h"
#include "button.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "gpio.h"

struct RLstat {
	int moving;
	int direction;
	int parpadea;
};

static struct RLstat RL = {
	.moving = 0,
	.direction = 1,
	.parpadea = 1,
};

int setup(void)
{
	leds_init();
	D8Led_init();
	/* Port G: configuración para espera activa */
	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//hay que configurar los pines 6 y 7 del puerto G como pines de entrada y
	//activar las correspondientes resistencias de pull-up

	portG_conf(BIT_BUTTON1, INPUT);
	portG_conf_pup(BIT_BUTTON1, ENABLE);
	portG_conf(BIT_BUTTON2, INPUT);
	portG_conf_pup(BIT_BUTTON2, ENABLE);

	/*******************************************/

	Delay(0);
	return 0;
}

int loop(void)
{
	int i = 0, j = 0;
	unsigned int status = 128;
	while ((RL.direction == 1 && i < 16) || (RL.direction == -1 && i >= 0))
	{
		j = 0;
		while (j < 7)
		{
			if (Digits[i] & Segments[j])
			{
				if(RL.moving)
				{
					status |= Segments[j];
					D8Led_enciende(status);
				}
				unsigned int buttons = read_button();
				if (buttons & BUT1)
				{
					led1_switch();
					if (RL.direction == 1) RL.direction = -1;
					else RL.direction = 1;
				}
				if (buttons & BUT2)
				{
					led2_switch();
					if (RL.moving == 1) RL.moving = 0;
					else RL.moving = 1;
				}
				Delay(2000);
			}
			Delay(2000);
			if (RL.moving) j++;
		}
		j = 0;
		while (j < 7)
		{
			if (Digits[i] & Segments[j])
			{
				if(RL.moving)
				{
					status &= ~Segments[j];
					D8Led_enciende(status);
				}
				unsigned int buttons = read_button();
				if (buttons & BUT1)
				{
					led1_switch();
					if (RL.direction == 1) RL.direction = -1;
					else RL.direction = 1;
				}
				if (buttons & BUT2)
				{
					led2_switch();
					if (RL.moving == 1) RL.moving = 0;
					else RL.moving = 1;
				}
				Delay(2000);
			}
			Delay(2000);
			if (RL.moving) j++;
		}
	status = 0;
	i += RL.direction;
	if (RL.direction == 1 && i == 16) i = 0;
	else if (RL.direction == -1 && i == -1) i = 15;
	}
	Delay(2000);
	return 0;
}


int main(void)
{
	setup();

	while (1) {
		loop();
	}
}
