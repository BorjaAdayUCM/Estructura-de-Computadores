#include "44b.h"
#include "uart.h"


void uart0_init( void )
{
	rUFCON0  = 0x1;	   // FIFOs activadas
   rUMCON0  = 0x0;	   // Control de flujo manual
	rULCON0  = 0x3;     // normal (no infrarojos), sin paridad, 1 bit de stop, 8 bits de datos
	rUBRDIV0 = 0x22;    // 115200 baudios asumiendo 64MHz
	rUCON0   = 0x5;     // Tx int/polling, Rx int/polling
}

void uart0_putchar( char ch )
{
	while( rUFSTAT0 & (1 << 9) );
	rUTXH0 = ch;
}

void uart0_puts( char *s )
{
    while( *s )
		uart0_putchar( *s++ );
}

void uart0_putint( unsigned int i )
{
	char buf[10 + 1];    /* Array con espacio suficiente para los digitos y el '\0' */
	char *p = buf + 10;	 /* Puntero al final del array */

	*p = '\0';

	do {
    	*--p = '0' + (i % 10);
		i /= 10;
	} while( i );

	uart0_puts( p );
}
