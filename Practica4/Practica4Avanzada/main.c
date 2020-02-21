#include <stdio.h>
#include "44b.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "gpio.h"
#include "timer.h"
#include "intcontroller.h"
#include "keyboard.h"

#define BUT1 0x1
#define BUT2 0x2

void timer0_ISR(void)__attribute__((interrupt ("IRQ")));
void timer1_ISR(void)__attribute__((interrupt ("IRQ")));
void button_ISR(void)__attribute__((interrupt ("IRQ")));
void keyboard_ISR(void)__attribute__((interrupt ("IRQ")));

struct RLstat {
	int direction;
	int numActual;
	int mode;
};

static struct RLstat RL = {
	.direction = 2,
	.numActual = -2,
	.mode = 0
};

void timer0_ISR(void)
{
	//COMPLETAR: cada vez que el TIMER0 interrumpe el led rotante se mueve
	// si el giro es horario position se incrementa si su valor es <5 si no escribe un 0
	// si el giro es antihorario position se decrementa si su valor es >0 si no se escribe un 5
	if (RL.mode == 0)
	{
		RL.numActual += RL.direction;
		if (RL.numActual > 14) RL.numActual = 0;
		if (RL.numActual < 0) RL.numActual = 14;
	}
	else
	{
		RL.numActual += RL.direction;
		if (RL.numActual > 15) RL.numActual = 1;
		if (RL.numActual < 1) RL.numActual = 15;
	}
	D8Led_digit(RL.numActual);
	ic_cleanflag(INT_TIMER0);
}

void timer1_ISR(void)
{
	leds_switch();
	ic_cleanflag(INT_TIMER1);
}

void button_ISR(void)
{
	unsigned int whichint = rEXTINTPND;
	unsigned int buttons = (whichint >> 2) & 0x3;

	//COMPLETAR: BUT1 cambia el estado de LED1 y cambia direcci√≥n de
	//movimiento del led rotante

	if (buttons & BUT1) {
		led1_switch();
		if (RL.direction == 2) RL.direction = -2;
		else RL.direction = 2;
		//Actualizamos y reiniciamos el timer
		//tmr_update(TIMER0);
		//tmr_start(TIMER0);
	}


	//COMPLETAR: BUT2 cambia el estado de LED2
	//activa movimiento de led rotante si parado
	//o lo para si est√° en movimiento (actuar sobre rutinas del TIMER)
	if (buttons & BUT2) {
		led2_switch();
		if (RL.mode == 0)
		{
			RL.mode = 1;
			if (RL.direction == 2) RL.numActual = -1;
			else RL.numActual = 17;
		}
		else
		{
			RL.mode = 0;
			if (RL.direction == 2) RL.numActual = -2;
			else RL.numActual = 16;
		}

		//Actualizamos y reiniciamos el timer
		//tmr_update(TIMER0);
		//tmr_start(TIMER0);
	}

		// eliminamos rebotes
	Delay(2000);

	//COMPLETAR: debemos borrar las peticiones de interrupci√≥n correspondientes a los pulsadores en rEXTINTPND
	rEXTINTPND |= buttons;
	ic_cleanflag(INT_EINT4567);
}

void keyboard_ISR(void)
{
	Delay(20);
	int tecla = kb_scan();
	D8Led_digit(tecla);
	switch (tecla)
	{
	case 0:
		tmr_set_divider(TIMER0, D1_8);
		tmr_set_count(TIMER0, 62500, 2000);
		tmr_update(TIMER0);
		tmr_set_divider(TIMER1, D1_8);
		tmr_set_count(TIMER1, 62500, 2000);
		tmr_update(TIMER1);
		break;
	case 1:
		tmr_set_divider(TIMER0, D1_8);
		tmr_set_count(TIMER0, 31250, 2000);
		tmr_update(TIMER0);
		tmr_set_divider(TIMER1, D1_8);
		tmr_set_count(TIMER1, 31250, 2000);
		tmr_update(TIMER1);
		break;
	case 2:
		tmr_set_divider(TIMER0, D1_8);
		tmr_set_count(TIMER0, 15625, 2000);
		tmr_update(TIMER0);
		tmr_set_divider(TIMER1, D1_8);
		tmr_set_count(TIMER1, 15625, 2000);
		tmr_update(TIMER1);
		break;
	case 3:
		tmr_set_divider(TIMER0, D1_4);
		tmr_set_count(TIMER0, 15625, 2000);
		tmr_update(TIMER0);
		tmr_set_divider(TIMER1, D1_4);
		tmr_set_count(TIMER1, 15625, 2000);
		tmr_update(TIMER1);
		break;
	case 14:
		if (tmr_isrunning(TIMER1)) tmr_stop(TIMER1);
		else tmr_start(TIMER1);
		break;
	case 15:
		if (tmr_isrunning(TIMER0)) tmr_stop(TIMER0);
		else tmr_start(TIMER0);
		break;
	}
	while (!(rPDATG & 0x2)) {}
	D8_led_enciende(0);
	/*Es preciso resaltar que para borrar la peticiÛn de interrupciÛn por una de estas lÌneas
	de interrupciÛn, la rutina de servicio debe borrar el bit correspondiente de este registro
	(escribiendo un í1í, no un í0í) antes de borrar el bit de INTPND del controlador de interrupciones.*/
	//rEXTINTPND |= 0x2;
	Delay(20);
	ic_cleanflag(INT_EINT1);
}

int setup(void)
{
	leds_init();
	D8Led_init();
	D8Led_segment(0);
	pISR_TIMER0 = (unsigned) timer0_ISR;
	pISR_EINT4567 = (unsigned) button_ISR;
	pISR_EINT1 = (unsigned) keyboard_ISR;
	pISR_TIMER1 = (unsigned) timer1_ISR;
	/* Port G: configuraci√≥n para espera activa */
	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//hay que configurar los pines 6 y 7 del puerto G como pines de entrada y
	//activar las correspondientes resistencias de pull-up

	portG_conf(6, EINT);
	portG_eint_trig(6, FALLING);
	portG_conf_pup(6, ENABLE);
	portG_conf(7, EINT);
	portG_eint_trig(7,FALLING);
	portG_conf_pup(7, ENABLE);
	portG_conf(1, EINT);
	portG_eint_trig(1, FALLING);
	portG_conf_pup(1, ENABLE);

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
	tmr_set_count(TIMER0, 62500, 2000);
	tmr_update(TIMER0);
	tmr_set_mode(TIMER0, RELOAD);
	tmr_stop(TIMER0);

	tmr_set_prescaler(TIMER1, 255);
	tmr_set_divider(TIMER1, D1_8);
	tmr_set_count(TIMER1, 62500, 2000);
	tmr_update(TIMER1);
	tmr_set_mode(TIMER1, RELOAD);
	tmr_stop(TIMER1);

	/***************************/

	/* Configuraci√≥n del controlador de interrupciones */

	ic_init();
	//COMPLETAR: utilizando el interfaz definido en intcontroller.h
	//		habilitar la l√≠nea IRQ en modo no vectorizado
	//		deshabilitar la l√≠nea FIQ
	//		configurar la l√≠nea INT_TIMER0 en modo IRQ
	//		configurar la l√≠nea INT_EINT4567 en modo IRQ
	//		habilitar la l√≠nea INT_TIMER0
	//		habilitar la l√≠nea INT_EINT4567

	ic_conf_irq(ENABLE, VEC);
	ic_conf_fiq(DISABLE);
	ic_conf_line(INT_TIMER0, IRQ);
	ic_conf_line(INT_TIMER1, IRQ);
	ic_conf_line(INT_EINT4567, IRQ);
	ic_conf_line(INT_EINT1, IRQ);
	ic_enable(INT_TIMER0);
	ic_enable(INT_TIMER1);
	ic_enable(INT_EINT4567);
	ic_enable(INT_EINT1);
	/***************************************************/


	Delay(0);
	return 0;
}


int main(void)
{
	setup();

	while (1) {
	}
}
