/*
 *	ioports.h
 *	
 *	Routines for PORTS initialization.
 *
 *	Programmer: Darío S. Baliña.
 */

#ifndef __IOPORTS_H__
#define __IOPORTS_H__

#include "derivative.h"

/*********************************/
/* HARDWARE CONCTIONS DEFINITION */
/*********************************/
/* DATA_REGISTERS */
/* PORT A Data Register */
#define TP6			PTAD_PTAD0		/* input */
#define TP7			PTAD_PTAD1		/* input */
#define WDOG_IN		PTAD_PTAD2		/* input */
#define VRLY_IN		PTAD_PTAD3		/* input */
/* PORT B Data Register */
#define VLOCK_IN	PTBD_PTBD0		/* input */
#define VLOCK_OUT	PTBD_PTBD1		/* output */
#define VRLY_OUT	PTBD_PTBD2		/* output */
#define WDOG_OUT	PTBD_PTBD3		/* output */
#define TP10		PTBD_PTBD4		/* input */
#define TP11		PTBD_PTBD5		/* input */
#define TP8			PTBD_PTBD6		/* input */
#define TP9			PTBD_PTBD7		/* input */
/*********************************/
/*********************************/
/* DATA DIRECTION REGISTERS */
/* PORT A Data Direction Register */
#define D_TP6		PTADD_PTADD0	/* input */
#define D_TP7		PTADD_PTADD1	/* input */
#define D_WDOG_IN	PTADD_PTADD2	/* input */
#define D_VRLY_IN	PTADD_PTADD3	/* input */
/* PORT B Data Direction Register */
#define D_VLOCK_IN	PTBDD_PTBDD0		/* input */
#define D_VLOCK_OUT	PTBDD_PTBDD1		/* output */
#define D_VRLY_OUT	PTBDD_PTBDD2		/* output */
#define D_WDOG_OUT	PTBDD_PTBDD3		/* output */
#define D_TP10		PTBDD_PTBDD4		/* input */
#define D_TP11		PTBDD_PTBDD5		/* input */
#define D_TP8		PTBDD_PTBDD6		/* input */
#define D_TP9		PTBDD_PTBDD7		/* input */
/*********************************/
/*********************************/
/* PULL UP REGISTERS */
/* PORT A PullUp Enable Register */
#define P_TP6		PTAPE_PTAPE0	/* input */
#define P_TP7		PTAPE_PTAPE1	/* input */
#define P_WDOG_IN	PTAPE_PTAPE2	/* input */
#define P_VRLY_IN	PTAPE_PTAPE3	/* input */
/* PORT B PullUp Enable Register */
#define P_VLOCK_IN	PTBPE_PTBPE0		/* input */
#define P_VLOCK_OUT	PTBPE_PTBPE1		/* output */
#define P_VRLY_OUT	PTBPE_PTBPE2		/* output */
#define P_WDOG_OUT	PTBPE_PTBPE3		/* output */
#define P_TP10		PTBPE_PTBPE4		/* input */
#define P_TP11		PTBPE_PTBPE5		/* input */
#define P_TP8		PTBPE_PTBPE6		/* input */
#define P_TP9		PTBPE_PTBPE7		/* input */

void ports_init(void);

#endif






