/*
 * pwrsys.h
 */

#ifndef __PWRSYS_H__
#define __PWRSYS_H__

#include "mytypes.h"

/*
 * Operating power system
 */

enum
{
	EXT, BACKUP
};

/*
 * Battery status
 */

enum
{
	BATTLOW, BATTREM, BATTOK
};

/*
 * Host Power control definitions
 */
enum
{
	HOST_ON, HOST_OFF
};

/*
 * init_pwrsys:
 * 	The system always start from external power
 * 	by hardware design reason. This function is
 * 	called on cold boot for loading the battery 
 * 	status and operating power system.
 */

MUInt init_pwrsys( void );

/*
 * set_batt_st:
 * 		BATTLOW/BATTREM/BATTOK
 */
void set_batt_st( MUInt state );

/*
 * set_pwr_st:
 * 		EXT/BACKUP
 */
void set_pwr_st( MUInt state );

/*
 * shutdown:
 * 	Force a system shutdown, now.
 */

void shutdown( void );

/*
 * Host Reset
 */
void host_reset( void );

/*
 * Force Primary shutdown from Secondary MCU
 */
void primary_shut_down( void );

unsigned char set_memstat( unsigned char safe_stat );


#endif
