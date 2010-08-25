/*
 * logprot.c
 */

#include "mytypes.h"
#include "utils.h"
#include "news.h"
#include "serdefs.h"
#include "mpdata.h"
#include "nqueue.h"
#include "nqdata.h"
#include "logprot.h"
#include "stimers.h"
#include "phylay.h"
#include "settings.h"
#include "alarm.h"
#include "locker.h"
#include "pwrsys.h"
#include "grstatus.h"
#include "sersw.h"
#include "users.h"
#include "vaultdef.h"
#include "pwrhdl.h"
#include "qdata.h"
#include "serial.h"
#include "serlog.h"
#include "valconf.h"
#include "reset.h"

#include "dfdata.h"
#include "dfmanage.h"
#include "dfsys.h"
#include "dfilsys.h"
#include "dfvars.h"
#include "sci.h"

#include <string.h>


/*
 * 		Used by exec_protocol function for removing
 * 		MAX_REMOVINGS bytes.
 */

#define MAX_REMOVINGS		1
#define MAX_READ_BYTES		250
#define MAX_WRITE_BYTES		250

#define send_success(x)		send_sbox_frame( prepare_resp_sbox \
								( (x), SBOX_SUCCESS, 0, NULL ) )
#define send_failure(x)		send_sbox_frame( prepare_resp_sbox \
								( (x), SBOX_FAILURE, 0, NULL ) )

enum
{
	NO_CHECK_MCU, CHECK_MCU
};

/*
 * 		Used by ADDUSR command
 */

typedef struct
{
	uchar user[ NUM_UID ];
	ushort dev;
	uchar pass0[ NUM_PASS ];
	uchar pass1[ NUM_PASS ];
	uchar user_log[ NUM_UID ];
	uchar pwd_log[ NUM_PASS ];
} ADD_T;

/*
 * 		Used by DELUSR command
 */

typedef struct
{
	uchar user[ NUM_UID ];
	uchar user_log[ NUM_UID ];
	uchar pwd_log[ NUM_PASS ];
} DEL_T;

/*
 * 		Used by EDITUSR command
 */

typedef struct
{
	uchar user[ NUM_UID ];
	uchar old_pwd[ NUM_PASS ];
	uchar new_pwd[ NUM_PASS ];
	uchar edit_pwd;
} EDIT_T;

/*
 * 		Used by GETUSRDEV command
 */

typedef struct
{
	uchar user[ NUM_UID ];
} GETDEV_T;

/*
 * 		Used by SETUSRDEV command
 */

typedef struct
{
	uchar user[ NUM_UID ];
	ushort dev_list;
	uchar user_log[ NUM_UID ];
	uchar pwd_log[ NUM_PASS ];
} SETDEV_T;

/*
 * 		Used by VALUSR command
 */

typedef struct
{
	uchar user[ NUM_UID ];
	uchar pwd[ NUM_PASS ];
} VAL_T;

/*
 * 		Used by to response from VALUSR command
 */

typedef struct
{
	ushort dev_list;
	uchar match;
} VALRESP_T;

/*
 * 		Used by to response from USRFORMAT command
 */

#if 0
typedef struct
{
	uchar user_log[ NUM_UID ];
	uchar pwd_log[ NUM_PASS ];
} USRFORMAT_T;
#endif

/*
 * 		Used by to response from GETUSRINFO command
 */

typedef struct
{
	uint max_users;
	uint free_users;
} GETINFO_T;

/*
 * 		Used by UNLOCK command
 */

typedef struct
{
	uchar pwd_type;
	uchar user0[ NUM_UID ];
	uchar pwd0[ NUM_PASS ];
	uchar user1[ NUM_UID ];
	uchar pwd1[ NUM_PASS ];
} UNLOCK_T;	

/*
 * 		Used by ACTRL command
 */

typedef struct
{
	uchar ctrl;
} ACTRL_T;	

/*
 * 		Used by CREATEFILE command
 */

typedef struct
{
	uchar df_desc;
	uchar unit_size;
	uchar access;
	long num_units;
} CREATEFILE_T;

/*
 * 		Used by to response from CREATEFILE command
 */

typedef struct
{
	MInt errno;
	long num_units;
} CREATE_RES_T;

/*
 * 		Used by STATUSFILE command
 */

typedef struct
{
	uchar df_desc;
} STATUSFILE_T;

/*
 * 		Used by SEEKFILE command
 */

typedef struct
{
	uchar df_desc;
	long offset;
	uchar whence;
} SEEKFILE_T;

/*
 * 		Used by CLOSEFILE command
 */

typedef struct
{
	uchar df_desc;
} CLOSEFILE_T;

/*
 * 		Used by REOPENFILE command
 */

typedef struct
{
	uchar df_desc;
} REOPENFILE_T;

/*
 * 		Used by REINITFILE command
 */

typedef struct
{
	uchar df_desc;
} REINITFILE_T;

/*
 * 		Used by READFILE command
 */

typedef struct
{
	uchar df_desc;
	uchar num_units;
} READFILE_T;

/*
 * 		Used by WRITEFILE command
 */

typedef struct
{
	uchar df_desc;
	uchar num_units;
	uchar buff[ MAX_WRITE_BYTES ];
} WRITEFILE_T;

/*
 * 		Used by to response from READFILE command
 */

typedef struct
{
	uchar errno;
	uchar buffer[ MAX_READ_BYTES ];
} READ_BUFF_T;


/*
 * 		Used by TLOCK command
 */

typedef struct
{
	uchar tlock0;
	uchar tlock1;
	uchar tlock2;
	uchar tlock3;
} TLOCK_T;

/*
 * 		Used by TUNLOCK_ENABLE command
 */

typedef struct
{
	uchar tunlock_enable;
} TUNLOCK_ENABLE_T;

/*
 * 		Used by FORCEUSRPASS command
 */

typedef struct
{
	uchar user[ NUM_UID ];
	uchar new_pwd[ NUM_PASS ];
	ushort num_frames;
} FORCE_USRFORMAT_T;

/*
 * 		Used by HOSTPWR command
 */

typedef struct
{
	uchar ctrl;
} HOSTPWR_T;

/*
 * 		Used by TUNLOCK_ENABLE_LOCKS command
 */

typedef struct
{
	uchar tunlock_enable_lock0;
	uchar tunlock_enable_lock1;
	uchar tunlock_enable_lock2;
	uchar tunlock_enable_lock3;
} TUNLOCK_ENABLE_LOCKS_T;

/*
 * 
 * 		Used by VALPWR command
 */

typedef struct
{
	uchar who;
	uchar pwr;
} VALPWR_T;


/*
 *		Used to check valid user device.
 */

typedef struct
{
	ushort mask;
	uchar devno;
} DVL_T;


static XMIT_BUFF_T xframe;		/* is shared with receiving side */
extern RFRAME_T rcv_frame;		/* is shared with receiving side */

static NEWS_T in;
static MUInt curr_val;
static MInt txmit0;
static USER_T user0, user1;
static FILESYS_T sta;

/*
 * 		Maintains the number of transmitted frames.
 */

static uint xmit_frames;


#define proc_dev_locker			proc_dev_sbox
#define proc_dev_plunger		proc_dev_sbox
#define proc_dev_locker			proc_dev_sbox
#define proc_dev_plunger		proc_dev_sbox
#define proc_dev_alarm			proc_dev_sbox

typedef void (*PFR)( RFRAME_T *p );

static void proc_unlock( RFRAME_T *p ),
			proc_lock( RFRAME_T *p ),
			proc_actrl( RFRAME_T *p ),
			proc_addusr( RFRAME_T *p ),
			proc_delusr( RFRAME_T *p ),
			proc_editusr( RFRAME_T *p ),
			proc_getusrdev( RFRAME_T *p ),
			proc_setusrdev( RFRAME_T *p ),
			proc_valusr( RFRAME_T *p ),
			proc_getusrinfo( RFRAME_T *p ),
			proc_usrformat( RFRAME_T *p ),
			proc_version( RFRAME_T *p ),
			proc_model( RFRAME_T *p ),
			proc_shutdown( RFRAME_T *p ),
			proc_gr1_status( RFRAME_T *p ),
			proc_blankdfs( RFRAME_T *p ),
			proc_create_file( RFRAME_T *p ),
			proc_status_file( RFRAME_T *p ),
			proc_read_file( RFRAME_T *p ),
			proc_write_file( RFRAME_T *p ),
			proc_seek_file( RFRAME_T *p ),
			proc_not_assigned( RFRAME_T *p ),
			proc_reinit_file( RFRAME_T *p ),
			proc_get_dfs_info( RFRAME_T *p ),
			proc_tlock( RFRAME_T *p ),
			proc_tunlock_enable( RFRAME_T *p ),
			proc_val_config( RFRAME_T *p ),
			proc_forceusrpass( RFRAME_T *p ),
			proc_syncnumframes( RFRAME_T *p ),
			proc_hostpwr( RFRAME_T *p ),
			proc_resetmcu( RFRAME_T *p ),
			proc_tunlock_enable_locks( RFRAME_T *p ),
			proc_valpwr( RFRAME_T *p );
			
static void proc_dev_val( RFRAME_T *p ),
			proc_dev_sbox( RFRAME_T *p );

/*
 * 		Must be defined one function for each device
 * 		to start of processing received frame.
 *
 * 		Note: must be perfectly linked with DEV_T.
 */

static const PFR pdevs[] =
{
	proc_dev_val, 
	proc_dev_val,
	proc_dev_locker,
	proc_dev_plunger,
	proc_dev_locker,
	proc_dev_plunger,
	proc_dev_alarm,
	proc_dev_alarm,
	proc_dev_sbox,
	NULL,
	NULL,
	proc_dev_locker,
	proc_dev_locker,
	proc_dev_plunger,
	proc_dev_plunger,
	NULL
};

/*
 * 		Must be defined one function for each command
 *
 * 		Note: must be perfectly linked with CMD_T.
 */

static const PFR pcmds[] =
{
	NULL, 
	proc_unlock,
	proc_lock,
	proc_actrl,
	proc_addusr,
	proc_delusr,
	proc_editusr,
	proc_getusrdev,
	proc_setusrdev,
	proc_valusr,
	proc_getusrinfo,
	proc_usrformat,
	proc_version,
	proc_model,
	proc_shutdown,
	proc_gr1_status,
	proc_blankdfs,
	proc_create_file,
	proc_status_file,
	proc_read_file,
	proc_write_file,
	proc_seek_file,
	proc_not_assigned,
	proc_not_assigned,
	proc_reinit_file,
	proc_get_dfs_info,
	proc_tlock,
	proc_tunlock_enable,
	NULL,
	NULL,
	NULL,
	proc_val_config,
	proc_forceusrpass,
	proc_syncnumframes,
	proc_hostpwr,
	proc_resetmcu,
	proc_tunlock_enable_locks,
	proc_valpwr
};


static const DVL_T dvlist[] =
{
	{ LOCKER0_MASK, LOCKER0	},
	{ LOCKER1_MASK, LOCKER1	},
	{ LOCKER2_MASK, LOCKER2	},
	{ LOCKER3_MASK, LOCKER3	}
};


static
MUInt
search_dev_in_list( uchar rcv_dev, ushort user_devs )
{
	MUInt ix;
	DVL_T *pdv;

	for( ix = 0, pdv = dvlist; ix < NUM_DEVS_LIST; ++ix, ++pdv )
		if( pdv->devno == rcv_dev )
			return ( pdv->mask & user_devs ) == 0;

	return 1;
}


/*
 * get_xmit_frames:
 *
 * 		Returns the number of transmitted frames to master side.
 */

#define get_xmit_frames()			xmit_frames

/*
 * inc_xmit_frames:
 *
 * 		Increment transmitted frame counter.
 */

#define inc_xmit_frames()			++xmit_frames

/*
 * set_xmit_frames:
 *
 * 		Set transmitted frame counter.
 */

#define set_xmit_frames(x)			xmit_frames=(x)


/*
 * send_val_frame:
 *
 * 		Select desired validator and send it a frame
 */

static
void
send_val_frame( MUInt val, XMIT_BUFF_T *p )
{
	curr_val = val;
	put_nchar( VALCH, p->buff, p->qty );
}

/* ------------ section: command processing ------------ */

/*
 * verify_installed_memory:
 */

static
MUInt
verify_installed_memory( MUInt memory, MUInt dev )
{
	MInt r;
	uchar error_found;
  
	error_found = 0;

	switch( memory )
	{
		case MAIN_DEVICE:
			if( ( mem_status & MAIN_MEM_FAIL ) == MAIN_MEM_FAIL )
				error_found = 1;
			break;
		case SECONDARY_DEVICE:
			if( ( mem_status & SEC_MEM_FAIL ) == SEC_MEM_FAIL )
				error_found = 1;
			break;
		default:
			if( mem_status != BOTH_MEM_OK )
				error_found = 1;
			break;
	}

	if( error_found )
	{
		r = ERR_USER_COMM_NOT_IN_EMERG;
		send_sbox_frame( prepare_resp_sbox( dev, SBOX_FAILURE, 1, &r ) );
		return 1;
	}
	return 0;
}

/*
 * verify_memory_and_mcu:
 */

static
MUInt
verify_memory_and_mcu( MUInt dev )
{
	MInt r;
	uchar error_found;

	error_found = 0;

	if( get_mcu_id() == MAIN_DEVICE )
	{
	   	if( ( mem_status & MAIN_MEM_FAIL ) == MAIN_MEM_FAIL )
			error_found = 1;
	}
	else 
	{
	   	if( ( mem_status & SEC_MEM_FAIL ) == SEC_MEM_FAIL )
			error_found = 1;
	}

	if( error_found )
	{
		r = ERR_USER_COMM_NOT_IN_EMERG;
		send_sbox_frame( prepare_resp_sbox( dev, SBOX_FAILURE, 1, &r ) );
		return 1;
	}
	return 0;
}

/*
 *	verify_content_mcu:
 *
 * 		If size of payload does not exactly match with
 *		expected size then it return 1 else if the operating MCU 
 *		is 'SECONDARY_DEVICE' then	all command from CTSAFE protocol
 *		must be ignored	with 'ERR_USER_COMM_NOT_IN_EMERG' and
 * 		error code within failure frame.
 */

static
MUInt
verify_content_mcu( uchar check_mcu, RFRAME_T *p, ushort size )
{
	MInt r;
  
	if( p->curr_ndata != size )
	{
		r = ERR_INVALID_CONTENT;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
		return 1;
	}

	if( check_mcu == CHECK_MCU && get_mcu_id() == SECONDARY_DEVICE )
	{
		r = ERR_USER_COMM_NOT_IN_EMERG;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
		return 1;
	}
	return 0;
}

/*
 * verify_to_respond:
 *
 * 		This function is used to verify a result code 
 * 		from previous call and send response according to.
 */

static
MUInt
verify_to_respond( MUInt dev, int result, void *to_send, ushort qty )
{
	int r;
	uchar errno;
	
	r = result;
	
	if( r < 0 )
	{
		errno = (uchar)-r;
		send_sbox_frame( prepare_resp_sbox( dev, SBOX_FAILURE, 1, &errno ) );
		return 1;
	}
	send_sbox_frame( prepare_resp_sbox( dev, SBOX_SUCCESS, qty, to_send ) );
	return 0;
}

static
MUInt
log_user_and_password( uchar dev, uchar *user, uchar *pwd  )
{
	MInt r;
	uchar match, errno;
	GETINFO_T get_info;

	get_user_information( &get_info.max_users, &get_info.free_users );

	if( get_info.max_users != get_info.free_users )
	{
		memcpy( user0.user_id, user, NUM_UID );
		memcpy( user0.pass0, pwd, NUM_PASS );

		if( ( r = users_validation( &user0, &match, NULL ) ) < 0 )
		{
			errno = (uchar)-r;
			send_sbox_frame( prepare_resp_sbox( dev, SBOX_FAILURE, 1, &errno ) );
			return 1;
		}
	}
	return 0;
}

static
MUInt
validate_user_and_devlist( uchar dev, USER_T *user, uchar *pass_match, ushort *pdev_list )
{
	MInt r;

	if( ( r = users_validation( user, pass_match, pdev_list ) ) != USER_OK  )
	{
		verify_to_respond( dev, r, NULL, 0 );
		return 1;
	}

	if( search_dev_in_list( dev, *pdev_list ) )
	{
		r = -ERR_USER_BAD_DEV;
		verify_to_respond( dev, r, NULL, 0 );
		return 1;
	}

	return 0;
}

/*
 * get_curr_val:
 *
 * 		Return a current validator
 */

MUInt
get_curr_val( void )
{
	return curr_val;
}

/*
 * send_nack_resp:
 *
 *		This action is called when received frame is corrupted
 */

void
send_nack_resp( MUInt dev )
{
	send_sbox_frame( prepare_resp_sbox( dev, SBOX_NACK, 0, NULL ) );
}

/* ######### Begin section: command action routines ######### */

static
void 
proc_unlock( RFRAME_T *p )
{
	MInt r;
	MUInt match[ 2 ];
	ushort dev_list;
	UNLOCK_T *pload;
	
	pload = (UNLOCK_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, 
			pload->pwd_type ? sizeof( UNLOCK_T ) : 
				sizeof( UNLOCK_T ) - NUM_PASS - NUM_UID ) )
		return;

	if( verify_memory_and_mcu( p->curr_dev ) )
		return;

	memcpy( user0.user_id, pload->user0, NUM_UID );
	memcpy( user0.pass0, pload->pwd0, NUM_PASS );
	
	match[ 1 ] = 0;

	switch( pload->pwd_type )
	{
		case SINGLE:

			if( validate_user_and_devlist( p->curr_dev, &user0, &match[ 0 ], &dev_list ) )
				return;

			if( verify_to_respond( p->curr_dev, r, match, 2 ) )
				return;

			set_locker( p->curr_dev, TOUNLOCK );
			break;
		case DUAL:
			memcpy( user1.user_id, pload->user1, NUM_UID );
			memcpy( user1.pass0, pload->pwd1, NUM_PASS );

			if( validate_user_and_devlist( p->curr_dev, &user0, &match[ 0 ], &dev_list ) )
				return;

			if( validate_user_and_devlist( p->curr_dev, &user1, &match[ 1 ], &dev_list ) )
				return;

			if( verify_to_respond( p->curr_dev, r, match, 2 ) )
				return;

			set_locker( p->curr_dev, TOUNLOCK );
			break;
		default:
			r = ERR_INVALID_CONTENT;
			send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
			break;
	}
}

static
void 
proc_lock( RFRAME_T *p )
{
	if( verify_content_mcu( NO_CHECK_MCU, p, 0 ) )
		return;

	set_locker( p->curr_dev, TOLOCK );
	send_success( p->curr_dev );
}

static
void 
proc_actrl( RFRAME_T *p )
{
	ACTRL_T *pload;

	pload = (ACTRL_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( ACTRL_T ) ) )
		return;
	
	set_alarm( p->curr_dev, pload->ctrl );
	send_success( p->curr_dev );
}

static
void 
proc_addusr( RFRAME_T *p )
{
	MInt r;
	ADD_T *pload;
	
	pload = (ADD_T*)p->payload;
	
	if( verify_content_mcu( CHECK_MCU, p, sizeof( ADD_T ) ) )
		return;
	
	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	if( log_user_and_password( p->curr_dev, pload->user_log, pload->pwd_log ) )
		return;

	memcpy( &user0, pload, sizeof( ADD_T ) - NUM_UID - NUM_PASS );
	conv_endians( &user0.dev_list, &pload->dev, sizeof( ushort ) );
	
	r = users_add( &user0 );
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
}

static
void 
proc_delusr( RFRAME_T *p )
{
	MInt r;
	DEL_T *pload;
	
	pload = (DEL_T*)p->payload;
	
	if( verify_content_mcu( CHECK_MCU, p, sizeof( DEL_T ) ) )
		return;
	
	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	if( log_user_and_password( p->curr_dev, pload->user_log, pload->pwd_log ) )
		return;

	memcpy( user0.user_id, pload->user, NUM_UID );
	
	r = users_erase( user0.user_id );
	verify_to_respond( p->curr_dev, r, NULL, 0 );
}

static
void 
proc_editusr( RFRAME_T *p )
{
	MInt r;
	EDIT_T *pload;
	
	pload = (EDIT_T*)p->payload;
	
	if( verify_content_mcu( CHECK_MCU, p, sizeof( EDIT_T ) ) )
		return;
	
	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	memcpy( user0.user_id, pload->user, NUM_UID );
	memcpy( user0.pass0, pload->old_pwd, NUM_PASS );

	memcpy( user1.user_id, user0.user_id, NUM_UID );

	memcpy( pload->edit_pwd == EDIT_PASS0 ? user1.pass0 : user1.pass1,
		pload->new_pwd, NUM_PASS );
		
	r = users_edit( pload->edit_pwd, &user0, &user1, LOG_EDIT );
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
}

static
void 
proc_getusrdev( RFRAME_T *p )
{
	MInt r;
	GETDEV_T *pload;
	
	pload = (GETDEV_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( GETDEV_T ) ) )
		return;
	
	if( verify_memory_and_mcu( p->curr_dev ) )
		return;

	memcpy( user0.user_id, pload->user, NUM_UID );

	r = users_get_devlist( user0.user_id, &user0.dev_list );
	verify_to_respond( p->curr_dev, r, &user0.dev_list, sizeof( ushort ) );	
}

static
void 
proc_setusrdev( RFRAME_T *p )
{
	MInt r;
	SETDEV_T *pload;
	
	pload = (SETDEV_T*)p->payload;
	
	if( verify_content_mcu( CHECK_MCU, p, sizeof( SETDEV_T ) ) )
		return;
	
	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	if( log_user_and_password( p->curr_dev, pload->user_log, pload->pwd_log ) )
		return;

	memcpy( user0.user_id, pload->user, NUM_UID );
	conv_endians( &user0.dev_list, &pload->dev_list, sizeof( ushort ) );

	r = users_set_devlist( user0.user_id, user0.dev_list );
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
}

static
void 
proc_valusr( RFRAME_T *p )
{
	MInt r;
	VAL_T *pload;
	VALRESP_T vresp;
	
	pload = (VAL_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( VAL_T ) ) )
		return;
	
	if( verify_memory_and_mcu( p->curr_dev ) )
		return;

	memcpy( user0.user_id, pload->user, NUM_UID );
	memcpy( user0.pass0, pload->pwd, NUM_PASS );

	r = users_validation( &user0, &vresp.match, &vresp.dev_list );
	verify_to_respond( p->curr_dev, r, &vresp, sizeof( VALRESP_T ) );	
}

static
void 
proc_getusrinfo( RFRAME_T *p )
{
	int r;
	GETINFO_T get_info;

	if( verify_content_mcu( NO_CHECK_MCU, p, 0 ) )
		return;
	
	if( verify_memory_and_mcu( p->curr_dev ) )
		return;

	r = get_user_information( &get_info.max_users, &get_info.free_users );
	verify_to_respond( p->curr_dev, r, &get_info, sizeof( GETINFO_T ) );	
}

static
void 
proc_usrformat( RFRAME_T *p )
{
#if 0
	MInt r;
	USRFORMAT_T *pload;

	pload = (USRFORMAT_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( USRFORMAT_T ) ) )
		return;
	
	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	if( log_user_and_password( p->curr_dev, pload->user_log, pload->pwd_log ) )
		return;

	r = users_format();
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
#else
	MInt r;
	GR1ST_T *pcurr_grstatus;

	if( verify_content_mcu( CHECK_MCU, p, 0 ) )
		return;
	
	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	pcurr_grstatus = get_grstat();
	if( pcurr_grstatus->plunger0 != OPENED )
	{
		r = ERR_USRFORMAT_NOT_POSSIBLE;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
		return;
	}

	r = users_format();
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
#endif	
}

static
void 
proc_version( RFRAME_T *p )
{
	if( verify_content_mcu( NO_CHECK_MCU, p, 0 ) )
		return;

	send_sbox_frame( prepare_resp_sbox( p->curr_dev,
		SBOX_SUCCESS, sizeof( VERSION_STRING ), VERSION_STRING ) );
}

static
void 
proc_model( RFRAME_T *p )
{
#if 1
	MUInt model;

	if( verify_content_mcu( NO_CHECK_MCU, p, 0 ) )
		return;

	model = MODEL_CONFIG;
	send_sbox_frame( prepare_resp_sbox( p->curr_dev,
		SBOX_SUCCESS, 1, &model ) );
#endif
#if 0
	MInt r;

	r = ERR_INVALID_CONTENT;
	send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
#endif
}

static
void 
proc_shutdown( RFRAME_T *p )
{
	if( verify_content_mcu( CHECK_MCU, p, 0 ) )
		return;
	
	shutdown();
}

static
void 
proc_gr1_status( RFRAME_T *p )
{
	if( verify_content_mcu( NO_CHECK_MCU, p, 0 ) )
		return;

	send_sbox_frame( prepare_resp_sbox( p->curr_dev,
		SBOX_SUCCESS, GR1_DEVS, get_group_status() ) );
}

static
void
proc_blankdfs( RFRAME_T *p )
{
	if( verify_content_mcu( CHECK_MCU, p, 0 ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	dfilesys_blanking();
	send_success( p->curr_dev );
}

static
void
proc_create_file( RFRAME_T *p )
{
	long r;
	CREATEFILE_T *pload;
	CREATE_RES_T create_result;

	pload = (CREATEFILE_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( CREATEFILE_T ) ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	conv_endians( &create_result.num_units, &pload->num_units, sizeof( long ) );
	r = dfilesys_create_file( pload->df_desc, pload->unit_size, pload->access, &create_result.num_units );

	create_result.errno = (MInt)-r;

	if( create_result.errno == DFILE_OK )
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_SUCCESS, sizeof( ulong ), &create_result.num_units ) );
	else
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, sizeof( CREATE_RES_T ), &create_result ) );
}

static
void
proc_status_file( RFRAME_T *p )
{
	MInt r;
	STATUSFILE_T *pload;

	pload = (STATUSFILE_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( STATUSFILE_T ) ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	r = dfilesys_status_file( pload->df_desc, &sta );
	verify_to_respond( p->curr_dev, r, &sta, sizeof( FILESYS_T ) );	
}

static
void
proc_read_file( RFRAME_T *p )
{
	int r, unit_size;
	static READ_BUFF_T read_buff;
	uchar read_qty, max_units;
	READFILE_T *pload;

	pload = (READFILE_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( READFILE_T ) ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	unit_size = dfilsys_object_size( pload->df_desc ); 
	
	if( unit_size < 0 )
	{
		unit_size = -unit_size;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &unit_size ) );
		return;
	}

	max_units = (uchar)(MAX_READ_BYTES/unit_size);
	pload->num_units = pload->num_units > max_units ? max_units : pload->num_units;

	r = dfilesys_read_file( pload->df_desc, pload->num_units, read_buff.buffer );

	if( r >= 0 )
	{
		read_buff.errno = (uchar)r;
		read_qty = ( r * unit_size ) > MAX_READ_BYTES ? MAX_READ_BYTES : (uchar)(r * unit_size);   
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_SUCCESS, read_qty + 1, &read_buff ) );
	}
	else
	{
		read_buff.errno = (uchar)-r;
		if( read_buff.errno == ERR_DFILE_SECTOR_ERROR )
		{
			read_qty = ( r * unit_size ) > MAX_READ_BYTES ? MAX_READ_BYTES : (uchar)(r * unit_size);
			send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, read_qty + 1, &read_buff ) );
		}
		else
			send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &read_buff.errno ) );
	}
}

static
void
proc_write_file( RFRAME_T *p )
{
	int r, unit_size;
	uchar write_qty, max_units;
	WRITEFILE_T *pload;

	pload = (WRITEFILE_T*)p->payload;

	if( get_mcu_id() == SECONDARY_DEVICE )
	{
		r = ERR_USER_COMM_NOT_IN_EMERG;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
		return;
	}
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	unit_size = dfilsys_object_size( pload->df_desc ); 
	
	if( unit_size < 0 )
	{
		unit_size = -unit_size;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &unit_size ) );
		return;
	}

	max_units = (uchar)(MAX_WRITE_BYTES/unit_size);
	pload->num_units = pload->num_units > max_units ? max_units : pload->num_units;

	r = dfilesys_write_file( pload->df_desc, pload->num_units, pload->buff );


	write_qty = (uchar)r;
	verify_to_respond( p->curr_dev, r, &write_qty, 1 );
}

static
void
proc_seek_file( RFRAME_T *p )
{
	MInt r;
	SEEKFILE_T *pload;
	long offset;

	pload = (SEEKFILE_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( SEEKFILE_T ) ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	conv_endians( &offset, &pload->offset, sizeof( long ) );
	r = dfilesys_seek( pload->df_desc, offset, pload->whence );
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
}

static
void
proc_reinit_file( RFRAME_T *p )
{
	MInt r;
	REINITFILE_T *pload;

	pload = (REINITFILE_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( REINITFILE_T ) ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	r = dfile_reinit( pload->df_desc );
	verify_to_respond( p->curr_dev, r, NULL, 0 );	
}

static
void
proc_get_dfs_info( RFRAME_T *p )
{
	DFS_INFO_T *q;

	if( verify_content_mcu( CHECK_MCU, p, 0 ) )
		return;
	
	if( verify_installed_memory( MAIN_DEVICE, p->curr_dev ) )
		return;

	q = dfilesys_get_information();
	verify_to_respond( p->curr_dev, 0, q, sizeof( DFS_INFO_T ) );	
}

static
void 
proc_tlock( RFRAME_T *p )
{
	TLOCK_T *pload;

	pload = (TLOCK_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( TLOCK_T ) ) )
		return;
	
	if( pload->tlock0 != TLOCK_NOT_ALLOWED )
		set_tlock( LOCKER0, pload->tlock0 );
	if( pload->tlock1 != TLOCK_NOT_ALLOWED )
		set_tlock( LOCKER1, pload->tlock1 );

	if( pload->tlock2 != TLOCK_NOT_ALLOWED )
		set_tlock( LOCKER2, pload->tlock2 );
	if( pload->tlock3 != TLOCK_NOT_ALLOWED )
		set_tlock( LOCKER3, pload->tlock3 );

	send_success( p->curr_dev );
}

static
void 
proc_tunlock_enable( RFRAME_T *p )
{
	TUNLOCK_ENABLE_T *pload;

	pload = (TUNLOCK_ENABLE_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( TUNLOCK_ENABLE_T ) ) )
		return;
	
	if( pload->tunlock_enable != TUNLOCK_NOT_ALLOWED )
	{
		set_tunlock_enable( LOCKER0, pload->tunlock_enable );
		set_tunlock_enable( LOCKER1, pload->tunlock_enable );
		set_tunlock_enable( LOCKER2, pload->tunlock_enable );
		set_tunlock_enable( LOCKER3, pload->tunlock_enable );
	}
	send_success( p->curr_dev );
}

static
void 
proc_val_config( RFRAME_T *p )
{
	VAL_CONFIG_T *pload;

	pload = (VAL_CONFIG_T*)p->payload;
	
	if( verify_content_mcu( CHECK_MCU, p, sizeof( VAL_CONFIG_T )*2 ) )
		return;
	
	set_val_config( VAL0, pload );
	set_val_config( VAL1, pload + 1 );
	send_success( p->curr_dev );
}

static
void
proc_forceusrpass( RFRAME_T *p )
{
	MInt r;
	FORCE_USRFORMAT_T *pload;
	ushort num_frames;

	pload = (FORCE_USRFORMAT_T*)p->payload;

	if( verify_content_mcu( CHECK_MCU, p, sizeof( FORCE_USRFORMAT_T ) ) )
		return;

	if( verify_installed_memory( NUM_FLASH_DEVICES, p->curr_dev ) )
		return;

	conv_endians( &num_frames, &pload->num_frames, sizeof( short ) );

	if( get_xmit_frames() != num_frames )
	{
		r = ERR_USRFORMAT_NOT_POSSIBLE;
		send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
		return;
	}

	memcpy( user0.user_id, pload->user, NUM_UID );
	memcpy( user1.user_id, user0.user_id, NUM_UID );
	memcpy( user1.pass0, pload->new_pwd, NUM_PASS );

	r = users_edit( EDIT_PASS0, &user0, &user1, FORCE_EDIT );
	verify_to_respond( p->curr_dev, r, NULL, 0 );
}

static
void
proc_syncnumframes( RFRAME_T *p )
{
	send_success( p->curr_dev );
	set_xmit_frames( 0 );
}

static
void 
proc_hostpwr( RFRAME_T *p )
{
	HOSTPWR_T *pload;

	pload = (HOSTPWR_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( HOSTPWR_T ) ) )
		return;
	
	set_host_pwr( pload->ctrl );
	send_success( p->curr_dev );
}

static
void 
proc_resetmcu( RFRAME_T *p )
{
	if( verify_content_mcu( NO_CHECK_MCU, p, 0 ) )
		return;

	send_success( p->curr_dev );
	
	dfilesys_sync();

	cop_reset_now();
}

static
void 
proc_tunlock_enable_locks( RFRAME_T *p )
{
	TUNLOCK_ENABLE_LOCKS_T *pload;

	pload = (TUNLOCK_ENABLE_LOCKS_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( TUNLOCK_ENABLE_LOCKS_T ) ) )
		return;
	
	if( pload->tunlock_enable_lock0 != TUNLOCK_NOT_ALLOWED )
		set_tunlock_enable( LOCKER0, pload->tunlock_enable_lock0 );
	if( pload->tunlock_enable_lock1 != TUNLOCK_NOT_ALLOWED )
		set_tunlock_enable( LOCKER1, pload->tunlock_enable_lock1 );

	if( pload->tunlock_enable_lock2 != TUNLOCK_NOT_ALLOWED )
		set_tunlock_enable( LOCKER2, pload->tunlock_enable_lock2 );
	if( pload->tunlock_enable_lock3 != TUNLOCK_NOT_ALLOWED )
		set_tunlock_enable( LOCKER3, pload->tunlock_enable_lock3 );

	send_success( p->curr_dev );
}

static
void 
proc_valpwr( RFRAME_T *p )
{
	VALPWR_T *pload;

	pload = (VALPWR_T*)p->payload;
	
	if( verify_content_mcu( NO_CHECK_MCU, p, sizeof( VALPWR_T ) ) )
		return;
	
	set_valpwr( pload->who, pload->pwr );

	send_success( p->curr_dev );
}

static
void
proc_not_assigned( RFRAME_T *p )
{
	MInt r;

	r = ERR_INVALID_CONTENT;
	send_sbox_frame( prepare_resp_sbox( p->curr_dev, SBOX_FAILURE, 1, &r ) );
}

/* ######### End section: command action routines ######### */

/*
 * proc_rcv_cmd:
 *
 *		Is called after receiving a command from CT.
 *		Any protocol's commands are processed by a function.
 */

void
proc_rcv_cmd( RFRAME_T *p  )
{
	PFR q;
	
	if( ( q = pcmds[ p->curr_cmd ] ) != NULL )
		( *q )( p );
}

/*
 * proc_dev_val:
 *
 *		Send received frame from CT to desired validator.
 */

static
void 
proc_dev_val( RFRAME_T *p )
{
	deplete_gsqueue( VALRX_QUEUE );
	
	set_sersw( p->curr_dev == VAL0 ? CH0 : CH1 );
	set_val_hw_config( p->curr_dev );
	
	if( get_val_echo( p->curr_dev ) == VAL_ECHO_DISABLE )
		disable_rcv_interrupt( VALCH );

	put_nqueue( COND_QUEUE, def_news[ VALUNBLOCK_IX ] );
	send_val_frame( p->curr_dev, prepare_resp_val( p ) );
	put_nqueue( COND_QUEUE, def_news[ VALSENT_IX ] );

	enable_rcv_interrupt( VALCH );
}

/*
 * proc_dev_sbox:
 *
 * 		Trigger a timer for	waiting half-duplex
 * 		communication delay.
 */

static
void 
proc_dev_sbox( RFRAME_T *p )
{
	kick_timer( txmit0, TOUT_TXMIT0 );
}

/*
 * proc_rcv_frame:
 *
 * 		Process received frame according to received device
 */

void 
proc_rcv_frame( RFRAME_T *p )
{
	( *pdevs[ p->curr_dev ] )( p );
}

/*
 * send_sbox_frame:
 *
 * 		Send frame passed by 'p' pointer to CT device
 */

void
send_sbox_frame( XMIT_BUFF_T *p )
{
	inc_xmit_frames();
	put_nchar( CTCH, p->buff, p->qty );
}

/*
 * prepare_resp_sbox:
 *
 * 		Prepare a frame to send to CT device
 */

XMIT_BUFF_T *
prepare_resp_sbox( MUInt dev, MUInt cmd, ushort ndata, void *p )
{
	xframe.buff = rcv_frame.payload;	/* shared buffer */

	xframe.buff[ MARK_POS ] = PROT_MARK;
	xframe.buff[ DEV_POS ] = dev;
	xframe.buff[ CMD_POS ] = cmd;
	memcpy( xframe.buff + NDATA_POS, (uchar*)&ndata + ((NDATA_SIZE == 1) ? 1 : 0), NDATA_SIZE );
	memcpy( xframe.buff + DATA_POS, (uchar*)p, ndata );	
	xframe.buff[ ndata + DATA_POS ] = get_checksum( xframe.buff + DEV_POS, 
														ndata + 2 + NDATA_SIZE );
	xframe.qty = ndata + 4 + NDATA_SIZE;
	return &xframe;
}

/*
 * prepare_resp_val:
 *
 * 		Prepare a frame to send to validator 
 */

XMIT_BUFF_T *
prepare_resp_val( RFRAME_T *p )
{
	xframe.buff = rcv_frame.payload;	/* shared buffer */
	xframe.qty = p->curr_ndata;

	return &xframe;
}

/*
 * init_logprot:
 *
 * 		Initialize this module
 */

void
init_logprot( void )
{
	txmit0 = assign_timer( ONESHOT, NULL, TXMITCT0_IX ); 
}

/*
 * exec_logprot:
 *
 *		This function is called from main program or receiving UART ISR.
 *		Must be passed UART channel as parameter for selecting
 *		receiving queue.
 */

void
exec_protocol( MUInt ch )
{
	MUInt removing;
	
	in.who = ch == CTCH ? CT_PROT : VAL_PROT;
	in.news_fly = UART;

	removing = MAX_REMOVINGS;
	while( removing-- && get_char( ch, &in.news ) != -EMPTY_QUEUE )
		exec_mparser( &in );
}
