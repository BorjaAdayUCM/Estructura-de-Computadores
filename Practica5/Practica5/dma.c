#include "44b.h"
#include "dma.h"

void zdma0_init( void )
{
	rZDCON0  = 0;
	rZDISRC0 = 0;
	rZDIDES0 = 0;
	rZDICNT0 = 0;
}


void putImageDMA( unsigned int imgDir )
{
	rZDISRC0 = (2 << 30) | (1 << 28) | imgDir;					    // datos de 32b, dirección post-incrementada
	rZDIDES0 = (2 << 30) | (1 << 28) | (unsigned int) 0x0c200000;	// recomendada, dirección post-incrementada
	rZDICNT0 = (2 << 28) | (1 << 26) | (1 << 20) | 0x9600; 	        // whole service, unit transfer, polling mode, no autoreload, enable DMA
	rZDCON0  = 1;											        // start DMA
}
