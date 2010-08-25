/*
 *	rti.h
 *		Routines handle RTI service
 *
 *	Programmer: Dar�o S. Bali�a.
 *	Date : 26/05/06.
 */

#ifndef __RTI_H__
#define __RTI_H__

/*
 *	MSEQBASE: Real Time Interrupt Base Time in msecs.
 *				
 *				
 */

enum
{
	RTI_DISABLE, RTI_ENABLE
};

void rti_ctrl( MUInt ctrl );


#endif



