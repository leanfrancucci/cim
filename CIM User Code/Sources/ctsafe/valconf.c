/*
 * valconfig.c
 */

#include "mytypes.h"
#include "serlog.h"
#include "serdefs.h"
#include "valconf.h"
#include "settings.h"

#include <string.h> 


#define START_TOUT_CONSTANT		(1000/STIMER_BASE)


static VAL_CONFIG_T val_configs[] =
{
	{ 0, ONE_BIT_STOP, EVEN_PAR, BIT8, B9600, DEF_TOUT_STVAL },
	{ 0, ONE_BIT_STOP, EVEN_PAR, BIT8, B9600, DEF_TOUT_STVAL }
//	{ 0, ONE_BIT_STOP, NO_PAR, BIT8, B9600, DEF_TOUT_STVAL },
//	{ 0, ONE_BIT_STOP, NO_PAR, BIT8, B9600, DEF_TOUT_STVAL }
};

/*
 * get_start_val_timeout:
 *
 * 		Used to wait a start of frame from validator.
 */

ushort
get_start_val_timeout( MUInt val )
{
	return val_configs[ val ].start_timeout * START_TOUT_CONSTANT;
}

/*
 * set_val_config:
 *
 * 		Store validator configuration.
 * 		Is called by start-up process and by VALCONFIG command.
 */

void
set_val_config( MUInt val, VAL_CONFIG_T *p )
{
#if 1
	if( val_configs[ val ].stop_bits == ONE_BIT_STOP )
		val_configs[ val ].stop_bits = p->stop_bits;

	if( val_configs[ val ].parity <= ODD_PAR )
		val_configs[ val ].parity  = p->parity;

	if( val_configs[ val ].word_bits == BIT8 )
		val_configs[ val ].word_bits = p->word_bits;

	val_configs[ val ].baud_rate = p->baud_rate;
	val_configs[ val ].start_timeout = p->start_timeout;
#else
	memcpy( &val_configs[ val ], p, sizeof( VAL_CONFIG_T ) );
#endif
}

/*
 * set_val_hw_config:
 *
 * 		Configure serial channel of desired validator.
 * 		Must be called before sending and receiving a frame from validator.
 */

void
set_val_hw_config( MUInt val )
{
	SERIAL_T serial_config;

	//serial_config.stop = val_configs[ val ].stop_bits; /* not implemented*/

	serial_config.baud = val_configs[ val ].baud_rate;
	serial_config.bits = val_configs[ val ].word_bits;
	serial_config.parity = val_configs[ val ].parity;

	init_serlog( VALCH, &serial_config );
}
