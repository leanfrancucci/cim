/*
 * qdata.h
 */
#ifndef __QDATA_H__
#define __QDATA_H__

#include "gsqueue.h"
#include "serdefs.h"

/*
 * Define queue handler according to user
 */
typedef enum
{
	COM1_QUEUE, COM2_QUEUE,
	NUM_QUEUES
}Q_HDLER;

extern GSQUEUE gsqueues[ NUM_QUEUES ];

#endif
