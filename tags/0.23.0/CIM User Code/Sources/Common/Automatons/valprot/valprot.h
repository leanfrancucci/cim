/*
 * valprot.h
 */

#ifndef __VALPROT_H__
#define __VALPROT_H__

/*
 * Validator's frame:
 *
 * 	NoBytes:	|	  1	   |  1  |	1  | 0..250 |  2  |
 * 	
 * 				| VAL_SYNC | LNG | CMD |  DATA  | CHK |
 *
 */

#define VAL_SYNC		0xFC


#endif
