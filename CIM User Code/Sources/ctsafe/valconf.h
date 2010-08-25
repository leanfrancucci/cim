/*
 * valconfig.h
 */

#ifndef __VALCONFIG_H__
#define __VALCONFIG_H__

#include "mytypes.h"

enum
{
	VAL_ECHO_ENABLE, VAL_ECHO_DISABLE
};


typedef struct
{
	unsigned reserved 	:1;
	unsigned echo		:1;
	unsigned stop_bits	:1;
	unsigned parity		:3;
	unsigned word_bits	:2;
	uchar baud_rate;
	uchar start_timeout;
} VAL_CONFIG_T;


/*
 * get_start_val_timeout:
 *
 * 		Used to wait a start of frame from validator.
 */

ushort get_start_val_timeout( MUInt val );

/*
 * set_val_config:
 *
 * 		Store validator configuration.
 * 		Is called by start-up process and by VALCONFIG command.
 */

void set_val_config( MUInt val, VAL_CONFIG_T *p );

/*
 * set_val_hw_config:
 *
 * 		Configure serial channel of desired validator.
 * 		Must be called before sending and receiving a frame from validator.
 */

void set_val_hw_config( MUInt val );


/*
 * get_val_echo:
 *
 * 		Returns echo status of desired validator channel.
 */

MUInt get_val_echo( MUInt val );


#endif
