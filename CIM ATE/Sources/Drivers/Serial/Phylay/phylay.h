/*
 * phylay.h
 */

#ifndef __PHYLAY_H__
#define __PHYLAY_H__

#include "mytypes.h"

/*
 * RS-485 direction
 */

enum
{
	TORCV, TOXMIT
};

/*
 * init_phymed:
 *	Initialize this module and lower
 *	layers
 */

void init_phymed( void );

/*
 * set_dir_phymed:
 *	Select direction of RS485 channel
 */

void set_dir_phymed( MUInt dir );

#endif
