/*
 * alarm.h
 */

#ifndef __ALARM_H__
#define __ALARM_H__

#include "mytypes.h"

/*
 * Alarm control
 */

enum
{
	ENABLE, DISABLE
};

/*
 * set_alarm:
 * 	Active or deactive a desired alarm.
 * 	The device number is extracted from 
 * 	devices table.
 */

void set_alarm( MUInt which, MUInt ctrl );

#endif
