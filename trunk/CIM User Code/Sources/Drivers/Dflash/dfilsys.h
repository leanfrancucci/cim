/*
 * 		dfilsys.h
 * 			communication view
 *
 * 		Data Flash fILE SYStem
 * 		New file system for vault project
 *
 * 		Delsat Group S.A.
 * 		First edition: 7/24/2007 14:37 ARG Version 0.1
 * 		Modification: 08/07/2007 16:00 Version 0.2
 * 		Modification: 08/10/2007 12:28 Version 0.3
 * 		Modification: 08/17/2007 12:54 Version 0.4
 * 		Modification: 09/03/2007 19:15 Version 0.5
 * 		Modification: 09/07/2007 15:39 Version 0.6
 * 			Se agrega dfilesys_get_information
 * 		Modification: 09/17/2007 13:28 Version 0.7
 * 			Changed QUEUE files as per Oliva requirement
 * 			QUEUE files in write overwrites older registers
 * 						They are read as RANDOM but moving in the
 * 						logical QUEUE space with dfilesys_seek
 * 						Position on power up is pointing to older
 * 						register, i.e. to queue beginning
 *
 * 		Version 0.7	- Proposal accepted by Soledad Oliva
 *
 * 		Eduardo Martinez
 */

#include "mytypes.h"

/*
 * 	dfilesys_blanking:
 * 		The entire filesystem is destroyed.
 * 		All files are removed
 * 		No input or output
 */

void dfilesys_blanking( void );

/*
 * 	dfilesys_create_file:
 * 		Creates a new file in file system
 * 		Inputs:
 * 			'df_desc' = file descriptor number to be created
 * 			'unit_size' (probably an structure size)
 * 			'*pnum_units' is the number of units that the file must hold.
 * 			In case '*pnum_units' is negative, then all the space remaining
 * 				is allocated for the file and the number of units allocated is returned by
 * 				reference in '*pnum_units"
 * 			'access' is the type of access that the file sustain for reading and writing.
 * 		Conditions
 * 			File contents are initialized to zero (C style)
 * 				Random files: position is reset to 0
 * 				Queue files: input and output index as well as
 * 					queue count are zeroed.
 * 		Returns:
 * 			From function: DFILE_OK or negative on error
 * 			By reference in '*pnum_units' the actual num_units allocated in case
 * 			of no error; *pnum_units as output may be greater as *pnum_units as input:
 * 			this comes from the fact that an integer number of pages is associated to a file.
 * 		Important note: if file system remaining space is less that on '*pnum_units' then
 * 			file DFILE_NOT_ENOUGH error is returned and in '*pnum_units' number of data units
 * 			that could be allocated is returned. File is not created.
 * 		Errors:
 * 			-ERR_DFILE_BAD_ACCESS:		Bad 'access' code
 * 			-ERR_DFILE_NOT_ENOUGH:		See important note
 * 			-ERR_DFILE_BAD_FDESC:		File descriptor not allowed
 * 			-ERR_DFILE_EXISTS_FDESC:	Exists file created with same file descriptor
 */

MInt dfilesys_create_file( MUInt df_desc, MUInt unit_size, MUInt access, long *pnum_units );

/*
 * 	dfilesys_status_file:
 * 		Return status of file.
 * 		Inputs:
 * 			File decriptor 'df_desc'
 * 		Returns:
 * 			Status returned in structure pointed by 'psta'
 * 			Function returns DFILE_OK on success or negative in case of error
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:	Descriptor out of range
 * 			-ERR_DFILE_NOT_EXISTS:	File not created
 */

MInt dfilesys_status_file( MUInt df_desc, FILESYS_T *psta );

/*
 * 	dfilesys_get_information:
 * 		Returns the bit field of file descriptors used,
 * 		bit field of file descriptors in error, file system 
 * 		pages in error and file system size.
 * 		Note: Must be called after 'dfilesys_init' function.
 *
 * 		If NULL, error.
 */

DFS_INFO_T *dfilesys_get_information( void );

/*
 * 	dfilesys_read_file:
 * 		Reads from a file according to its access mode in creation.
 * 		Conditions
 * 			Random file, reads from its current position.
 * 			Queue file, reads from its logical position. (More about that in
 * 				dfilsys_seek).
 * 		Inputs
 * 			'df_desc' is the file descriptor
 * 			'num_units' carry the units quantity to be read
 * 		Outputs:
 * 			Function returns number of units actually read if >= 0 or negative
 * 				in error case
 * 			If no error, 'pbuff' contains data.
 * 			In case of -DFILE_SECTOR_ERROR, 'pbuff' contains data that
 * 				was read but without guarantee of corrctness
 *
 * 		If value returned (being positive) is less that 'num_units'
 * 		(including 0) represents that:
 * 			Random file: end of file has been reached.
 * 			Queue file: trying to read from a logical position
 * 				that is beyond the input pointer or the queue has
 * 				lesser units than requiered or the queue is empty 
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:		Descriptor out of range
 * 			-ERR_DFILE_BAD_NUMUNITS:	num_units is 0
 * 			-ERR_DFILE_NOT_EXISTS:		File not created
 * 			-ERR_DFILE_SECTOR_ERROR:	There was a sector error
 */

int dfilesys_read_file( MUInt df_desc, uint num_units, uchar *pbuff );

/*
 * 	dfilesys_write_file:
 * 		Writes in a file according to its access mode in creation.
 * 		Doesn't allows to write randomly in a QUEUE file reopened as random
 * 		Inputs:
 * 			'num_units' quantity to be written is passed
 * 			'pbuff' contains data to be written.
 *		Conditions:
 * 			Random file, writes into its current position.
 * 			Queue file, writes into input index. If queue is full
 * 				overwrites older registers
 * 		Function returns number of units actually written if >= 0 or negative
 * 			in error case.
 *
 * 		If value returned (being positive) is less that 'num_units'
 * 		represents that:
 * 			Random file: end of file has been reached.
 * 			Queue file: only in case 'num_units' exceeds total queue size
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:		Descriptor out of range
 * 			-ERR_DFILE_BAD_NUMUNITS:	num_units is 0
 * 			-ERR_DFILE_NOT_EXISTS:		File not created
 * 			-ERR_DFILE_NOT_ALLOWED:		A write intent over a QUEUE file reopened as RANDOM
 */

int dfilesys_write_file( MUInt df_desc, uint num_units, uchar *pbuff );

/*
 *	 dfilesys_seek:
 *	 	Only used for files created as random or for files
 *	 	created for queue use in case reopened as random.
 *	 	Inputs:
 *	 		'df_desc' is file handler
 *	 		'offset' is desplacement from 'whence' point measure in terms
 *	 			of object units.
 *	 		'whence' specifies frow where 'offset' is measured
 *	 			Note: arguments are very similar to that of 'seek' Unix system call
 *	 	Important note: in case of QUEUE files, this seek only serves for reading
 *	 		and not for writing.
 *	 		QUEUE files are written as normal queues, i.e. using an input pointer
 *	 		regardless of the position reached by this call.
 *	 		In read case, the position is relative to the beginning of queue, i.e.
 *	 		the position is logical within the queue and not physical as is in
 *	 		the case of RANDOM files
 *	 		The seek in QUEUE files is valid for read. Each subsequent read advances
 *	 		position in num_items efectively read.
 *	 		In case a QUEUE file write is done, next position for read is reset to
 *	 		first position, i.e. to beginning of queue
 *	 	Outputs:
 *	 		Function returns DFILE_OK on succcess or negative in error case
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:		Descriptor out of range
 * 			-ERR_DFILE_NOT_EXISTS:		File not created
 * 			-ERR_DFILE_NOT_ALLOWED:		Seek on a QUEUE file not reopened as RANDOM
 * 			-ERR_DFILE_BAD_WHENCE:		whence code incorrect
 * 			-ERR_DFILE_BAD_SEEK:		Seek before beginning of file or past end of file
 *											Seek not done
 *
 */

MInt dfilesys_seek( MUInt df_desc, long offset, MUInt whence );

#if 0

/*
 * 	This functions has no more meaning in from version
 * 	0.7 and up
 */

/*
 * 	dfilesys_reopen_as_random:
 * 		Only to be called with a file created as QUEUE
 * 		Permits to consider a file created as QUEUE as a random
 * 		file in order to recover information contained in queue
 * 		This access mode is retained for this file till
 * 		'dfilesys_close_as_random' is executed.
 * 		Position in file is set to beginning of file
 * 		Supports in this mode seek and read as a normal
 * 		random file. Status is returned as both a queue and random
 * 		file.
 * 		Input:
 * 			'df_desc' file descriptor
 *		Output:
 * 			Function returns DFILE_OK on success or negative in error case
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:		Descriptor out of range
 * 			-ERR_DFILE_NOT_EXISTS:		File not created
 *			-ERR_DFILE_NOT_ALLOWED:		Trying to reopen a random file or a QUEUE file that
 *											was reopened before without closing
 */

MInt dfilesys_reopen_as_random( MUInt df_desc );

/*
 * 	dfilesys_close_as_random:
 * 		Only to be called with a file created as QUEUE and
 * 		after a call to 'dfilesys_reopen_as_random' has been
 * 		made.
 * 		Terminates consideration of a QUEUE file as RANDOM
 * 		Input:
 * 			'df_desc' file descriptor
 *		Output:
 * 			Function returns DFILE_OK on success or negative in error case
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:		Descriptor out of range
 * 			-ERR_DFILE_NOT_EXISTS:		File not created
 *			-ERR_DFILE_NOT_ALLOWED:		Trying to close a QUEUE file that
 *											was not reopened before.
 */

MInt dfilesys_close_as_random( MUInt df_desc );

#endif

/*
 * 	dfile_reinit:
 * 		This call reinits a file to its empty condition
 * 		Input:
 * 			'df_desc' file descriptor
 * 		Conditions:
 * 			Random file: all of its contents are zeroed and position
 * 				is reset to 0
 * 			Queue file: input and output index are zeroed and
 * 				also queue count
 * 		Output:
 * 			Function returns DFILE_OK on success or negative in error case
 * 		Errors:
 * 			-ERR_DFILE_BAD_FDESC:		Descriptor out of range
 * 			-ERR_DFILE_NOT_EXISTS:		File not created
 */

MInt dfile_reinit( MUInt df_desc );

/*
 * 	dfilesys_sync:
 * 		This call permits all transactional data
 * 		as well as status data to be written to file array
 * 		from caching in RAM
 * 		It's main use is to be called in power fail interrupt
 * 		No input values to function.
 * 		Returns DFILE_OK or negative in error case
 */

MInt dfilesys_sync( void );

