/*
 * 		cparse.c
 */

#include "mytypes.h"

#include "cparse.h"
#include "cpdata.h"

/*
 * 		Type definitions
 */

/*
 * 	Maintenance data
 */

enum
{
	LOCKS_DOOR1, PLGS_DOOR1,
	LOCKS_DOOR2, PLGS_DOOR2,
	VAL1_STACKER, VAL2_STACKER,
	KEY_SWITCH, 
	LOCKS_DOOR3, LOCKS_DOOR4,
	PLGS_DOOR3, PLGS_DOOR4,
};

static CPDATA cpdata[] =
{
	{ (CPARSE *)senspar, 1, CLOSE_ST, 0, LOCKS_DOOR1 },
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, PLGS_DOOR1  },
	{ (CPARSE *)senspar, 1, CLOSE_ST, 0, LOCKS_DOOR2 },
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, PLGS_DOOR2  },
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, VAL1_STACKER},
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, VAL2_STACKER},
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, KEY_SWITCH  },
	{ (CPARSE *)senspar, 1, CLOSE_ST, 0, LOCKS_DOOR3 },
	{ (CPARSE *)senspar, 1, CLOSE_ST, 0, LOCKS_DOOR4 },
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, PLGS_DOOR3  },
	{ (CPARSE *)senspar, 0, CLOSE_ST, 0, PLGS_DOOR4  }
};

#define MAX_PARSER (sizeof(cpdata)/sizeof(*cpdata))

/*
 *	Static pointers
 *	for action routines
 */

static CPDATA *qcp;
static CPARSE *pcp;

/*
 * 		Static functions
 */

/*
 * 		Execute parser
 * 			Executes transition
 * 			for 'which' news based
 * 			in static pointers
 */

#include "reset.h"

static
MUInt
execute_parser( MUInt hdlr, MUInt which )
{
	static CPARSE *p1;

	p1 = pcp + qcp->state * NUM_ENTRIES + which;

	(*p1->act)();

	if( p1->ix_tout != NORELOAD )
	{
		if( p1->ix_tout != TNOTOUT )
			qcp->tout = sens_tout[hdlr][p1->ix_tout];
		else
			qcp->tout = 0;
	}
	qcp->state	= p1->next_state;
	return qcp->state;
}

/*
 * 		Public functions
 */

/*
 *		init_cparse
 *			Initialize this module
 */

void
init_cparse( MUInt shdlr, MUInt hnews )
{
	CPDATA *p;
	MUInt state;
	
	hnews = hnews != 0;
	p = &cpdata[shdlr];
	if( p->last_value != hnews )
	{
		state = ( p->state == CLOSE_ST ) ? OPEN_ST : CLOSE_ST;
		p->state = state;
		p->last_value = hnews;
	}
}

/*
 * 		do_cparse:
 * 			Parses 'hnews' of 'cphdlr'
 * 			parser. If hnews same as
 * 			last one, only decrements timer
 * 			if != 0 and parses if timer fall
 * 			to 0
 */

MInt
do_cparse( MUInt shdlr, MUInt hnews )
{
	hnews = hnews != 0;

	qcp = &cpdata[ shdlr ];
	pcp = qcp->ptbl;

	if( qcp->last_value != hnews )
	{
		qcp->last_value = hnews;
		execute_parser( shdlr, HNEW );
		return qcp->state;
	}

	if( qcp->tout != 0 && --qcp->tout == 0 )
		execute_parser( shdlr, HTIME );
	return qcp->state;
}


/*
 * 		close_cparse:
 * 			Closes parser opened
 * 			under 'cphdlr'
 */

#ifdef  CLOSE_DRIVERS

MInt
close_cparse( MUInt cphdlr )
{
	if( cpdata[ cphdlr ].ptbl == NULL )
		return -CP_NOT_OPEN;
	cpdata[ cphdlr ].ptbl = NULL;
	return CPOK;
}
#endif

MUInt
get_cstate( MUInt cphdlr )
{
	return cpdata[cphdlr].state;
}

MUInt
which_sensor( void )
{
	return qcp->which;
}

MUInt
is_key_switch_open( void )
{
	return (cpdata[KEY_SWITCH].state == OPEN_ST);
}

