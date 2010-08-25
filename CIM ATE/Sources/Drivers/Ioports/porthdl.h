/*
 * porthdl.h
 */

#ifndef __PORTHDL_H__
#define __PORTHDL_H__

#include "mytypes.h"

enum
{
	PORT0,
	PORT1,
	PORT2,
	PORT3,
	PORT4,
	PORT5,
	PORT6,
	NUM_PORTS
};

extern unsigned char ports[NUM_PORTS];

MInt set_output_hdl( MUInt mcu_port, MUInt mask );
MInt clear_output_hdl( MUInt mcu_port, MUInt mask );
MInt outport_hdl( MUInt mcu_port, unsigned char value );
MInt inport_hdl( MUInt mcu_port, unsigned char *value );
MInt read_input_hdl( MUInt mcu_port, unsigned char mask );
int read_adc_hdl( MUInt mcu_port, unsigned char mask );
#endif