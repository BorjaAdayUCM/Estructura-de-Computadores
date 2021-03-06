/*-----------------------------------------------------------------
**
**  Fichero:
**    button.c
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las definiciones de los prototipos de funciones
**    para la gesti�n de los pulsadores de la placa de prototipado
**
**  Notas de dise�o:
**
**---------------------------------------------------------------*/

#ifndef BUTTON_H_
#define BUTTON_H_

#define BUT1 0x1
#define BUT2 0x2

#define BIT_BUTTON1 6
#define BIT_BUTTON2 7

unsigned int read_button(void);

#endif /* BUTTON_H_ */
