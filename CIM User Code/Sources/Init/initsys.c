/**
 * initsys.c
 */

/*---- Hardware module includes ----*/

#include "drivers.h"
#include "dispatch.h"
#include "vaultdef.h"
#include "dfdata.h"
#include "dfmanage.h"
#include "dfsys.h"
#include "users.h"
#include "dfilsys.h"
#include "pwrsys.h"
#include "settings.h"
#include "sleep.h"
#include "initsys.h"
#include <stdlib.h>
#include "mpdata.h"

/*---- Software module includes ----*/

/**
 * init_system:
 * 	Called after processor reset.
 */
void
init_system( void )
{
	static MUInt mcu_id;
	static MUInt installed_mem;

	/*---- Hardware initialization ----*/

	mcu_id = drivers_init();

	if( mcu_id == SECONDARY_DEVICE )
	{
		sleep( SECMCU_BOOT_DELAY );
		primary_shut_down();
	}

	init_mparsers( PWRCTRL_CTRL );	

	installed_mem = dfmanage_init( mcu_id );

	if( mcu_id == MAIN_DEVICE && ( installed_mem & MAIN_MEM_FAIL ) != MAIN_MEM_FAIL ||
			mcu_id == SECONDARY_DEVICE && ( installed_mem & SEC_MEM_FAIL ) != SEC_MEM_FAIL )
		users_init( NULL );


	if( mcu_id == MAIN_DEVICE && ( installed_mem & MAIN_MEM_FAIL ) != MAIN_MEM_FAIL )
		dfilesys_init( NULL, NULL );

	/*---- Software initialization ----*/
	init_dispatcher();

}
