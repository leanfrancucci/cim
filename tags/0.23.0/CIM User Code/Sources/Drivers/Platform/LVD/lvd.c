/*
 *	lvd.c
 *
 *	Routines for LVD Module.
 *
 */

#include "derivative.h"
#include "lvd.h"

/*
 *	lvd_init:
 */
void
lvd_init( void )
{
#if 0
	SPMSC1_LVDRE = 1;   /* Force an MCU reset when LVDF = 1 */
	SPMSC1_LVDSE = 1;	  /* LVD Control Enable in stop */
	SPMSC1_LVDACK = 1;	/* Acknowledge of LVDF */
	SPMSC1_LVDE = 1;    /* Enable LVD */

  SPMSC2_LVDV = 1;    /* LVDVH selected ~ 2.2V */
#endif
	
	SPMSC1 = 0x15;
	SPMSC2 = 0x20;	
}
