/*
 * 	sensor.h
 */

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "mytypes.h"
#include "ioports.h"


#define LOCK1S_MASK	0x01
#define PLG1S_MASK	0x02
#define LOCK2S_MASK	0x04
#define PLG2S_MASK	0x08

#define VAL1ST_MASK	0x40
#define VAL2ST_MASK	0x80

#define KEYS_MASK	0x04

/* 
 * Auxiliars Input Redefinition
 */
#define LOCK3		AUXIO3
#define D_LOCK3		D_AUXIO3
#define P_LOCK3		P_AUXIO3
#define LOCK3S		AUXIO4
#define D_LOCK3S	D_AUXIO4
#define P_LOCK3S	P_AUXIO4
#define DOOR3S		AUXIO5
#define D_DOOR3S	D_AUXIO5
#define P_DOOR3S	P_AUXIO5

#define LOCK4		AUXIO2
#define D_LOCK4		D_AUXIO2
#define P_LOCK4		P_AUXIO2
#define LOCK4S		AUXIO1
#define D_LOCK4S	D_AUXIO1
#define P_LOCK4S	P_AUXIO1
#define DOOR4S		AUXIO0
#define D_DOOR4S	D_AUXIO0
#define P_DOOR4S	P_AUXIO0

#define LOCK3S_MASK	0x20
#define DOOR3S_MASK	0x40
#define LOCK4S_MASK	0x04
#define DOOR4S_MASK	0x02

#define AUXILIAR_IN	(PTBD&0x7E)

void init_sensors( void );
void proc_sensors( void );

MUInt is_key_switch_open( void );
#endif
