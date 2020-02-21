/*-------------------------------------------------------------------
**
**  Fichero:
**    main.c  11/2/2016
**
**  Notas de diseño:
**    - El buffer de memoria del LCD esta en 0x0c200000 (es programable)
**      y es de tamaño 0x9600B (320x240px y con 2px/B)
**    - Las 36 imagenes a visualizar consecutivamente en el LCD están
**      ubicadas a partir de la dirección 0x0c400000 cada 0x10000B,
**      cada una ocupa 0x9600B y se indican en el fichero load_img.text
**
**-----------------------------------------------------------------*/


#include "44b.h"
#include "44blib.h"
#include <stdio.h>
#include "option.h"
#include "dma.h"
#include "uart.h"
#include "utils.h"
#include "intcontroller.h"
#include "timer.h"
#include "gpio.h"
#include "leds.h"
#include "button.h"
#include "keyboard.h"
#include "D8Led.h"

#define BUT2 0x2

struct RLstat {
	int mode;
	int position;
};

static struct RLstat RL = {
	.mode = 4,
	.position = 0
};

struct RL1stat {
	int direction;
	int numActual;
	int mode;
};

static struct RL1stat RL1 = {
	.direction = 2,
	.numActual = -2,
	.mode = 0
};


/*
** El buffer de memoria del LCD esta en 0x0c200000 y es de tamaño 0x9600B
** (El LCD tiene 320x240px y en memoria cada byte contiene 2px)
**UBICADAS EN MEMORIA A PARTIR DE LA 0X0C400000 CADA OX10000B
*/

void timer0_ISR( void ) __attribute__ ((interrupt ("IRQ")));
void timer1_ISR( void ) __attribute__ ((interrupt ("IRQ")));
void timer2_ISR( void ) __attribute__ ((interrupt ("IRQ")));
void keyboard_ISR(void)__attribute__((interrupt ("IRQ")));

unsigned int i=0;
unsigned int imagDir = 0x0c400000;

void putImageNoDMA( unsigned int imgDir )
{
	unsigned int i;
	unsigned char *src, *dst;

	src = (unsigned char *) imgDir;
	dst = (unsigned char *) 0x0c200000;

	for( i=0; i<0x9600; i++ )
		dst[i] = src[i];
}

void timer0_ISR(void)
{
	unsigned int buttons = read_button();
	if(buttons & BUT2)
	{
		putImageNoDMA(imagDir);
		led2_switch();
	}
	else
	{
		putImageDMA(imagDir);
		led1_switch();
	}
	if(i < 44 - 1)
	{
		i++;
		imagDir = imagDir + 0x10000;
	}
	else
	{
		imagDir = 0x0c400000;
		i=0;
	}
	ic_cleanflag(INT_TIMER0);
}

void timer1_ISR( void )
{
	switch(RL.mode)
		{
		case 0:
			D8Led_enciende(Hola[RL.position]); RL.position++;
			if (RL.position > 3) tmr_stop(TIMER1);
			break;
		case 1:
			D8Led_enciende(EC[RL.position]); RL.position++;
			if (RL.position > 1) tmr_stop(TIMER1);
			break;
		case 2:
			D8Led_enciende(anio[RL.position]); RL.position++;
			if (RL.position > 3) tmr_stop(TIMER1);
			break;
		case 3:
			D8Led_enciende(Adios[RL.position]); RL.position++;
			if (RL.position > 4) tmr_stop(TIMER1);
			break;
		}
		ic_cleanflag(INT_TIMER1);
}

void timer2_ISR( void )
{
	if (RL1.mode == 0)
	{
		RL1.numActual += RL1.direction;
		if (RL1.numActual > 14) RL1.numActual = 0;
		if (RL1.numActual < 0) RL1.numActual = 14;
	}
	else
	{
		RL1.numActual += RL1.direction;
		if (RL1.numActual > 15) RL1.numActual = 1;
		if (RL1.numActual < 1) RL1.numActual = 15;
	}
	D8Led_digit(RL1.numActual);
	ic_cleanflag(INT_TIMER2);
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
		tmr_set_count(TIMER0, 3137, 2000);
		tmr_update(TIMER0);
		break;
	case 1:
		tmr_set_divider(TIMER0, D1_8);
		tmr_set_count(TIMER0, 1568, 1500);
		tmr_update(TIMER0);
		break;
	case 2:
		tmr_set_divider(TIMER0, D1_8);
		tmr_set_count(TIMER0, 784, 700);
		tmr_update(TIMER0);
		break;
	case 3:
		tmr_set_divider(TIMER0, D1_4);
		tmr_set_count(TIMER0, 392, 300);
		tmr_update(TIMER0);
		break;
	case 4: RL.mode = 0; RL.position = 0; tmr_start(TIMER1); tmr_stop(TIMER2); break;
	case 5: RL.mode = 1; RL.position = 0; tmr_start(TIMER1); tmr_stop(TIMER2); break;
	case 6: RL.mode = 2; RL.position = 0; tmr_start(TIMER1); tmr_stop(TIMER2); break;
	case 7: RL.mode = 3; RL.position = 0; tmr_start(TIMER1); tmr_stop(TIMER2); break;
	case 8:
		if (RL1.direction == 2) RL1.direction = -2;
		else RL1.direction = 2;
		tmr_stop(TIMER1); tmr_start(TIMER2);
		break;
	case 9:
		if (RL1.mode == 0)
		{
			RL1.mode = 1;
			if (RL1.direction == 2) RL1.numActual = -1;
			else RL1.numActual = 17;
		}
		else
		{
			RL1.mode = 0;
			if (RL1.direction == 2) RL1.numActual = -2;
			else RL1.numActual = 16;
		}
		tmr_stop(TIMER1); tmr_start(TIMER2);
		break;
	}
	while (!(rPDATG & 0x2)) {}
	D8Led_enciende(0);
	Delay(20);
	ic_cleanflag(INT_EINT1);
}

void sys_init( void )
{
	rWTCON  = 0;		// deshabilita el watchdog
	rINTMSK = ~0;		// enmascara todas las interrupciones

	// Configuracion del gestor de reloj
	rLOCKTIME = 0xfff;	// estabilizacion del PLL = 512 us
	rPLLCON   = 0x38021;	// MCLK = 64MHz
	rCLKSLOW  = 0x8;		// MCLK_SLOW = 500 KHz
	rCLKCON   = 0x7ff8;	// modo NORMAL y reloj distribuido a todos los controladores

	// Configuracion del arbitro de bus
	// prioridad fija por defecto LCD > ZDMA > BDMA > IRQ
     rSBUSCON = 0x8000001b;	

	// Configuracion de cache
	rSYSCFG = 0;				// deshabilitada

	// Configuracion del controlador de interrupciones
	rI_PMST = 0x1f1b;	// prioridades fijas por defecto
	rI_PSLV = 0x1b1b1b1b;
	
	rEXTINTPND = ~0;			// borra las interrupciones externas pendientes por linea EINT7..4
    rI_ISPC = ~0;			// borra todas las interrupciones pendientes
}	

void lcd_init( void )
{      
	rDITHMODE = 0x12210;
	rDP1_2    = 0xa5a5;
	rDP4_7    = 0xba5da65;
	rDP3_5    = 0xa5a5f;
	rDP2_3    = 0xd6b;
	rDP5_7    = 0xeb7b5ed;
	rDP3_4    = 0x7dbe;
	rDP4_5    = 0x7ebdf;
	rDP6_7    = 0x7fdfbfe;
	
	rREDLUT   = 0;
	rGREENLUT = 0;
	rBLUELUT  = 0;

	// indica al controlador de LCD que el buffer LCD esta en la dirección 0x0c200000 y es de tamaño 0x9600B;
	rLCDSADDR1 = 0x16100000;
	rLCDSADDR2 = 0x20104B00;
	rLCDSADDR3 = 0x50;

	rLCDCON2  = 0x13cef;
	rLCDCON3  = 0;
	rLCDCON1   = 0xc021; // refresco a 120 Hz
}

int setup(void)
{
    sys_init();
    uart0_init();
    lcd_init();
    zdma0_init();
    D8Led_init();


    pISR_EINT1 = (unsigned) keyboard_ISR;
	pISR_TIMER0 = (unsigned) timer0_ISR;
	pISR_TIMER1 = (unsigned) timer1_ISR;
	pISR_TIMER2 = (unsigned) timer2_ISR;
/* Configurar los puertos, el controlador de interrupciones y el timer0 para que genere interrupciones vectorizadas por la línea IRQ cada 0,1segundos. */

   	portG_conf(7, INPUT);
   	portG_conf_pup(7, ENABLE);
   	portG_conf(1, EINT);
   	portG_eint_trig(1, FALLING);
   	portG_conf_pup(1, ENABLE);

	tmr_set_prescaler(TIMER1, 255);
   	tmr_set_divider(TIMER1, D1_8);
   	tmr_set_count(TIMER1, 62500, 2000);
 	tmr_update(TIMER1);
  	tmr_set_mode(TIMER1, RELOAD);
  	tmr_stop(TIMER1);

  	tmr_set_prescaler(TIMER0, 255);
  	tmr_set_divider(TIMER0, D1_8);
  	tmr_set_count(TIMER0, 3137, 2000);
  	tmr_update(TIMER0);
  	tmr_set_mode(TIMER0, RELOAD);
  	tmr_stop(TIMER0);
    tmr_start(TIMER0);

    tmr_set_prescaler(TIMER2, 255);
    tmr_set_divider(TIMER2, D1_8);
    tmr_set_count(TIMER2, 62500, 2000);
    tmr_update(TIMER2);
    tmr_set_mode(TIMER2, RELOAD);
    tmr_stop(TIMER2);
    tmr_start(TIMER2);

  	ic_init();

  	ic_conf_irq(ENABLE, VEC);
  	ic_conf_fiq(DISABLE);
  	ic_conf_line(INT_TIMER0, IRQ);
 	ic_enable(INT_TIMER0);
 	ic_conf_line(INT_TIMER1, IRQ);
 	ic_enable(INT_TIMER1);
 	ic_conf_line(INT_TIMER2, IRQ);
 	ic_enable(INT_TIMER2);
 	ic_conf_line(INT_EINT1, IRQ);
 	ic_enable(INT_EINT1);
	return 0;
}

int main( void )
{
	unsigned int num = 0;

	setup();
 
	while( 1 )
    {
		uart0_putint(num++);
		uart0_putchar( '\n' );
    }
}


