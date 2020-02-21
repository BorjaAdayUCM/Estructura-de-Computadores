#include <stdio.h>
#include "44b.h"
#include "button.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "gpio.h"

struct RLstat {
	int moving;
	int speed;
	int iter;
	int direction;
	int position;
};

static struct RLstat RL = {
	.moving = 0,
	.speed = 5,
	.iter = 0,
	.direction = 0,
	.position = 0,
};

int setup(void)
{
	leds_init();
	D8Led_init();
	D8Led_segment(RL.position);

	/* Port G: configuración para espera activa */
	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//hay que configurar los pines 6 y 7 del puerto G como pines de entrada y
	//activar las correspondientes resistencias de pull-up

	portG_conf(BIT_BUTTON1, INPUT);
	//portG_eint_trig(BIT_BUTTON1, FALLING);
	portG_conf_pup(BIT_BUTTON1, ENABLE);
	portG_conf(BIT_BUTTON2, INPUT);
	//portG_eint_trig(7,FALLING);
	portG_conf_pup(BIT_BUTTON2, ENABLE);

	/*******************************************/

	Delay(0);
	return 0;
}

int loop(void)
{
	unsigned int buttons = read_button();

	if (buttons & BUT1) {
		// COMPLETAR: utilizando la interfaz para los leds definida en leds.h
		// hay que conmutar el led1, si esta encendido apagarlo y viceversa
		// También hay que comutar la dirección del movimiento del led rotante
		// representado por el campo direction de la variable RL
		led1_switch();
		if (RL.direction == 1) RL.direction = 0;
		else RL.direction = 1;
	}

	if (buttons & BUT2) {
		// COMPLETAR: utilizando la interfaz para los leds definida en leds.h
		// hay que conmutar el led2
		// También hay que comutar el estado de movimiento del led rotante
		// representado por el campo moving de la variable RL, y en caso de
		// ponerlo en marcha debemos reiniciar el campo iter al valor del campo speed.
		led2_switch();
		if (RL.moving == 1) RL.moving = 0;
		else
		{
			RL.moving = 1;
			RL.iter = RL.speed;
		}
	}

	if (RL.moving) {

		RL.iter--;
		if (RL.iter == 0) {
		// COMPLETAR: debemos hacer avanzar el led rotante una posición en
		// la dirección indicada por el campo direction de la variable RL.
	    // La posición actual está representada en el campo position.
	   // Recordar que queremos un movimiento circular, representado por
	   // las 6 primeras posiciones en el array Segmentes del display de 8
	   // segmentos, por lo que position debe estar siempre entre 0 y 5.
		RL.iter = RL.speed;
		if (RL.direction == 0) RL.position -= 1;
		else RL.position += 1;
		if (RL.position > 5) RL.position = 0;
		if (RL.position < 0) RL.position = 5;
		}
		D8Led_segment(RL.position);
	}

	Delay(2000); // espera de 200ms para que el bucle se repita 5 veces por segundo
	return 0;
}


int main(void)
{
	setup();

	while (1) {
		loop();
	}
}
