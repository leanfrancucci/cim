/*
 * mpdata.h
 */

#ifndef __MPDATA_H__
#define __MPDATA_H__

#include "mparser.h"

enum
{
	CT_PROT, VAL_PROT, 
	DOORWS_CTRL, DOORWOS_CTRL, PWRCTRL_CTRL,
	
	NUM_MPARSERS
};

extern MPARSE mparsers[ NUM_MPARSERS ];


#endif
