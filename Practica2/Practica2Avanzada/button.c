/*-------------------------------------------------------------------
**
**  Fichero:
**    button.c
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automï¿½tica
**    Facultad de Informï¿½tica. Universidad Complutense de Madrid
**
**  Propï¿½sito:
**    Contiene las implementaciones de las funciones
**    para la gestiï¿½n de los pulsadores de la placa de prototipado
**
**  Notas de diseï¿½o:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "utils.h"
#include "button.h"
#include "leds.h"
#include "gpio.h"

unsigned int read_button(void)
{
	unsigned int buttons = 0;
	enum digital val;
	//COMPLETAR utilizando el interfaz del puerto G de gpio.h
	//tiene que leer los pines 6 y 7 del puerto G (portG_read) y devolver en la variable buttons
	//un 0 si no hay ningún botón pulsado
	//1 si se a pulsado el botón 1
	//2 si se a pulsado el botón 2
	// 3 si se a pulsado los 2 (Así se ejecutan los 2 y no solo el 2).
	if (portG_read(BIT_BUTTON1, &val) == 0)
	{
		if (val == LOW) buttons |= BUT1;
	}
	if (portG_read(BIT_BUTTON2, &val) == 0)
	{
		if (val == LOW) buttons |= BUT2;
	}
	return buttons;
}


