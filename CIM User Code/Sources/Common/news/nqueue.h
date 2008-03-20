/*
 *	nqueue.h
 */

#ifndef __NQUEUE__H__
#define __NQUEUE__H__

#include "mytypes.h"
#include "news.h"

/*
 * The return codes
 */

enum
{
	OK_NQUEUE, EMPTY_NQUEUE, FULL_NQUEUE
};

typedef struct
{
	NEWS_T *queue;
	NEWS_T *pout;
	NEWS_T *pin;
	MUInt elem_qty;
	MUInt num_elems;
} NQUEUE;

MUInt is_empty_nqueue( MUInt qh );
MInt remove_nqueue( MUInt qh, NEWS_T *elem );
MInt put_nqueue( MUInt qh, NEWS_T elem );
	
#endif
