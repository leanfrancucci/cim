/*
 * 	dfmanage.h
 */

#include "mytypes.h"

/*
 * 	df_manage_init:
 * 		First (and always first!) sets emergency in 'emerg'
 * 		Inits flash system
 * 		Query types of one or both memories
 * 		Returns 1 if all ok or 0 if one or both
 * 		data flashes are not classified or pages
 * 		does not match
 * 		Calculates page_eff_size, df_num_pages.
 */

MUInt dfmanage_init( MUInt emergency );

/*
 * 	program_page;
 * 		Program page in pf
 */

MInt program_page( DFLASH_T *pf );

/*
 * 	df_copy_page:
 *
 * 		copies a page from one device
 * 		to the backup or main and/or from
 * 		present device to the other device based
 * 		on data on pf, according to:
 * 			device = from which device
 * 			page = origin page
 * 				if copy == 1 then copies to the
 * 				same device in page + offset
 * 					(offset to be considered as signed)
 * 			if both == 1 then copies to other device
 */

MInt df_copy_page( DFLASH_T *pf, MUInt emerg );

/*
 * 	df_format_page:
 *
 * 		formats a page from 'device' setting
 * 		all bytes to 0
 * 		Then based on 'copy' and 'both' copies
 * 		format (calls df_copy_page)
 */

MInt df_format_page( DFLASH_T *pf );

/*
 * 	df_read_page:
 *
 * 		Reads a page to its data flash buffer
 * 		In pf->pbuff remains the checksum value
 */

MInt df_read_page( DFLASH_T *pf );

/*
 * 	df_read_data:
 *
 * 		Reads data from page in data flash buffer
 */

MInt df_read_data( DFLASH_T *pf );

/*
 * 	df_write_data:
 *
 * 		Writes data to page in data flash buffer
 */

MInt df_write_data( DFLASH_T *pf );

/*
 * 	df_write_page:
 *
 * 		Writes to flash data in flash buffer
 * 		Calculates checksum
 * 		Tries to copy in backup page
 * 		and in backup device
 */

MInt df_write_page( DFLASH_T *pf );

/*
 *	modify_page:
 * 		Writes an object trough 'buffer' in 'device'
 * 		Object pointed by '*pobj' and size in 'size'
 * 		Tries to replicate in backup page and
 * 		in backup device
 * 		Returns DFILE_OK or negative in error
 */

MInt modify_page( DFLASH_T *pf );

/*
 * 	copy_fw_file:
 * 		Copies firmware file from beginning
 * 		of FS 'from_device'to other device
 * 		as a whole
 */

void copy_fw_file( MUInt from_device );
/*
 *	complement_check_and_write:
 *		Used only for test purposes
 */

void complement_check_and_write( DFLASH_T *pf );
	
