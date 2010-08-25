/*
 * logprot.h
 */

#ifndef __LOGPROT_H__
#define __LOGPROT_H__

#include "mytypes.h"

#include "cscmds.h"
#include "csdevs.h"
#include "csframe.h"


/*
 * 		It is used to store the frame to send
 * 		to lower layers of protocol.
 */

typedef struct
{
	unsigned char buff[ MAX_FRAME_SIZE ];
	ushort qty;
} BUFF_T;

typedef struct
{
	unsigned char buff[ VAL_MAX_FRAME_SIZE ];
	ushort qty;
} VAL_BUFF_T;

typedef struct
{
	uchar *buff;
	ushort qty;
} XMIT_BUFF_T;

/*
 * 		Defines the size of number of payload's data.
 */

typedef unsigned short NDATA_T;

/*
 * 		Maintains the received frame's fields
 * 		in order for processing it into logical
 * 		layer of protocol.
 */

typedef struct
{
	unsigned char curr_dev;
	unsigned char curr_cmd;
	NDATA_T curr_ndata;
	unsigned char payload[ MAX_NDATA_LEN ];
} RFRAME_T;

/*
 * init_logprot:
 *
 * 		Initialize this module
 */

void init_logprot( void );

/*
 * proc_rcv_frame:
 *
 * 		Process received frame according to received device
 */

void proc_rcv_frame( RFRAME_T *p );

/*
 * proc_rcv_cmd:
 *
 *		Is called after receiving a command from CT.
 *		Any protocol's commands are processed by a function.
 */

void proc_rcv_cmd( RFRAME_T *p );

/*
 * get_curr_val:
 *
 * 		Return a current validator 
 */

MUInt get_curr_val( void );

/*
 * send_nack_resp:
 *
 *		This action is called when received frame is corrupted
 */

void send_nack_resp( MUInt dev );

/*
 * prepare_resp_sbox:
 * 	
 * 		Prepare a frame to send to CT device
 */

XMIT_BUFF_T *prepare_resp_sbox( MUInt dev, MUInt cmd, ushort ndata, void *p );

/*
 * prepare_resp_val:
 *
 * 		Prepare a frame to send to validator 
 */

XMIT_BUFF_T *prepare_resp_val( RFRAME_T *p );

/*
 * send_sbox_frame:
 *
 * 		Send frame passed by 'p' pointer to CT device
 */

void send_sbox_frame( XMIT_BUFF_T *p );

/*
 * exec_logprot:
 *
 *		This function is called from main program or receiving UART ISR.
 *		Must be passed UART channel as parameter for selecting
 *		receiving queue.
 */

void exec_protocol( MUInt ch );

/*
 * get_xmit_frames:
 *
 * 		Returns the number of transmitted frames to master side.
 */

uint get_xmit_frames( void );

/*
 * inc_xmit_frames:
 *
 * 		Increment transmitted frame counter.
 */

void inc_xmit_frames( void );


#endif
