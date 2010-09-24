/*
 * mpdata.c
 */

#include "mparser.h"

#include "alphctrl.h"
#include "alphact.h"
#include "valctrl.h"
#include "valact.h"
#include "lockctrl.h"
#include "lockact.h"
#include "pwrctrl.h"
#include "pwract.h"

MPARSE mparsers[] =
{
	{	&idle_prot,		&idle_prot,		init_prot		},
	{	&vblocked,		&vblocked, 		init_val		},
	{	&lockidle,		&lockidle, 		init_lock0		},
	{	&lockidle,		&lockidle, 		init_lock1		},
	{	&lockidle,		&lockidle, 		init_lock2		},
	{	&lockidle,		&lockidle, 		init_lock3		},
	{	&extchrg,	 	&extchrg, 		init_pwrctrl	}
};
