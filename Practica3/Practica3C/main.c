#include <stdio.h>
#include "44b.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "gpio.h"
#include "timer.h"
#include "intcontroller.h"

#define BUT1 0x1
#define BUT2 0x2

void irq_ISR(void) __attribute__ ((interrupt ("IRQ")));

struct RLstat {
	int moving;
	int direction;
	int position;
};

static struct RLstat RL = {
	.moving = 0,
	.direction = 1,
	.position = 0,
};

void timer_ISR(void)
{
	//COMPLETAR: cada vez que el TIMER0 interrumpe el led rotante se mueve
	// si el giro es horario position se incrementa si su valor es <5 si no escribe un 0
	// si el giro es antihorario position se decrementa si su valor es >0 si no se escribe un 5
	if(RL.moving == 1)
	{
		RL.position += RL.direction;
		if (RL.position > 5) RL.position = 0;
		if (RL.position < 0) RL.position = 5;
		D8Led_segment(RL.position);
	}
}

void button_ISR(void)
{
	unsigned int whichint = rEXTINTPND;
	unsigned int buttons = (whichint >> 2) & 0x3;

	//COMPLETAR: BUT1 cambia el estado de LED1 y cambia dirección de
	//movimiento del led rotante

	if (buttons & BUT1) {
		led1_switch();
		if (RL.direction == 1) RL.direction = -1;
		else RL.direction = 1;
		//Actualizamos y reiniciamos el timer
		tmr_update(TIMER0);
		tmr_start(TIMER0);
	}


	//COMPLETAR: BUT2 cambia el estado de LED2
	//activa movimiento de led rotante si parado
	//o lo para si está en movimiento (actuar sobre rutinas del TIMER)
	if (buttons & BUT2) {
		
		led2_switch();
		if (RL.moving == 0) RL.moving = 1;
		else RL.moving = 0;

		//Actualizamos y reiniciamos el timer
		tmr_update(TIMER0);
		tmr_start(TIMER0);
	}

		// eliminamos rebotes
	Delay(2000);

	//COMPLETAR: debemos borrar las peticiones de interrupción correspondientes a los pulsadores en rEXTINTPND
	rEXTINTPND |= buttons;//revisar

}

int setup(void)
{
	leds_init();
	D8Led_init();
	D8Led_segment(RL.position);
	pISR_IRQ = (unsigned) irq_ISR;
	/* Port G: configuración para espera activa */
	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//hay que configurar los pines 6 y 7 del puerto G como pines de entrada y
	//activar las correspondientes resistencias de pull-up

	portG_conf(6, EINT);
	portG_eint_trig(6, FALLING);
	portG_conf_pup(6, ENABLE);
	portG_conf(7, EINT)
	;portG_eint_trig(7,FALLING);
	portG_conf_pup(7, ENABLE);

	/*******************************************/
    /* Configuracion del timer */

	//COMPLETAR: utilizando el interfaz para los timers definido en timer.h
	//		configurar el timer 0:
	//      valor de prescalado a 255
	//      valor del divisor 1/8
	//      valor de cuenta 62500 y cualquier valor de comparacion entre 1 y 62499
	//      actualizar el contador con estos valores (update)
	//      poner el contador en modo RELOAD
	//      dejar el contador parado

	tmr_set_prescaler(TIMER0, 255);
	tmr_set_divider(TIMER0, D1_8);
	tmr_set_count(TIMER0, 62500, 2000);//para tener el mismo delay que la  version previa
	tmr_update(TIMER0);
	tmr_set_mode(TIMER0, RELOAD);
	tmr_stop(TIMER0);



if (RL.moving)
{
		tmr_start(TIMER0);
}
	/***************************/

	/* Configuración del controlador de interrupciones */

	ic_init();
	//COMPLETAR: utilizando el interfaz definido en intcontroller.h
	//		habilitar la línea IRQ en modo no vectorizado
	//		deshabilitar la línea FIQ
	//		configurar la línea INT_TIMER0 en modo IRQ
	//		configurar la línea INT_EINT4567 en modo IRQ
	//		habilitar la línea INT_TIMER0
	//		habilitar la línea INT_EINT4567

	ic_conf_irq(ENABLE,NOVEC);
	ic_conf_fiq(DISABLE);
	ic_conf_line(INT_TIMER0, IRQ);
	ic_conf_line(INT_EINT4567, IRQ);
	ic_enable(INT_TIMER0);
	ic_enable(INT_EINT4567);
	/***************************************************/


	Delay(0);
	return 0;
}

void irq_ISR(void)
{
	int bit = rI_ISPR;
	if (bit == 0x02000)
	{
		timer_ISR();
		ic_cleanflag(INT_TIMER0);
	}
	if (bit == 0x0200000)
	{
		button_ISR();
		ic_cleanflag(INT_EINT4567);
	}
}


int main(void)
{
	setup();

	while (1) {
	}
}
