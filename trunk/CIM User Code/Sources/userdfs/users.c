/*
 * 		users.c
 * 			User management
 * 			for FireKing project
 * 			Version 1.0 - 06/22/2007 16:54 ARG
 * 			Version 1.1 - 06/07/2007 11:18 ARG
 * 			Version 2.0 - 08/11/2007 08:54 ARG
 * 				Major change: changes devices usage
 * 					Only an entry per user is allowed
 * 			Version 2.1 - 08/17/2007 13:25 ARG
 * 				Changed according to L.Francucci email
 * 			Version 2.2 - 09/11/2007 12:07 ARG
 * 				Changed user_id to byte string
 *
 * 				Version 2.2
 *
 * 			Delsat Group S.A.
 * 			Programmer: E.A.Martinez
 *
 * 			Proposal submitted to approval
 *
 */

#include <string.h>

#include "allinc.h"

#include "users.h"

#include "dfmanage.h"

/*
 * 	General considerations:
 *
 * 		Each flash sector can store a number of 
 * 		USER_T that are determined by the flash density
 * 		and the number of NUM_FLASH_PAGES_FOR_USERS.
 *
 * 		Therefore, the number of users per page and the total
 * 		number of users are automatically determined by the flash
 * 		page size.
 *
 * 		Each page has a checksum word that protects page data. 
 * 		The checksum word is stored	in the page but out of the 2^n boundary of each page
 *
 * 		Each user page (main page) is duplicated in the same flash in
 * 		another page (the backup page).
 *
 * 		The whole main flash also is backed-up in another flash
 *
 * 		At power up, each flash is read in order and in case
 * 		of bad flash pages, the page is recovered from the good
 * 		copy in the same flash and stored in the main page.
 *
 * 		The recover algorithm uses the following precedence:
 * 			Main memory - main page
 * 			Main memory - backup page
 * 			Backup memory - main page
 * 			Backup memory - backup page
 */


/*
 * 	Used for 'user_exists'
 */

enum
{
	NO_SEARCH, SEARCH
};

/*
 * 	To mark each user record
 */

enum
{
	UAVAIL, UUSED
};

/*
 * 	Used for verify_integrity functions
 */

enum
{
	MUST_FORMAT, VERIFY_OK
};


#define NO_ROOM			0xffff
#define USER_COPY		4

typedef struct
{
	uchar	used;
	uchar	signature;
	USER_T	user;
} FUSER_T;

/*
 *	Static variables
 *	uninitialized
 */

static FUSER_T	user_buffer;
static DFLASH_T df;
static ushort check_found;
static MInt pass_index;
static MUInt pass[USER_COPY];
static uint recovered_count, ok_count, format_count;
#ifdef USER_ACCEL
static uchar page_buffer[ 1024 ];
#endif


/*
 * ----- Static functions
 */

/*
 * ===== Functions for flash verification
 */


/*
 *	set_results:
 *		For eeach (i) (case test)
 *		sets if page passes and, in this case
 *		remembers check if no previous check remembered
 */

static
void
set_results( MUInt i )
{
	ushort check;

	if( df_read_page( &df ) != USER_OK )
	{
		pass[i] = 0;
		return;
	}
	check = *(ushort *)df.pbuff;
	if( pass_index < 0 )
	{
		pass_index = i;
		check_found = check;
		pass[i] = 1;
	} else
		pass[i] = ( check_found == check );
}

/*
 * 	check_results for page
 * 		if no main and backup pages passed, then
 * 		formats
 * 		else if all pages ok, simply retyrns
 * 		else copy first good page in all others
 */

static
MInt
check_results( void )
{
	MUInt i;
	MInt status;

	df.wbuff = BUFFER0;
	df.base_page = 0;
	df.copy = 1;
	df.both = 1;
	df.berase = 1;
	if( pass_index < 0 )
	{
		df.device = MAIN_DEVICE;
		df.offset = OFFSET_BACKUP;
		++format_count;
		return df_format_page( &df );
	}
	for( i = emerg * 2 ; pass[i] ; ++i )
		if( i >= USER_COPY - 1 )
		{
			++ok_count;
			return USER_OK;
		}
#if 0
	switch( pass_index )
	{
		case 0:
			df.device = MAIN_DEVICE;
			df.offset = OFFSET_BACKUP; 
			df.base_page = 0;
			break;
		case 1:
			df.device = MAIN_DEVICE;
			df.offset = -OFFSET_BACKUP; 
			df.base_page = OFFSET_BACKUP;
			break;
		case 2:
			df.device = SECONDARY_DEVICE;
			df.offset = OFFSET_BACKUP; 
			df.base_page = 0;
			break;
		case 4:
			df.device = SECONDARY_DEVICE;
			df.offset = -OFFSET_BACKUP; 
			df.base_page = OFFSET_BACKUP;
			break;
	}
	++recovered_count;
	return df_copy_page( &df, emerg );
#else
	df.base_page = 0;
	df.wbuff 	= BUFFER0;
	switch( pass_index )
	{
		case 0:
			df.device	= MAIN_DEVICE;
			df.offset	= 0;
			break;
		case 1:
			df.device	= MAIN_DEVICE;
			df.offset	= OFFSET_BACKUP;
			break;
		case 2:
			df.device	= SECONDARY_DEVICE;
			df.offset	= 0;
			break;
		case 3:
			df.device	= SECONDARY_DEVICE;
			df.offset	= OFFSET_BACKUP;
			break;
	}
	++recovered_count;
	df_read_page( &df );
	df.device = MAIN_DEVICE;
	df.offset = 0;
	if( (status = program_page( &df ) ) != DFILE_OK )
		return status;	
	df.offset = OFFSET_BACKUP;
	return df_copy_page( &df, emerg );
#endif
}

/*
 * 	verify_and_recover:
 * 		For each users page verifies and
 * 		eventually recovers or format page
 */

static
MUInt
verify_and_recover( void )
{
	format_count = ok_count = recovered_count = 0; 
	df.wbuff = BUFFER0;
	for( df.page = 0 ; df.page < NUM_FLASH_PAGES_FOR_USERS ; ++df.page )
	{
		pass_index = -1;
	
		if( !emerg )
		{	
			df.device = MAIN_DEVICE;
			df.base_page = 0;
			set_results( 0 );
		
			df.base_page = OFFSET_BACKUP;
			set_results( 1 );
		}
		
		if( ( mem_status & SEC_MEM_FAIL ) != SEC_MEM_FAIL )
		{
			df.device = SECONDARY_DEVICE;
			df.base_page = 0;
			set_results( 2 );
		
			df.base_page = OFFSET_BACKUP;
			set_results( 3 );
		}

		check_results();
	}
	if( recovered_count != 0 || ( ok_count != 0 && format_count != 0 ) )
		return RECOVERED;
	if( format_count != 0 )
		return COLD_FORMATTED;
	return INIT_OK;
}

static
MUInt
calculate_signature( uchar *puid )
{
	MUInt signature, i;

	for( signature = 0, i = 0 ; i < NUM_UID ; ++i )
		signature += *puid++;
	return signature & 0xff;
}


/*
 *	======= End of verification and recover
 */

/*
 * ----- Auxiliary functions for users
 */


/*
 * 	pass_compare:
 * 		Compares both passes (*p1 against *p2)
 * 		for NUM_PASS bytes
 * 		Returns true or false
 */

static
MUInt
pass_compare( const uchar *p1, const uchar *p2 )
{
	MUInt i;

	for( i = 0 ; i < NUM_PASS ; ++i )
		if( *p1++ != *p2++ )
	   		return 0;
 	return 1;
}	

static
MInt
change_user_page( uint page, uint address )
{
	df.device		= 	MAIN_DEVICE;
	df.wbuff		=	BUFFER0;
	df.pbuff		=	(uchar *)&user_buffer;
	df.size			=	sizeof( FUSER_T );
	df.base_page	=	0;
	df.page 		=	(ushort)page;
	df.address		=	(ushort)address;
	df.offset		=	OFFSET_BACKUP;
	df.copy			=	1;
	df.both			=	1;
	df.berase		=	1;

	return modify_page( &df );
}


/*
 * 	user_insert:
 * 		Inserts a new user in 'device' from '*p' in 'page'
 * 		at 'address'
 */

static
MInt
user_insert( const USER_T *p, uint page, uint address )
{
	user_buffer.used = UUSED;
	user_buffer.user = *p;
	user_buffer.signature = calculate_signature( p->user_id );
	return change_user_page( page, address );
}

/*
 * 	user_delete:
 * 		Deletes an existing user in both devices
 * 		in 'page' at 'address'
 */

static
MInt
user_delete( uint page, uint address )
{
	user_buffer.used = UAVAIL;
	return change_user_page( page, address );
}

/*
 * 	change_password:
 * 		Changes pass0, pass1 or both (depending on 'edit_pass') at both
 * 		devices at 'address' of 'page' with contents extracted from '*p'
 */

static
MInt
change_password( MUInt edit_pass, const USER_T *pnew, uint page, uint address )
{
	if( edit_pass == EDIT_PASS0 || edit_pass == BOTH_EDIT )
		memcpy( user_buffer.user.pass0, pnew->pass0, NUM_PASS );
	if( edit_pass == EDIT_PASS1 || edit_pass == BOTH_EDIT )
		memcpy( user_buffer.user.pass1, pnew->pass1, NUM_PASS );
	return change_user_page( page, address );
}

static
MInt
set_current_user_devlist( uint dev_list, uint page, uint address )
{
	user_buffer.user.dev_list = (ushort)dev_list;
	return change_user_page( page, address );
}

/*
 * 	---	Here functions which only read
 * 	---	They read from MAIN_DEVICE and main 'page'
 */

/*
 * 	verify_password:
 * 		Compares pass0 of 'pold' with both passwords
 * 		at 'address' in 'page'
 * 		returns PASS_BAD, PASS0_MATCH or PASS1_MATCH
 */

static
MUInt
verify_password( const USER_T *pold, uint page, uint address )
{
	df.device		= (uchar)emerg;
	df.wbuff		= BUFFER0;
	df.base_page 	= 0;
	df.page			= (ushort)page;

	df_read_page( &df );

	df.pbuff		= (uchar *)&user_buffer;
	df.size			= sizeof( FUSER_T );
	df.address		= (ushort)address;

	df_read_data( &df );

	if( pass_compare( user_buffer.user.pass0, pold->pass0 ) )
		return PASS0_MATCH;
	if( pass_compare( user_buffer.user.pass1, pold->pass0 ) )
		return PASS1_MATCH;
	return PASS_BAD;
}

#if 0

static
MUInt
user_compare( uchar *p )
{
	uchar *q;

	for( q = user_buffer.user.user_id ; q < user_buffer.user.user_id + NUM_UID ; q )	
		if( *p++ != *q++ )
			return 0;
	return 1;
}

#endif

/*
 * 	user_exists:
 * 		Search user for 'user_id' and returns
 * 		true if exists or false if not found
 * 		In order not to read twice, sets *ppage and *paddress to the
 * 		value of page and address where:
 * 			a.- First free space is found in case 'in_search' == false
 * 				If no free space is found, 	*paddress has NO_ROOM.
 * 			b.- page and address where user was found. In case not found
 * 				page and address are meaningless
 * 		in case of search, returns in '*pdev_list' the 'dev_list' of user
 * 			if '*pdev_list' is not NULL
 *
 * 		Changed from version 1.x to version 2.x
 * 		user_id is the only key
 */

static
MUInt
user_exists( const uchar *puser_id, ushort *pdev_list, uint *ppage, uint *paddress, MUInt in_search )		/*	Changed prototype	*/
{
	MUInt free_found;
	MUInt found;
	MUInt signature;
	FUSER_T *p;
#ifdef USER_ACCEL
	ushort address;
#endif


	df.device		= (uchar)emerg;
	df.wbuff		= BUFFER0;
	df.base_page	= 0;

	signature = calculate_signature( puser_id );
	found = 0;
	*paddress = NO_ROOM;
	for( df.page = 0, free_found = 0 ; df.page < NUM_FLASH_PAGES_FOR_USERS ; ++df.page ) 
	{
		df_read_page( &df );
#ifndef USER_ACCEL
		df.pbuff 		= (uchar *)( p = &user_buffer );
		df.size			= sizeof( FUSER_T );
		for( df.address = 0 ; df.address < num_users_per_page * sizeof( FUSER_T ) ; df.address += sizeof( FUSER_T ) )
		{
			df_read_data( &df );
#else
		df.pbuff 		= page_buffer;
		df.size			= page_eff_size;
		df.address		= 0;
		df_read_data( &df );
		for( address = 0, p = (USER_T *)page_buffer ; address < num_users_per_page * sizeof( FUSER_T ) ; address += sizeof( FUSER_T ), ++p )
		{
#endif
			
			if( p->used == UUSED )
			{
//				if( user_buffer.user.user_id == user_id )		/*	Changed from V1.x to V2.x	*/
//				if( user_compare( puser_id ) )
				if( p->signature == signature && memcmp( p->user.user_id, puser_id, NUM_UID ) == 0 )
				{

					if( in_search )
					{
						*ppage = df.page;
#ifndef USER_ACCEL
						*paddress = df.address;
						user_buffer = *p;
#else
						*paddress = address;
#endif
						if( pdev_list != NULL )
							*pdev_list = p->user.dev_list;
						return 1;
					}
					else
						found = 1;
				}
			} else
			{
				if( !in_search && !free_found  )
				{
					free_found = 1;
					*ppage = df.page;
#ifndef USER_ACCEL
						*paddress = df.address;
#else
						*paddress = address;
#endif
					if( found )
						return found;
				}
			}
		}
	}
	return found;
}

/*
 * ---	Here functions to format
 */


/*
 * 	format_page:
 * 		Formats a 'page' (main and backup)
 * 		on both devices
 */

static
MInt
format_page( uint page )
{
	df.device	= MAIN_DEVICE;
	df.wbuff 	= BUFFER0;
	df.page 	= (ushort)page;
	df.offset	= OFFSET_BACKUP;
	df.copy		= 1;
	df.both 	= 1;
	df.berase	= 1;

	return df_format_page( &df );
}


/*
 * 	get_free_users:
 * 		Sweeps all user memory to find
 * 		how many users slots are free
 */

static
uint
get_free_users( void )
{
	uint free_count;

	df.device		= (uchar)emerg;
	df.wbuff		= BUFFER0;
	df.base_page	= 0;

	for( df.page = 0, free_count = 0 ; df.page < NUM_FLASH_PAGES_FOR_USERS ; ++df.page ) 
	{
		df_read_page( &df );
		df.pbuff = (uchar *)&user_buffer;
		df.size	= sizeof( FUSER_T );

		for( df.address = 0 ; df.address < num_users_per_page * sizeof( FUSER_T ) ; df.address += sizeof( FUSER_T ) )
		{
			df_read_data( &df );
			if( user_buffer.used == UAVAIL )
				++free_count;
		}
	}
	return free_count;
}

/*
 * ------ Public functions
 */

/*
 * 	users_init:
 * 		Used on power up to initialize
 * 		users structures
 * 		
 * 		Repair or initializes both
 * 		flashes
 *
 * 		Return if flashes initted from cold,
 * 		repaired, or OK.
 *
 * 		Return by reference 'num_users_avail'
 * 		resulting from NUM_FLASH_PAGES_FOR_USERS,
 * 		and installed flash page size
 *
 * 		No change from Version 1.x to Version 2.x
 *
 * 		From version 3.x, users_init must be called after
 * 		calling 'gflash_init'; also changed arguments
 *
 */
	

MInt
users_init( uint *pnum_users_avail )
{
	num_users_per_page = page_eff_size/sizeof( FUSER_T );
	if( pnum_users_avail != NULL )
		*pnum_users_avail = num_users_per_page * NUM_FLASH_PAGES_FOR_USERS;
	return verify_and_recover();
}

/*
 * 	users_format:
 * 		Formats unconditionally the users 
 * 		flash pages.
 * 		If users_format is used, there is no need
 * 		to call users_init
 * 		Not allowed from secondary processor (emerg == 1 )
 * 		Must be called after 'users_init'
 *
 * 		No change from Version 1.x to Version 2.x
 */

MInt
users_format( void )
{
	uint page;
	MInt status;

	if( emerg )
		return -ERR_USER_COMM_NOT_IN_EMERG;
	for( page = 0 ; page < NUM_FLASH_PAGES_FOR_USERS ; ++page )
		if( ( status = format_page( page ) ) != USER_OK )
			return status;
	return USER_OK;
}


/*
 * 	users_add:
 * 		For adding user from data in 'p' 
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case.
 *
 * 		Changed from V 1.x yo version 2.x
 * 		Only 'user_id' is used as key
 */

MInt
users_add( const USER_T *p )
{
	uint page;
	uint address;

	if( emerg )
		return -ERR_USER_COMM_NOT_IN_EMERG;
	if( user_exists( p->user_id, NULL, &page, &address, NO_SEARCH ) )
		return -ERR_USER_EXISTS;
	if( address == NO_ROOM )
		return -ERR_USER_NUM_EXCEEDED;
	return user_insert( p, page, address );
}


/*
 * 	users_erase:
 * 		Eliminates 'user_id' if exists
 * 		*puser_id is truncated to NUM_UID bytes.
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 * 		Changed argument for user_id from version 2.2 and up
 */

MInt users_erase(  uchar *puser_id )
{
	uint page;
	uint address;

	if( emerg )
		return -ERR_USER_COMM_NOT_IN_EMERG;
	if( user_exists( puser_id, NULL, &page, &address, SEARCH ) )
		return user_delete( page, address );
	return -ERR_USER_NOT_EXISTS;
}

/*
 * 	users_edit:
 * 		Edits user information stored under 'user_id' of '*pold'.
 * 		Presents for logging one of the passwords in 'pass0'.
 * 		(Password in pass1 is don't care).
 * 		Only passwords may be edited; 'edit_pass' says which one or both
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 *
 * 		Changed from V 1.x yo version 2.x
 * 		Only 'user_id' is used as key
 */



MInt
users_edit( MUInt edit_pass, USER_T *pold, USER_T *pnew, MUInt log )
{
	uint page;
	uint address;

	if( emerg )
		return -ERR_USER_COMM_NOT_IN_EMERG;
	if( !user_exists( pold->user_id, NULL, &page, &address, SEARCH ) )		/*	Changed from V1.x to V2.x	*/
		return -ERR_USER_NOT_EXISTS;
	if( log == LOG_EDIT && verify_password( pold, page, address ) == PASS_BAD )
		return -ERR_USER_BAD_PASSWORD;
	return change_password( edit_pass, pnew, page, address );
}

/*
 * 	users_validation:
 * 		Validates one user
 * 			In case of two users validation
 * 			this function must be called twice
 * 			as for example in case both must be ok:
 *				user_validation( &user0 ) == USER_OK && user_validation( &user1 ) == USER_OK;
 *			or in case one user must be validated
 *				user_validation( &user0 ) == USER_OK || user_validation( &user1 ) == USER_OK;
 *			Significance of arguments and return values same as 'users_pass_edit'
 *			Returns:
 *				USERS_OK if pass0 of '*p' matches with any one of the two passwords
 *				for user_id in '*p' or negative in error case.
 *				In success case:
 *					Returns in '*pass_match' which of both passwords matched.
 *					In '*pdev_list' returns the actual dev_list
 *
 * 		Changed from V 1.x yo version 2.x
 * 		Only 'user_id' is used as key
 */

MInt
users_validation( const USER_T *p, uchar *pass_match, ushort *pdev_list )
{
	uint page;
	uint address;
	ushort dev_list;

	if( !user_exists( p->user_id, &dev_list, &page, &address, SEARCH ) )
		return -ERR_USER_NOT_EXISTS;
	if( ( *pass_match = (uchar)verify_password( p, page, address ) ) == PASS_BAD )
		return -ERR_USER_BAD_PASSWORD;
	if( pdev_list != NULL )
		*pdev_list = dev_list;
	return USER_OK;
}

/*
 * 	users_get_devlist:
 * 		This call returns '*p_dev_list' related with 'user_id'
 * 		in case 'user_id' exists
 * 		*puser_id is truncated to NUM_UID bytes.
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 * 		Changed argument for user_id from version 2.2
 */

MInt
users_get_devlist( uchar *puser_id, ushort *pdev_list )
{
	uint page;
	uint address;
	ushort dev_list;

	if( !user_exists( puser_id, &dev_list, &page, &address, SEARCH ) )
		return -ERR_USER_NOT_EXISTS;
	if( pdev_list != NULL )
		*pdev_list = dev_list;
	return USER_OK;
}


/*
 * 	users_set_devlist:
 * 		This calls set 'dev_list' for 'user_id' in case
 * 		'user_id' exists.
 * 		*puser_id is truncated to NUM_UID bytes.
 * 		'dev_list' is truncated to 16 bits.
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 * 		Changed argument for user_id from version 2.2
 */

MInt
users_set_devlist( uchar *puser_id, uint dev_list )
{
	uint page;
	uint address;

	if( !user_exists( puser_id, NULL, &page, &address, SEARCH ) )
		return -ERR_USER_NOT_EXISTS;
	return set_current_user_devlist( dev_list, page, address );
}


/*
 * 	get_user_information:
 * 		Returns information about maximum user number
 * 		and free space remaining for users only
 * 		if corresponding pointer is not NULL
 */

MInt
get_user_information(  uint *pmax_users, uint *pfree_users )
{
	if( pfree_users != NULL )
		*pfree_users = get_free_users();
	if( pmax_users != NULL )
		*pmax_users = num_users_per_page * NUM_FLASH_PAGES_FOR_USERS;
	return USER_OK;
}

/*
 * 	get_processor_state:
 * 		Returns which processor is active
 * 		giving MAIN_DEVICE or SECONDARY_DEVICE
 * 		This call must be made after 'users_init'
 */

MUInt
get_processor_state( void )
{
	return emerg;
}

