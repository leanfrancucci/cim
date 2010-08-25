/*
 * 		cparse.h
 */


#ifndef __CPARSE_H__
#define __CPARSE_H__

#include "mytypes.h"

typedef struct
{
	unsigned char next_state;
	void (*act)( void );
	char ix_tout;
} CPARSE;

typedef struct
{
		/*	Constant data	*/
	CPARSE  *ptbl;
		/*	Variable data	*/
	unsigned char last_value;
	unsigned char state;
	unsigned short tout;
	unsigned char which;
} CPDATA;

enum
{
	HNEW, HTIME, NUM_ENTRIES
};

enum
{
	CPOK,
	CP_NO_ROOM, CP_NOT_OPEN,
	CP_BAD_PTR = 100, CP_BAD_INIT, CP_BAD_HDLR, CP_BAD_NEWS
};

void init_cparse( MUInt shdlr, MUInt hnews );


MInt do_cparse( MUInt cphdlr, MUInt news );

#ifdef  CLOSE_DRIVERS
	MInt close_cparse( MUInt cphdlr );
#else
	#define close_cparse( cphdlr )
#endif

MUInt get_cstate( MUInt cphdlr );

MUInt which_sensor( void );

/*	Predefined automata handlers */
enum
{
	LOCKS1, PLGS1, LOCKS2, PLGS2, 
	VAL1STCK, VAL2STCK, KEYSWITCH,
	LOCKS3, LOCKS4, PLGS3, PLGS4,
	NUM_SENSORS
};

#endif
