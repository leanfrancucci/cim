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
	KEYS_MASK
};

static
MUInt
get_sensor( MUInt hdlr )
{
	if( hdlr == KEYSWITCH )
		return KEYS; 
	else
		return SENSORS_IN & sens_mask[hdlr];
}

void
init_sensors( void )
{
	MUInt hdlr;

	for( hdlr=LOCKS1; hdlr < NUM_SENSORS; ++hdlr )
		init_cparse( hdlr, get_sensor(hdlr) );
}

void
proc_sensors( void )
{
	MUInt hdlr;

	for( hdlr=LOCKS1; hdlr < NUM_SENSORS; ++hdlr )
		do_cparse( hdlr, get_sensor(hdlr) );
}

