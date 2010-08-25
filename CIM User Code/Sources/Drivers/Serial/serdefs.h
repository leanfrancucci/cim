/*
 * 	serdefs.h
 */

#ifndef __SERDEFS_H__
#define __SERDEFS_H__

#include "gsqueue.h"

#define HOSTRX_MAX_CH		144
#define VALRX_MAX_CH		64

/*
 * Maintain the Host received chars
 */

extern ELEM_TYPE hostrx_queue[ HOSTRX_MAX_CH ];

/*
 * Maintain the Module received chars
 */

extern ELEM_TYPE valrx_queue[ VALRX_MAX_CH ];

enum
{
	CTCH, VALCH,
	NUM_CHANNELS
};

enum
{
	B300, B600, B1200, B2400, B4800, B9600, B19200,
	B38400, B57600,
	NUM_BAUD
};

enum
{
	BIT8, BIT7, BIT6, BIT5,
	NUM_BITS
};

enum
{
	NO_PAR, EVEN_PAR, ODD_PAR, HIGH_PAR, LOW_PAR,
	NUM_PAR
};

enum
{
	ONE_BIT_STOP, TWO_BIT_STOP
};

typedef struct
{
	MUInt baud;
	MUInt bits;
	MUInt parity;
} SERIAL_T;

#endif
