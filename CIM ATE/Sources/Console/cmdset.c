/*
 * cmdset.c
 */

#include "ioports.h"
#include "mytypes.h"
#include "command.h"
#include "conser.h"
#include "formats.h"
#include "error.h"
#include "adc.h"
#include "iodefs.h"
#include "cmdset.h"
#include "porthdl.h"
#include "ioportsl.h"
#include "sleep.h"
#include <string.h>
#include <stdlib.h>

#define RESP_DELAY	300/SLEEP_BASE

MInt 
do_set( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	MInt i;
	const PORT_TABLE *pport;
	const PIN_TABLE *ppin;

	if( argc < SETB_MIN_ARGS )
	{
		log_error(MIN_PIN);
		return 1;
	}
	for( i = 1; i < argc; i++ )
	{
		if( (ppin =	find_pin( argv[i], &pport )) == NULL )
		{
			log_error( INVALID_PINNUMBER );
			return 1;
		}

		if( ppin->dir != OUTPUT )
		{
			log_error( PIN_MUSTOUT );
			return -1;
		}

		if( set_output( ppin, pport ) < 0 )
			return 1;

		myprintf( 0, "\n%s: 0x%02X\n\n", pport->alias, ports_str[pport->mcu_port].data);
	}
	return 0;
}

MInt 
do_clear( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	MInt i;
	const PORT_TABLE *pport;
	const PIN_TABLE *ppin;

	if( argc < CLRB_MIN_ARGS )
	{
		log_error(MIN_PIN);
		return 1;
	}
	for( i = 1; i < argc; i++ )
	{
		if( (ppin = find_pin( argv[i], &pport )) == NULL )
		{
			log_error( INVALID_PINNUMBER );
			return 1;
		}
		
		if( ppin->dir != OUTPUT )
		{
			log_error( PIN_MUSTOUT );
			return -1;
		}

		if( clear_output( ppin, pport ) < 0 )
			return 1;

		myprintf( 0, "\n%s: 0x%02X\n", pport->alias, ports_str[pport->mcu_port].data);
	}
	return 0;
}

MInt 
do_getb( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	MInt i;
	const PORT_TABLE *pport;
	const PIN_TABLE *ppin;
	MInt b;

	if( argc < GETB_MIN_ARGS )
	{
		log_error(MIN_PIN);
		return 1;
	}
	for( i = 1; i < argc; i++ )
	{
		if( (ppin = find_pin( argv[i], &pport )) == NULL )
		{
			log_error( INVALID_PINNUMBER );
			return 1;
		}

		if( ppin->dir == ADC )
		{
			log_error(MUST_IO);
			return 1;
		}

		if( ( b = read_input( ppin, pport ) ) < 0 )
			return 1;
		
		sleep(RESP_DELAY);
//		myprintf( 0, "\n%s: PIN%s(%s): %d (%s)\n", 
//					pport->alias, ppin->number, ppin->alias, b, b == 1 ? ppin->hightxt : ppin->lowtxt );
		myprintf( 0, "\n%s:PIN%s: %d\n", pport->alias, ppin->number, b ); 
	}
	return 0;
}

MInt 
do_outport( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	const PORT_TABLE *pport;
	unsigned char value;
	
	if( argc < OUTP_MIN_ARGS )
	{
		log_error(MIN_PORT);
		return 1;
	}
	
	if( (pport = find_port( argv[1] )) == NULL )
	{
		log_error( INVALID_PORTNUMBER );
		return 1;
	}

	value = (unsigned char)strtol( argv[2], NULL, 0 ); 
	
	if( outport( pport, value ) < 0 )
		return 1;
	
	myprintf( 0, "\n%s: 0x%02X\n", pport->alias, ports_str[pport->mcu_port].data);

	return 0;	
}

MInt 
do_inport( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	const PORT_TABLE *pport;
	unsigned char value;
	char *c;
	
	if( argc < INP_MIN_ARGS )
	{
		log_error(MIN_PORT);
		return 1;
	}
	
	c = argv[1];
	
	if( *c == '-' )
	{
		switch( *(c+1) )
		{
			case 'a':
				for( pport = port_tbl; pport->alias != NULL; ++pport )
					inport( pport, &value );
					break;
			default :
				log_error(INVALID_OPT);
				return -1;
		}	
		return 0;
	}
	
	if( (pport = find_port( argv[1] )) == NULL )
	{
		log_error( INVALID_PORTNUMBER );
		return 1;
	}

	if(  inport( pport, &value ) < 0 )
		return 1;

	sleep(RESP_DELAY);
	myprintf( 0, "\n%s: 0x%02X\n", pport->alias, value );
	return 0;	
}

MInt 
do_geta( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	const PORT_TABLE *pport;
	const PIN_TABLE *ppin;
	int val;

	if( argc < GETA_MIN_ARGS )
	{
		log_error(MIN_PIN);
		return 1;
	}
	
	if( (ppin = find_pin( argv[1], &pport )) == NULL )
	{
		log_error( INVALID_PINNUMBER );
		return 1;
	}

	if( ppin->dir != ADC )
	{
		log_error(MUST_ADC);
		return 1;
	}

	if( read_adc( ppin, pport, &val ) < 0 )
		return 1;
	
	sleep(RESP_DELAY);
	myprintf( 0, "\n%s: PIN%s(%s): %sV\n", pport->alias, ppin->number, ppin->alias, adc2ascci(val) );
	return 0;
}

MInt 
do_status( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	const PORT_TABLE *pport;
	const PIN_TABLE *ppin;
	int adc;
	unsigned char port;
	MInt b;
	
	for( pport = port_tbl; pport->alias != NULL; ++pport )
	{
		inport( pport, &port );
		myprintf( 0, "\n-------------\n", pport->alias, port );
		myprintf( 0, "%s: 0x%02X\n\n", pport->alias, port );
		for( ppin = pport->ppintbl; ppin->alias != NULL; ++ppin )
		{
			if( ppin->dir == ADC )
			{
				read_adc(ppin, pport, &adc);
				myprintf( 0, "PIN%s(%s): %sV\n", ppin->number, ppin->alias, adc2ascci(adc) );
			}
			else
			{
				b = read_input(ppin, pport);
				myprintf( 0, "PIN%s(%s): %d (%s)\n",
					ppin->number, ppin->alias, b, b == 1 ? ppin->hightxt : ppin->lowtxt );
			}
		}	
	}
	return 0;
}
