/*-------------------------------------------------------------------
**
**  Fichero:

**    D8Led.c
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las implementaciones de las funciones
**    para el acceso al display 8-segmentos de la placa de prototipado
**
**  Notas de diseño:
**    Equivale al fichero homónimo usado en Fundamentos de Computadores 
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "44blib.h"
#include "D8Led.h"

/* 
 * Máscaras útiles para el uso del display de 8 segmentos
 * Cada bit representa un segmento. En la máscara ponemos
 * un 1 si queremos que se encienda dicho segmento. Como 
 * el display funciona con lógica invertida, nos toca
 * invertir el valor al escribir en el puerto.
 */
			  					  
void D8Led_init(void)
{
    LED8ADDR = ~0 ;
}

void D8Led_segment(int value)
{
	if( (value >= 0) && (value < 8) ){
		//COMPLETAR: debemos escribir en el display (LED8ADDR) el valor
		//correcto para que se ilumine el segmento que está en la posición
		//value del array Segments. Hay que tener en cuenta que el display
		//usa lógica invertida (0 enciende, 1 apaga)
		LED8ADDR = ~Segments[value];
	}

}

void D8Led_digit(int value)
{
	if( (value >= 0) && (value < 16) ){
		//COMPLETAR: debemos escribir en el display (LED8ADDR) el valor
		// correcto para que se ilumine el dígito que está en la posición
		//value del array Digits. Hay que tener en cuenta que el display
		//usa lógica invertida (0 enciende, 1 apaga)
		LED8ADDR = ~Digits[value];
	}

}

void D8Led_enciende(int value)
{
		LED8ADDR = ~value;
}
