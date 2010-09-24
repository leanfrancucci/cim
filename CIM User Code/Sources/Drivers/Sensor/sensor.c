/*
 * 	sensor.c
 */

#include "mytypes.h"

#include "sensor.h"
#include "ioports.h"
#include "cparse.h"

static const char sens_mask[ NUM_SENSORS ] =
{
	LOCK1S_MASK, PLG1S_MASK,
	LOCK2S_MASK, PLG2S_MASK,
	VAL1ST_MASK, VAL2ST_MASK,
	KEYS_MASK,
	LOCK3S_MASK, LOCK4S_MASK,
	DOOR3S_MASK, DOOR4S_MASK
};

static
MUInt
get_sensor( MUInt hdlr )
{
	switch( hdlr )
	{
		case LOCKS1:
		case PLGS1:
		case LOCKS2:
		case PLGS2:
		case VAL1STCK:
		case VAL2STCK:
			return SENSORS_IN & sens_mask[hdlr];
		case KEYSWITCH:
			return KEYS;
		case LOCKS3:
		case LOCKS4:
		case PLGS3:
		case PLGS4:
			return AUXILIAR_IN & sens_mask[hdlr];
		default:
			return 0;
	}
#if 0
	if( hdlr == KEYSWITCH )
		return KEYS; 

	else
		return SENSORS_IN & sens_mask[hdlr];
#endif
}

void
init_auxiliar_io( void )
{
	LOCK3 = 0;
	D_LOCK3 = 1;
	P_LOCK3S = 1;
	D_LOCK3S = 0;
	P_DOOR3S = 1;
	D_DOOR3S = 0;

	LOCK4 = 0;
	D_LOCK4 = 1;
	P_LOCK4S = 1;
	D_LOCK4S = 0;
	P_DOOR4S = 1;
	D_DOOR4S = 0;
}

void
init_sensors( void )
{
	MUInt hdlr;

	init_auxiliar_io();

	for( hdlr=LOCKS1; hdlr < NUM_SENSORS; ++hdlr )
		init_cparse( hdlr, get_sensor(hdlr) );
}

void
proc_sensors( void )
{
static	MUInt hdlr = LOCKS1;

//	for( hdlr=LOCKS1; hdlr < NUM_SENSORS; ++hdlr )
	if( ++hdlr == NUM_SENSORS )
		hdlr = LOCKS1;

	do_cparse( hdlr, get_sensor(hdlr) );
}

