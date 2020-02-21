/*-------------------------------------------------------------------
**
**  Fichero:

**    D8Led.c
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las implementaciones de las funciones
**    para el acceso al display 8-segmentos de la placa de prototipado
**
**  Notas de dise�o:
**    Equivale al fichero hom�nimo usado en Fundamentos de Computadores 
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "44blib.h"
#include "D8Led.h"

/* 
 * M�scaras �tiles para el uso del display de 8 segmentos
 * Cada bit representa un segmento. En la m�scara ponemos
 * un 1 si queremos que se encienda dicho segmento. Como 
 * el display funciona con l�gica invertida, nos toca
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
		//correcto para que se ilumine el segmento que est� en la posici�n
		//value del array Segments. Hay que tener en cuenta que el display
		//usa l�gica invertida (0 enciende, 1 apaga)
		LED8ADDR = ~Segments[value];
	}

}

void D8Led_digit(int value)
{
	if( (value >= 0) && (value < 16) ){
		//COMPLETAR: debemos escribir en el display (LED8ADDR) el valor
		// correcto para que se ilumine el d�gito que est� en la posici�n
		//value del array Digits. Hay que tener en cuenta que el display
		//usa l�gica invertida (0 enciende, 1 apaga)
		LED8ADDR = ~Digits[value];
	}

}

void D8Led_enciende(int value)
{
		LED8ADDR = ~value;
}
