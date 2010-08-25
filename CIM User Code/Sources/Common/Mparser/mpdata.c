/*
 * mpdata.c
 */

#include "mparser.h"

#include "alphctrl.h"
#include "alphact.h"
#include "valctrl.h"
#include "valact.h"
#include "dwsctrl.h"
#include "dwsact.h"
#include "dwosctrl.h"
#include "dwosact.h"
#include "pwrctrl.h"
#include "pwract.h"

MPARSE mparsers[] =
{
	{	&idle_prot,		&idle_prot,		init_prot		},
	{	&vblocked,		&vblocked, 		init_val		},
	{	&idle_ws,		&locked_ws, 	init_dwsctrl	},
	{	&idle_wos,		&locked_wos, 	init_dwosctrl	},
	{	&extchrg,	 	&extchrg, 		init_pwrctrl	}
};
