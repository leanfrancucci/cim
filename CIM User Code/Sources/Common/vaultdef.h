#ifndef __VAULTDEF_H__
#define __VAULTDEF_H__

#include "mytypes.h"

/*
 * 	vaultdef.h
 * 		General definitions
 * 		for vault safety system
 *
 *		Version 2.1
 *
 * 		Modification V 2.0 08/18/2007 11:51 ARG
 * 			Replaced some error codes
 * 		Modification V 2.1 09/03/2007 19:12 ARG
 * 			Replaced and added error codes
 * 			Number of file descriptors redefined
 *
 *
 * 		Eduardo Martinez
 *
 *
 *
 * 		Delsat Group S.A.
 * 		07/07/2007
 */

/*
 * 	Defines how many pages to use
 * 	for users (users.h & users.c)
 *	Rest of pages are used for simple
 *	file system (filedf.h & filedf.c)
 */

#define NUM_FLASH_PAGES_FOR_USERS	108		/*	This is a tentative number, must be clearly defined	*/


/*
 * 	General errors for all users calls
 * 	Value returned form functions other than 'users_init'
 * 	Take into account fatal addressing flash errors
 * 	They are negative values
 */

enum
{
	USER_OK, DFILE_OK = USER_OK,
	ERR_USER_NOT_EXISTS,			/*	user_id doesn't exists				*/
	ERR_USER_BAD_PASSWORD,			/*	Password presented does not match	*/
	ERR_USER_NUM_EXCEEDED,			/*	Can't add a new user				*/
	ERR_USER_EXISTS,				/*	user_id	exists						*/
	ERR_FREE1,						/*	Error not exists			 		*/
	ERR_USER_COMM_NOT_IN_EMERG,		/*	Command not allowed in emergency	*/
	ERR_FREE2,						/*	Error not exists					*/
	ERR_FLASH_BAD_PROGRAM,			/*	Programming does not verify			*/
	ERR_INVALID_CONTENT,

	ERR_DFILE_INVALID_DF,			/*	Invalid file descriptor				*/
	ERR_DFILE_BAD_ACCESS,			/*	Bad access in create				*/
	ERR_DFILE_NOT_ENOUGH,			/*	Not enough place for creation		*/
	ERR_DFILE_NOT_ALLOWED,			/*	Operation not allowed				*/
	ERR_DFILE_BAD_WHENCE,			/*	Bad whence data in seek				*/
	ERR_DFILE_BAD_SEEK,				/*	Seeks cannot be done				*/
	ERR_DFILE_BAD_FDESC,			/*	File descriptor out of range		*/
	ERR_DFILE_EXISTS_FDESC,			/*	File exists for the same descriptor	*/
	ERR_DFILE_NOT_EXISTS,			/*	File not exists						*/
	ERR_DFILE_BAD_ORIGIN,			/*	Invocation origin not exists		*/
	ERR_DFILE_SECTOR_ERROR,			/*	Sector error in reading				*/
	ERR_DFILE_BAD_OFFSET,			/*	Offset out of limits				*/
	ERR_DFILE_BAD_UNITSIZE,			/*	Record size is too long or zero		*/
	ERR_DFILE_BAD_NUMUNITS,			/*	Record number is zero				*/

	USER_FLASH_ERRORS,

	ERR_USER_BAD_DEV,				/*	Device not match with device list	*/	
	ERR_USRFORMAT_NOT_POSSIBLE,		/*	The plunger0 is not opened!			*/	

	ERR_UNKNOWN_ERROR = 128
		
};

/*
 * 	To address both devices
 */
enum
{
	MAIN_DEVICE, SECONDARY_DEVICE,
	NUM_FLASH_DEVICES
};

/*
 * Memories Errors
 */
enum
{ 
	BOTH_MEM_OK, SEC_MEM_FAIL, MAIN_MEM_FAIL, BOTH_MEM_FAIL 
};

/*
 * 	To address one of the two data flash buffers
 */

enum
{
	BUFFER0, BUFFER1
};


/*
 * 	Structure for addressing
 * 	data flash devices
 */

typedef struct
{
	uchar	*pbuff;			/*	Pointer to data to read or write		(IN-OUT)*/
	ushort	size;			/*	Size of data to read or write			(IN-OUT)*/
	ushort	base_page;		/*	Page origin to measure 'page'			(IN)	*/
	ushort	page;			/*	page to read or write					(IN)	*/
	ushort	address;		/*	address in page to read or write		(IN)	*/
							/*	also block address for 'block_erase'			*/
	short	offset;			/*	page offset to copy	(signed)			(IN)	*/
	uint	copy:1;			/*	Flag to copy to offset					(IN)	*/
	uint	both:1;			/*	Flag to copy to OTHER device			(IN)	*/
	uint	berase:1;		/*	Flag for Built in erase					(IN)	*/
	uint	device:1;		/*	Selects MAIN or SECONDARY device		(IN)	*/
	uint	wbuff:1;		/*	Selects BUFFER0 (0) or BUFFER1 (1)		(IN)	*/

} DFLASH_T;

/*
 * 	Offset to beginning of backup for users
 */

#define USER_START				0
#define OFFSET_BACKUP			(USER_START+NUM_FLASH_PAGES_FOR_USERS)
#define TOTAL_NUM_USERS_PAGES	(2*NUM_FLASH_PAGES_FOR_USERS)

/*
 * 	Offset of file system beginning  
 */

#define FILE_SYSTEM_START	(USER_START+TOTAL_NUM_USERS_PAGES)

/*
 * 	Maximum size of buffer for a file read
 */

#define LEO_BUFFER		250

#define WPAGE_SIZE	528

#define NUM_FW_PAGES	128

#endif

