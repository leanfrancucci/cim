/*
 * 		cpdata.h
 * 			Prototypes for
 * 			cpdata.c 
 */

#ifndef __CPDATA_H__
#define __CPDATA_H__

#include "cparse.h"

#define MAX_PARSERS	NUM_SENSORS

#define NONEWS		-1
#define TNOTOUT		-1
#define NORELOAD	-2

enum
{
	IX_OPEN_TIME, IX_CLOSE_TIME, NUM_TIMEVAL
};

enum
{
	OPEN_NEWS, CLOSE_NEWS, NUM_NEWSTYPE
};

/*
 * 		States of Sensor Automata
 */

#define SENSOR_BASE		2

#define	LOCKS1_OPENT	100/SENSOR_BASE
#define LOCKS1_CLOSET	100/SENSOR_BASE
#define	PLGS1_OPENT		100/SENSOR_BASE
#define	PLGS1_CLOSET	100/SENSOR_BASE
#define	LOCKS2_OPENT	100/SENSOR_BASE
#define	LOCKS2_CLOSET	100/SENSOR_BASE
#define	PLGS2_OPENT		100/SENSOR_BASE
#define	PLGS2_CLOSET	100/SENSOR_BASE
#define	LOCKS3_OPENT	100/SENSOR_BASE
#define	LOCKS3_CLOSET	100/SENSOR_BASE
#define	PLGS3_OPENT		100/SENSOR_BASE
#define	PLGS3_CLOSET	100/SENSOR_BASE
#define	LOCKS4_OPENT	100/SENSOR_BASE
#define	LOCKS4_CLOSET	100/SENSOR_BASE
#define	PLGS4_OPENT		100/SENSOR_BASE
#define	PLGS4_CLOSET	100/SENSOR_BASE
#define	VAL1STCK_OPENT	100/SENSOR_BASE
#define	VAL1STCK_CLOSET	100/SENSOR_BASE
#define	VAL2STCK_OPENT	100/SENSOR_BASE
#define	VAL2STCK_CLOSET	100/SENSOR_BASE

#define	KEYSWITCH_OPENT	100/SENSOR_BASE
#define	KEYSWITCH_CLOSET 100/SENSOR_BASE

enum
{
	CLOSE_ST,
	OPENING_ST,
	OPEN_ST,
	CLOSING_ST,
	SNUM
};

extern const CPARSE senspar[SNUM][NUM_ENTRIES];
extern const short sens_tout[MAX_PARSERS][NUM_TIMEVAL]; 

#endif
