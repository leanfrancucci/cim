/*
 * porthdl.h
 */
#include <stdlib.h>
#include "porthdl.h"
#include "adc.h"
#include "formats.h"
#include "ioportsl.h"
#include "enaint.h"

unsigned char ports[NUM_PORTS];

MInt
set_output_hdl( MUInt mcu_port, MUInt mask )
{
	if( mcu_port >= NUM_PORTS )
		return -1;

	sem_disable();
	ports_str[mcu_port].data |= mask;
	sem_enable();
	return 0;
}

MInt
clear_output_hdl( MUInt mcu_port, MUInt mask )
{
	if( mcu_port >= NUM_PORTS )
		return -1;

	sem_disable();
	ports_str[mcu_port].data &= ~mask;
	sem_enable();
	return 0;
}

MInt
outport_hdl( MUInt mcu_port, unsigned char value )
{
	if( mcu_port >= NUM_PORTS )
		return -1;

	sem_disable();
	ports_str[mcu_port].data = value;
	sem_enable();
	return 0;
}

MInt
inport_hdl( MUInt mcu_port, unsigned char *value )
{
	if( mcu_port >= NUM_PORTS )
		return -1;

	sem_disable();
	*value = ports_str[mcu_port].data;
	sem_enable();
	return 0;
}

MInt
read_input_hdl( MUInt mcu_port, unsigned char mask )
{
	MInt b;
	
	if( mcu_port >= NUM_PORTS )
		return -1;

	sem_disable();
	b = ( (ports_str[mcu_port].data&mask) != 0 );
	sem_enable();
	return b; 
}

int
read_adc_hdl( MUInt mcu_port, unsigned char mask )
{
	int val;
	
	if( mcu_port != ANALOG_PORT )
		return -1;

	val = adquire_adc( mask );
//	val *= 10; 
	
	return (val/ADC_K);
}

