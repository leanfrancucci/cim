/*
 * nqdata.h
 */
#ifndef __NQDATA_H__
#define __NQDATA_H__

#include "nqueue.h"
#include "news.h"

/*
 * Define queue handler according to user
 */

enum
{
	EMER_QUEUE, COND_QUEUE,	NEWS_QUEUE, 
	
	NUM_NQUEUES
};

extern NQUEUE nqueues[ NUM_NQUEUES ];

#endif
