/*
 * cscmds.h
 */


/*
 * 		It indicates the desired command to execute on received device.
 * 		Here is described the possible direction of frame that 
 * 		support it.
 */

typedef enum
{
	/* Direction: CT <-> SAFEBOX */
	
	NO_CMD,				/* 	Frame to validator devices 			*/

	/* Direction: CT -> SAFEBOX */
	
	UNLOCK,				/* 	Unlock locker devices 				*/
	LOCK, 				/* 	Lock locker devices 				*/
	ACTRL,				/*	Active/deactive alarm devices		*/
	ADDUSR,				/*	Add one user 						*/
	DELUSR,				/*	Delete one user 					*/
	EDITUSR,			/*	Edit user passwords					*/
	GETUSRDEV,			/*	Get user device list				*/
	SETUSRDEV,			/*	Set user device list				*/
	VALUSR,				/*	Validate one user					*/
	GETUSRINFO,			/*	Get user info						*/
	USRFORMAT,			/*	Format user memory					*/
	VERSION,			/*	Return version string				*/
	MODEL,				/*	Not assigned 						*/
	SHUTDOWN,			/*	Force a system shutdown				*/
	GR1_STATUS,			/*	Return status of several devices	*/
	BLANKDFS,
	CREATEFILE,
	STATUSFILE,
	READFILE,
	WRITEFILE,
	SEEKFILE,
	REOPENFILE,			/*	Not assigned 						*/
	CLOSEFILE,			/*	Not assigned						*/
	REINITFILE,
	GETDFSINFO,
	TLOCK,				/*	Set a lock time for lockers			*/
	TUNLOCKENABLE,		/*	Set a unlock enable time for keyswitch */
	
	/* Direction: SAFEBOX -> CT */

	SBOX_FAILURE,
	SBOX_SUCCESS,
	SBOX_NACK,

	/* Here is defined the new commands with direction: CT -> SAFEBOX */

	VALCONFIG,
	FORCEUSRPASS,
	SYNCNUMFRAMES,
	HOSTPWR,
	RESETMCU,
	TUNLOCKENABLE_LOCKS,
	VALPWR,

	NUM_CMDS
} CMD_T;

/*
 * 		Value not allowed for lock timer. 
 * 		Used by tlock command.
 */

#define TLOCK_NOT_ALLOWED			0xFF

/*
 * 		Value not allowed for unlock enable timer. 
 * 		Used by tunlock_enable command.
 */

#define TUNLOCK_NOT_ALLOWED			0xFF

/*
 * 		It define facilty of unlock command.
 */

enum
{
	SINGLE, DUAL
};

/*
 * 		Available memory devices. Used by any memory commands.
 */

enum
{
	GPMEM, USERSMEM
};
