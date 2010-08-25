/*
 * 		users.h
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

#include "mytypes.h"

#define	NUM_PASS	8

/*
 * 	From version 2.2 and up
 * 	user_id changed to array of 16 bytes
 * 	IMPORTANT: all bytes in 0xFF MUST BE NOT USED
 * 		for a valid user !!!
 */

#define NUM_UID		16

/*
 * 	Returned by 'users_init'
 */

enum
{
	INIT_OK,				/*	No errors in both flashes			*/
	COLD_FORMATTED,			/*	Typical in first use				*/
	RECOVERED,				/*	Errors recovered					*/

	NUM_USERS_INIT
};

/*
 *		Define passwords to edit
 *			Used in 'users_edit'
 */

enum
{
	EDIT_PASS0, EDIT_PASS1, BOTH_EDIT
};	

/*
 *		Password matched
 *			Used in users_validation
 */

enum
{
	PASS0_MATCH, PASS1_MATCH, PASS_BAD 
};

/*
 *		Define type of user edit.	
 *			Used in users_edit
 */

enum
{
	FORCE_EDIT, LOG_EDIT 
};

/*
 * 	Structure for dealing with users data
 * 	
 * 		dev_list has a 16 bit word for device bit pattern
 * 			dev_list is used only for higher level software and
 * 			is not interpreted by this module.
 *		user_id has the user id as an unsigned number
 *		pass0 and pass1 are the two passwords of a user
 */


typedef struct
{
	uchar	user_id[NUM_UID];		/*	user_id is main search key							*/
	ushort	dev_list;				/*	dev_list only for high level software				*/
	uchar	pass0[ NUM_PASS ];		/*	In case validation, here goes the login password	*/
	uchar	pass1[ NUM_PASS ];
} USER_T;

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
 *
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
 * 		repaired or OK.
 *
 * 		In case of addressing by 'emergency' channel
 * 		only SECONDARY flash is verified
 *
 * 		Return by reference 'num_users_avail'
 * 		resulting from NUM_FLASH_PAGES_FOR_USERS,
 * 		and installed flash page size
 *
 * 		From version 3.x(in preparation), users_init must be called after
 * 		calling 'dfmanage_init, also changed arguments '
 */
	
MInt users_init( uint *pnum_users_avail );

/*
 * 	users_format:
 * 		Formats unconditionally the users 
 * 		flash pages.
 * 		If 'emerg' != 0, then only
 * 		there are transactions with SECONDARY_DEVICE
 * 		and also only this device is formatted
 * 		Must be called after 'users_init'
 */

MInt users_format( void );

/*
 * 	users_add:
 * 		For adding user from data in 'p' 
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 */

MInt users_add( const USER_T *p );

/*
 * 	users_erase:
 * 		Eliminates 'user_id' if exists
 * 		*puser_id is truncated to NUM_UID bytes.
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 * 		Changed argument for user_id from version 2.2 and up
 */

MInt users_erase(  uchar *puser_id );

/*
 * 	users_edit:
 * 		Edits user information stored under 'user_id' of '*pold'.
 * 		Presents for logging one of the passwords in 'pass0'.
 * 		(Password in pass1 is don't care).
 * 		Only passwords may be edited; 'edit_pass' says which one or both.
 * 		To edit without logging the parameter 'log' must be set in FORCE_EDIT.
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 *
 */

MInt users_edit( MUInt edit_pass, USER_T *pold, USER_T *pnew, MUInt log );

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
 */

MInt users_validation( const USER_T *p, uchar *pass_match, ushort *pdev_list );

/*
 * 	users_get_devlist:
 * 		This call returns '*p_dev_list' related with 'user_id'
 * 		in case 'user_id' exists
 * 		*puser_id is truncated to NUM_UID bytes.
 * 		Returns USERS_OK in case of succcess or a negative number
 * 		in error case
 * 		Changed argument for user_id from version 2.2
 */

MInt users_get_devlist( uchar *puser_id, ushort *pdev_list );

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

MInt users_set_devlist( uchar *puser_id, uint dev_list );

/*
 * 	get_user_information:
 * 		Returns information about maximum user number
 * 		and free space remaining for users only
 * 		if corresponding pointer is not NULL
 */

MInt get_user_information(  uint *pmax_users, uint *pfree_users );

/*
 * 	get_processor_state:
 * 		Returns which processor is active
 * 		giving MAIN_DEVICE or SECONDARY_DEVICE
 * 		This call must be made after 'users_init'
 */

MUInt get_processor_state( void );

