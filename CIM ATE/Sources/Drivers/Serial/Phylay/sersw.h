/*
 * sersw.h
 */

#ifndef __SERSW_H__
#define __SERSW_H__

#include "mytypes.h"

enum
{
	CH0, CH1, NUM_CHS
};

/*
 * init_sersw:
 * 	Initialize serial switch
 */

void init_sersw( void );

/*
 * set_sersw:
 *	Select desired channel
 */

void set_sersw( MUInt ch );

#endif
