/*
 *	grqueue.h
 *		This module contain tha basic routines
 *		to management a simple queue.
 */
#ifndef __GRQUEUE__H__
#define __GRQUEUE__H__

#include "mytypes.h"
#include "grstatus.h"

#define GRQ_MAX_ELEMENTS 16
/*
 * The return codes
 */
typedef enum
{
	OK_GQUEUE, EMPTY_GQUEUE, FULL_GQUEUE
} QCODES;

typedef struct
{
	GR1ST_T *queue;
	GR1ST_T *pout;
	GR1ST_T *pin;
	MUInt elem_qty;
	MUInt num_elems;
} GRQUEUE;

MUInt is_empty_grqueue( void );
MUInt get_grq_qty( void );
MInt remove_grqueue( GR1ST_T *elem );
MInt put_grqueue( GR1ST_T *elem );
void deplete_grqueue( void );
	
#endif
