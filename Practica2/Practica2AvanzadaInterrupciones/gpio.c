#include "44b.h"
#include "gpio.h"

/* Port B interface implementation */

int portB_conf(int pin, enum port_mode mode)
{
	int ret = 0;
	if (pin < 0 || pin > 10)
		return -1;

	if (mode == SIGOUT){
		//COMPLETAR: Configurar el pin del puerto B
		//para que salga la senal correspondiente del controlador de memoria
		rPCONB |= (0x1 << pin);
	}
	else if (mode == OUTPUT){
		///COMPLETAR: Configurar el pin del puerto B
		//para que sea de salida
		rPCONB &=  ~(0x1 << pin);
	}
	else
		ret = -1;

	return ret;
}

int portB_write(int pin, enum digital val)
{
	if (pin < 0 || pin > 10)
		return -1;

	if (val < 0 || val > 1)
		return -1;

	if (val){
		
		//COMPLETAR: poner en el pin del puerto B
		//el valor adecuado para que el led se apague
		rPDATB |= (0x1<< pin);
	}
	else{
		//COMPLETAR: poner en el pin del puerto B
		//el valor adecuado para que el led se encienda
		rPDATB &=  ~(0x1 << pin);
	}
	return 0;
}

/* Port G interface implementation */

int portG_conf(int pin, enum port_mode mode)
{
	//La variable pos indica el primer bit del registro de control del puerto G que corresponde al pin
	int pos  = pin*2;

	if (pin < 0 || pin > 7)
		return -1;

	switch (mode) {
		case INPUT:
			//COMPLETAR: Configurar el pin del puerto G
			//para que sea de entrada
			rPCONG &= ~(0x3 << pos); //NEGADO O NO


			break;
		case OUTPUT:
			//COMPLETAR: Configurar el pin del puerto G
			//para que sea de salida
			rPCONG &= ~(0x3 << pos);
			rPCONG |= (0x1 << pos);

			break;
		case SIGOUT:
			//COMPLETAR: Configurar el pin del puerto G
			//para que salga la señal interna correspondiente
			rPCONG &= ~(0x3 << pos);
			rPCONG |= (0x2 << pos);

			break;
		case EINT:
			//COMPLETAR: Configurar el pin del puerto G
			//para habilitar la generación de interrupciones externas
			rPCONG |= (0x3 << pos);

			break;
		default:
			return -1;
	}

	return 0;
}

int portG_eint_trig(int pin, enum trigger trig)
{
	//La variable pos indica el primer bit del registro EXTINT del puerto G que corresponde al pin
	int pos = pin*4;

	if (pin < 0 || pin > 7)
		return -1;

	switch (trig) {
		case LLOW:
			// COMPLETAR: configurar el pin del puerto G para que genere interrupciones externas por nivel bajo
			rEXTINT &= ~(0x07 << pos);

			break;
		case LHIGH:
			// COMPLETAR: configurar el pin del puerto G para que genere interrupciones externas por nivel alto
			rEXTINT &= ~(0x07 << pos);
			rEXTINT |= (0x01 << pos);

			break;
		case FALLING:
			// COMPLETAR: configurar el pin del puerto G para que el pin genere interrupciones externas por flanco de bajada
			rEXTINT &= ~(0x07 << pos);//puede ser 6
			rEXTINT |= (0x02 << pos);

			break;
		case RISING:
			// COMPLETAR: configurar el pin del puerto G para que el pin genere interrupciones externas por flanco de subida
			rEXTINT &= ~(0x07 << pos);//puede ser 6
			rEXTINT |= (0x04 << pos);

			break;
		case EDGE:
			// COMPLETAR: configurar el pin del puerto G para que el pin genere interrupciones externas por cualquier flanco
			rEXTINT |= (0x07 << pos);//puede ser 6

			break;
		default:
			return -1;
	}
	return 0;
}


int portG_read(int pin, enum digital* val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1;

	if (rPCONG & (0x3 << pos))
		return -1;
	//COMPLETAR la condicion del if: comprobar si el pin del registro de datos del puerto G es un 1)
	if (rPDATG & (0x1 << pin))
		*val=HIGH;
	else
		*val=LOW;

	return 0;
}

int portG_conf_pup(int pin, enum enable st)
{
	if (pin < 0 || pin > 7)
		return -1;

	if (st != ENABLE && st != DISABLE)
		return -1;

	if (st == ENABLE){
		//COMPLETAR: activar la resistencia de pull-up del pin del puerto G
		rPUPG &= ~(0x01 << pin);

	}
	else{
		//COMPLETAR: desactivar la resistencia de pull-up del pin del puerto G
		rPUPG |= (0x01 << pin);
	}
	return 0;
}


int portG_write(int pin, enum digital val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1;

	if (val < 0 || val > 1)
		return -1;

	if ((rPCONG & (0x3 << pos)) != (0x1 << pos))
		return -1;

	if (val)
		rPDATG |= (0x1 << pin);
	else
		rPDATG &= ~(0x1 << pin);

	return 0;
}

