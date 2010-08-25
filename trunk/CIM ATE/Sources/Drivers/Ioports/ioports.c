/*
 * ioports.c
 */
#include "porthdl.h"
#include "ioports.h"
#include "mytypes.h"
#include "error.h"
#include "iodefs.h"
#include <string.h>

MInt
set_output( const PIN_TABLE *ppin, const PORT_TABLE *pport )
{
	return set_output_hdl( pport->mcu_port, ppin->mask );
}

MInt
clear_output( const PIN_TABLE *ppin, const PORT_TABLE *pport )
{

	return clear_output_hdl( pport->mcu_port, ppin->mask );
}

MInt
read_input( const PIN_TABLE *ppin, const PORT_TABLE *pport )
{
	return read_input_hdl( pport->mcu_port, ppin->mask );
}

MInt
outport( const PORT_TABLE *pport, unsigned char value )
{
	return outport_hdl( pport->mcu_port, value );
}

MInt
inport( const PORT_TABLE *pport, unsigned char *value )
{
	return inport_hdl( pport->mcu_port, value );
}

MInt
read_adc( const PIN_TABLE *ppin, const PORT_TABLE *pport, int *val )
{
	if( (*val = read_adc_hdl(pport->mcu_port, ppin->mask)) < 0 )
		return -1;
		
	return 0; 
}

const
PIN_TABLE *
find_pin( const char *pin, void ** pport )
{
	const PIN_TABLE *ppin;
	const PORT_TABLE *p;

	for( p = port_tbl; p->alias != NULL ; ++p )
	{
		for( ppin = p->ppintbl; ppin->alias != NULL ; ++ppin )
		{
			if( strcmp( pin, ppin->number ) == 0 )
			{
				(const PORT_TABLE *)*pport = p;		
				return ppin;
			}
			if( strcmp( pin, ppin->alias ) == 0 )
			{
				(const PORT_TABLE *)*pport = p;		
				return ppin;
			}
		}
	}
	return NULL;
}	

const
PORT_TABLE *
find_port( const char *port )
{
	const PORT_TABLE *p;

	for( p = &port_tbl[ 0 ]; p->alias != NULL ; ++p )
	{
		if( strcmp( port, p->number ) == 0 )
			return p;
		if( strcmp( port, p->alias ) == 0 )
			return p;
	}
	return NULL;	
}

