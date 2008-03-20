/*
 * 	dfpwf.c
 * 		Power fail routines
 * 		used for dataflash
 *
 * 		Version 0.1	7/11/07 12:36 ARG
 *
 * 		Delsat Group S.A.
 * 		Eduardo Martinez
 *
 */

#include "mytypes.h"

#include "dfpwf.h"
#include "pwfhal.h"
#include "pwrhdl.h"
#include "dfsys.h"

void
disable_pwf_interrupt( void )
{
	do_disable_pwf_interrupt();
}


void
enable_pwf_interrupt( void )
{
	if( !in_pwf() )
		do_enable_pwf_interrupt();
}

void
dfpwf_interrupt( void )
{
	dfilesys_sync();
}

