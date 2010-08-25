/*
 *	ioports.c
 *	
 *	Routines for PORTS initialization.
 *
 *	Programmer: Darío S. Baliña.
 */

#include "derivative.h"
#include "ioports.h"
#include "sleep.h"

void
ports_init(void)
{
	D_WDOG_IN = 0;	
	P_WDOG_IN = 1;	
	D_VRLY_IN = 0;	
	P_VRLY_IN = 1;	
	D_VLOCK_IN = 0;
	P_VLOCK_IN = 1;

	WDOG_OUT = 1;	
	D_WDOG_OUT = 1;	
	VRLY_OUT = 0;	
	D_VRLY_OUT = 1;	
	VLOCK_OUT = 0;
	D_VLOCK_OUT = 1;

	TP6 = 0;
	D_TP6 = 0;
	P_TP6 = 1;
	TP8 = 0;
	D_TP8 = 1;
}


