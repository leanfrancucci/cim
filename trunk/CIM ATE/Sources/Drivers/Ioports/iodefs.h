/*
 * iodefs.h
 */

#ifndef __IODEFS_H__
#define __IODEFS_H__

#include "mytypes.h"

typedef struct 
{
	char* alias;
	char* number;
	char* hightxt;
	char* lowtxt;
	MUInt mask;
	MUInt dir;
} PIN_TABLE;

typedef struct 
{
	char* alias;
	char* number;
	MUInt mcu_port;
	const PIN_TABLE *ppintbl;
} PORT_TABLE;

enum
{
	INPUT,
	OUTPUT,
	ADC
};

#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80

extern const PIN_TABLE porta_pintbl[];
extern const PIN_TABLE portb_pintbl[];
extern const PIN_TABLE portc_pintbl[];
extern const PORT_TABLE port_tbl[];

#endif
