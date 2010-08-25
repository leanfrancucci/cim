/*
 * 	sertable.c
 */

#include "mytypes.h"

#include "sertable.h"
#include "serdefs.h"

unsigned char hostrx_queue[ HOSTRX_MAX_CH ];
unsigned char valrx_queue[ VALRX_MAX_CH ];

const SERIAL_T serial_default[] =
{
	{	B38400, BIT8, NO_PAR	},
	{	B9600, 	BIT8, EVEN_PAR	}
};
