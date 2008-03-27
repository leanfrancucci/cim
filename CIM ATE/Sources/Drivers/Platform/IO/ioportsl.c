/*
 *	ioports.c
 *	
 *	Routines for PORTS initialization.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#ifdef _S08_PLATFORM_
#include "derivative.h"
#endif

#include "ioportsl.h"
#include "iodefs.h"
#include "porthdl.h"
#include "string.h"

PORT_STR ports_str[NUM_PORTS];

void
refresh_ports( void )
{
#ifdef _S08_PLATFORM_
	PORT0_DATA = ports_str[ PORT0 ].data;
	PORT0_DIR = ports_str[ PORT0 ].dir;
	PORT0_PUP = ports_str[ PORT0 ].pup;
	ports_str[ PORT0 ].data = PORT0_DATA;

	PORT1_DATA = ports_str[ PORT1 ].data;
	PORT1_DIR = ports_str[ PORT1 ].dir;
	PORT1_PUP = ports_str[ PORT1 ].pup;
	ports_str[ PORT1 ].data = PORT1_DATA;

	PORT2_DATA = ports_str[ PORT2 ].data;
	PORT2_DIR = ports_str[ PORT2 ].dir;
	PORT2_PUP = ports_str[ PORT2 ].pup;
	ports_str[ PORT2 ].data = PORT2_DATA;
	
	PORT3_DATA = ports_str[ PORT3 ].data;
	PORT3_DIR = ports_str[ PORT3 ].dir;
	PORT3_PUP = ports_str[ PORT3 ].pup;
	ports_str[ PORT3 ].data = PORT3_DATA;

	PORT4_DATA = ports_str[ PORT4 ].data;
	PORT4_DIR = ports_str[ PORT4 ].dir;
	PORT4_PUP = ports_str[ PORT4 ].pup;
	ports_str[ PORT4 ].data = PORT4_DATA;

	PORT5_DATA = ports_str[ PORT5 ].data;
	PORT5_DIR = ports_str[ PORT5 ].dir;
	PORT5_PUP = ports_str[ PORT5 ].pup;
	ports_str[ PORT5 ].data = PORT5_DATA;

	PORT6_DATA = ports_str[ PORT6 ].data;
	PORT6_DIR = ports_str[ PORT6 ].dir;
	PORT6_PUP = ports_str[ PORT6 ].pup;
	ports_str[ PORT6 ].data = PORT6_DATA;
#endif
}

void
ports_init(void)
{
	const PORT_TABLE *pport;
	const PIN_TABLE *ppin;
	const PORT_STR *p;

	for( pport = &port_tbl[ 0 ]; pport->alias != NULL ; ++pport )
	{
		for( ppin = pport->ppintbl; ppin->alias != NULL ; ++ppin )
		{
			ports_str[pport->mcu_port].data = 0;
			if( ppin->dir == OUTPUT )
				ports_str[pport->mcu_port].dir |= ppin->mask;
			else
			{
				ports_str[pport->mcu_port].dir &= ~ppin->mask;
				ports_str[pport->mcu_port].pup |= ppin->mask;
			}
		}
	}

	refresh_ports();

}

