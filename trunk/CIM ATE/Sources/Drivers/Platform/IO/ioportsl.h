/*
 *	ioports.h
 *	
 *	Routines for PORTS initialization.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#ifndef __IOPORTSL_H__
#define __IOPORTSL_H__

//#include "derivative.h"
#include "porthdl.h"

typedef struct
{
	unsigned char data;
	unsigned char dir;
	unsigned char pup;
}PORT_STR;

void ports_init(void);
void refresh_ports( void );

extern PORT_STR ports_str[NUM_PORTS];

#define PORT0_DATA	PTAD
#define PORT0_DIR	PTADD
#define PORT0_PUP	PTAPE

#define PORT1_DATA	PTBD
#define PORT1_DIR	PTBDD
#define PORT1_PUP	PTBPE

#define PORT2_DATA	PTCD
#define PORT2_DIR	PTCDD
#define PORT2_PUP	PTCPE

#define PORT3_DATA	PTDD
#define PORT3_DIR	PTDDD
#define PORT3_PUP	PTDPE

#define PORT4_DATA	PTED
#define PORT4_DIR	PTEDD
#define PORT4_PUP	PTEPE

#define PORT5_DATA	PTFD
#define PORT5_DIR	PTFDD
#define PORT5_PUP	PTFPE

#define PORT6_DATA	PTGD
#define PORT6_DIR	PTGDD
#define PORT6_PUP	PTGPE

#endif






