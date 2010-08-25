/*
 *	gsqueue.h
 *		This module contain tha basic routines
 *		to management a simple queue.
 */
#ifndef __GSQUEUE__H__
#define __GSQUEUE__H__

#include "mytypes.h"

/*
 * The return codes
 */
typedef enum
{
	OK_QUEUE, EMPTY_QUEUE, FULL_QUEUE
} QCODES;

typedef MUInt QHDLR;
typedef MUInt ELEM_TYPE;

typedef struct
{
	ELEM_TYPE *queue;
	ELEM_TYPE *pout;
	ELEM_TYPE *pin;
	MUInt elem_qty;
	MUInt num_elems;
} GSQUEUE;


MUInt is_empty_gsqueue( QHDLR qh );
MUInt get_gsq_qty( QHDLR qh );
MInt remove_gsqueue( QHDLR qh, ELEM_TYPE *elem );
MInt put_gsqueue( QHDLR qh, ELEM_TYPE elem );
void deplete_gsqueue( QHDLR qh );
	
#endif
