/*
 * 	faccess.h
 * 		Permits access to file
 * 		details
 */


#include "mytypes.h"

/*
 * Returned by 'verify_and_recover_firmware_file'
 */

enum
{
	FW_OK,
	FW_NOTFREE, FW_NOFD,		/*	Not recoverable	*/
	FW_BLANK, FW_21, FW_12,		/*	Recoverable		*/
	FW_CANFILL,					/*	Not returned to this level	*/
	FW_NOTHING_TO_DO
};

/*
 * 	verify_and_recover_firmware_file
 *
 * 		Actions:
 * 		_________________________________________________________________
 * 		|	main	|	slave	|	Differ	|	Actions		|	Return	|
 * 		_________________________________________________________________
 * 		|	error	|	error	|	  X		|	blank_file	|	FW_BLANK|
 * 		|	error	|	ok		|	  X		|	M2->M1		|	FW_21	|
 * 		|	ok		|	error	|	  X		|	M1->M2		|	FW_12	|
 * 		|	ok		|	ok		|	 Yes	|	M1->M2		|	FW_12	|
 * 		|	ok		|	ok		|	  No	|	no action	|	FW_OK	|
 * 		_________________________________________________________________
 *
 * 		if at beginning of file system address is not a fd=15 file or
 * 		there is no room for that file, returns FW_NOTFREE
 * 		If construct_dir hasn't detected a FD_FW file, nothing is done
 * 		and returns FW_NOFD.
 * 		
 */


MInt verify_and_recover_firmware_file( void );

/*
 * 	construct_dir:
 * 		Constructs directory access
 * 		using information stored
 * 		in each page of flash
 *		Thinked to call it in power up
 *		The directory is constructed on memory
 *		Receives:
 *			Pointers to bit patterns:
 *				*pfd_ex = fd_exists
 *				*pdf_err = fd_inerror
 *			NULL if you don't want data
 *			Returns pages in error
 */

uint construct_dir( ushort *pfd_ex, ushort *pfd_err );

/*
 * 	make_file_current:
 * 		Receives a fd and, if fd exists, set it as current
 * 		and returns TRUE; else returns FALSE
 */

MUInt make_file_current( MUInt fd );

/*
 * 	get_existing_space:
 * 		Receives 'unit_size' of object to be stored
 * 		Returns maximum 'unit_number' that can be covered
 * 		with the pages that are not assigned yet
 */

ulong get_existing_space( MUInt unit_size );

/*
 * 	get_current_file_status:
 * 		For a file known to exists and set as current
 * 		with a prior call to 'make_file_current'
 * 		returns as reference its status
 */

MInt get_current_file_status( FILESYS_T *psta );

/*
 * 	blank_whole_system:
 * 		Formats each available page to
 * 		not used and also erase memory directory
 * 		Data contents are not initizialized
 */

void blank_whole_system( void );

/*
 * 	blank_current_file:
 * 		For an existing file and current with
 * 		a prior call to 'make_file_current'
 * 		set, for each page, its data contents to 0
 * 		and sets only for first page both indices to 0
 */ 		

MInt blank_current_file( MUInt in_init );

/*
 * 	create_file:
 * 		For an fd that is known that doesn't exists
 * 		creates a new file.
 * 		First make entry in directory
 *		Then simply calls to blank_current_file
 */

long create_file( MUInt fd, MUInt unit_size, MUInt access, ulong num_units );

/*
 *	read_current_file:
 *		Reads a random or queue file
 *		Must be called after 'make_file_current'
 */

int read_current_file( uint num_units, uchar *pbuff );

/*
 *	write_current_file:
 *		Writes a random or queue file.
 *		Must be called after 'make_file_current'
 */

int write_current_file( uint num_units, uchar *pbuff );

/*
 * 	seek_current:
 * 		After a 'make_file_current' changes
 * 		position in a file opened as RANDOM or on a QUEUE
 * 		file reopened as RANDOM
 */

MInt seek_current( long offset, MUInt whence );

/*
 * 	reopen_queue:
 * 		After a 'make_file_current' tries
 * 		to reopen as RANDOM.
 * 		Conditions: must be a QUEUE file and
 * 		not yet reopened
 */

MInt reopen_queue( void );

/*
 * 	reclose_queue:
 * 		After a 'make_file_current' tries
 * 		to close a QUEUE file reopened as RANDOM.
 * 		It may fail if file is RANDOM or in a QUEUE
 * 		case, was not previously reopened
 */

MInt reclose_queue( void );

/*
 * 	get_current_unit_size:
 * 		gets object size of current file
 */

uint get_current_unit_size( void );

/*
 * 	flush_page_in_buffer
 * 		If there is a page in buffer and it's dirty
 * 		then writes it in flash. Clears dirty
 * 		If argument is true, page_in_fuffer is voided
 */

MInt flush_page_in_buffer( MUInt void_page );

