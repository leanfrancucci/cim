/*
 * 	dfmanage.c
 * 		Routines for data flash management
 * 		(read, write, program, etc. )
 *
 * 		Version 1.0  08/18/2007 13:13 ARG
 * 		Version 1.1  08/26/2007 16:23 ARG
 * 			Changed position of checksum
 * 		Version 1.2	 09/03/2008	14:20 ARG
 * 		   
 *
 * 		DelsatGroup S.A.
 * 		Programmer: E.A.Martinez
 */

#include <stdio.h>
#include <string.h>

#include "allinc.h"

#include "dfvars.h"
#include "dfmanage.h"

#include "dataflsh.h"


/*
 * 	NUM_CALC_BUFF is used for reading
 * 	chunks of data.
 * 	NOTE: Must be 8 'always' because the
 * 	lower density flash is of 256+8 bytes
 * 	DON'T change this define !
 */

#define NUM_CALC_BUFF		8

/*
 * 	MAX_CHUNK is the chunk size for reading
 * 	or writing a flash contents from its buffer
 *
 */

#define MAX_CHUNK			16
#define NUM_PROG_RETRIES	3

static MUInt emergency;

/*
 * --------  Static functions
 */

/*
 * 	get_next_power:
 */

uint
get_next_power( uint size )
{
	uint power, last_power;

	for( last_power = 0, power = 2 ; size >= power ; last_power = power, power *= 2 )
		if( size == power )
			return power;
	return last_power;
}

/*
 * 	program_page;
 * 		Program page in pf
 */

MInt
program_page( DFLASH_T *pf )
{
	MUInt retry;

	pf->pbuff = pf->wbuff ? fs_buff : user_buff;
	pf->size = page_size;
	pf->address = 0;
	pf->berase = 1;
	for( retry = 0 ; retry < NUM_PROG_RETRIES ; ++retry )
	{
		dataflash_main_memory_page_program_through_buffer( pf );
		if( dataflash_main_memory_page_to_buffer_compare( pf ) )
			return DFILE_OK;
	}
	return -ERR_FLASH_BAD_PROGRAM;
}

/*
 *	copy_buffer_devices:
 *		Copies information between devices
 *		in same page
 */

static
MInt
copy_buffer_devices( DFLASH_T *pto, DFLASH_T *pfrom )
{
	return df_write_page( pto );
}

/*
 *	copy_to_same:
 *		Mirrors information in same device
 *		iff 'copy' is true
 */

static
MInt
copy_to_same( DFLASH_T *pf )
{
	DFLASH_T df;

	if( !pf->copy )
		return DFILE_OK;
	df = *pf;
	df.page += df.offset;
	return program_page( &df );
}

/*
 *	copy_to_other:
 *		Copies information in one device
 *		to other device iff 'both' is true.
 *		Includes mirroring in the same device
 *		iff 'copy' is true
 */

static
MInt
copy_to_other( DFLASH_T *pf )
{
	DFLASH_T df;
	MInt status;

	if( !pf->both )
		return DFILE_OK;
	df = *pf;
	df.device ^= 1;
	if( ( status = copy_buffer_devices( &df, pf ) ) != DFILE_OK )
		return status;
	if( !df.copy )
		return DFILE_OK;
	return copy_to_same( &df );
}

/*
 *	df_format_this_page:
 *		Sets 0 in all data positions,
 *		sets checksum
 */

static
MInt
df_format_this_page( DFLASH_T *pf )
{
	memset( pf->wbuff ? fs_buff : user_buff, 0, page_eff_size );
	return df_write_page( pf );
}

/*
 * 	calculate_checksum
 * 		Calculates checksum for a whole page
 * 		in 'device' and 'buffer'.
 * 		Returns checksum calculated
 */

static
uint
calculate_checksum( DFLASH_T *pf )
{
	uint check, count;
	uchar *p;

	for( p = pf->wbuff ? fs_buff : user_buff, check = count = 0 ; count < ( page_size - sizeof(ushort) ) ; ++count )
		check += (uint)(*p++);
	return ~check;
}

/*
 * ------ Public functions
 */

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

MUInt
dfmanage_init( MUInt emergency )
{
	uint sec_page_size, main_page_size;

	/* mem_status: declared extern in dfvars */

	emerg = (uchar)emergency;
	dataflash_init();

	sec_page_size = main_page_size = mem_status = 0;
	
	if( dataflash_query_type( SECONDARY_DEVICE ) == 0 )
		mem_status = SEC_MEM_FAIL;
	else
	{
		page_size = sec_page_size = (ushort)dataflash_page_size( SECONDARY_DEVICE );
		page_eff_size = (ushort)get_next_power( page_size );
		df_num_pages = (ushort)dataflash_page_num( SECONDARY_DEVICE );	
	}

	if( !emerg )
	{
		if( dataflash_query_type( MAIN_DEVICE ) == 0 )
			mem_status |= MAIN_MEM_FAIL;
		else
		{
			main_page_size = dataflash_page_size( MAIN_DEVICE );
			
			if( ( mem_status & SEC_MEM_FAIL ) != SEC_MEM_FAIL && main_page_size != sec_page_size )
				return mem_status = BOTH_MEM_FAIL;

			page_size = (ushort)main_page_size;
			page_eff_size = (ushort)get_next_power( page_size );
			df_num_pages = (ushort)dataflash_page_num( MAIN_DEVICE );	
		}
	}

	return mem_status;
}


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

MInt
df_copy_page( DFLASH_T *pf, MUInt emerg )
{
	MInt status;

	if( ( status = copy_to_same( pf ) ) != DFILE_OK )
		return status;
	return emerg ? DFILE_OK : copy_to_other( pf );
}

/*
 * 	df_format_page:
 *
 * 		formats a page from 'device' setting
 * 		all bytes to 0
 * 		Then based on 'copy' and 'both' copies
 * 		format (calls df_copy_page)
 */

MInt
df_format_page( DFLASH_T *pf )
{
	MInt status;

	if( (status = df_format_this_page( pf ) ) != DFILE_OK )
		return status;
	return df_copy_page( pf, emergency );
}

/*
 * 	df_read_page:
 *
 * 		Reads a page to its data flash buffer
 * 		In pf->pbuff remains the checksum value
 * 		pointing to static 'check_read'
 */

MInt
df_read_page( DFLASH_T *pf )
{
	ushort check_calc;
	static ushort check_read;

	pf->address = 0;
	pf->pbuff = pf->wbuff ? fs_buff : user_buff;
	pf->size = page_size;
	dataflash_main_memory_page_read( pf );
	check_calc = (ushort)calculate_checksum( pf );

	pf->pbuff = (uchar *)&check_read;
	pf->size = sizeof( check_read );
	pf->address = page_size - sizeof( check_read );
	df_read_data( pf );

	return check_calc == check_read ? DFILE_OK : -ERR_DFILE_SECTOR_ERROR;
}

/*
 * 	df_read_data:
 *
 * 		Reads data from page in data flash buffer
 */

MInt
df_read_data( DFLASH_T *pf )
{
	memcpy( pf->pbuff, ( pf->wbuff ? fs_buff : user_buff ) + pf->address, pf->size );
	return DFILE_OK;
}

/*
 * 	df_write_data:
 *
 * 		Writes data to page in data flash buffer
 */

MInt
df_write_data( DFLASH_T *pf )
{
	memcpy( ( pf->wbuff ? fs_buff : user_buff ) + pf->address, pf->pbuff,  pf->size );
	return DFILE_OK;
}

/*
 * 	df_write_page:
 *
 * 		Writes to flash data in flash buffer
 * 		Calculates checksum
 */

MInt
df_write_page( DFLASH_T *pf )
{
	ushort check;
	DFLASH_T df;

	df = *pf;
	check = (ushort)calculate_checksum( &df );
	df.pbuff = (uchar *)&check;
	df.size = sizeof( check );
	df.address = page_size - sizeof( check );
	df_write_data( &df );

	return program_page( pf );
}

/*
 *	modify_page:
 * 		Writes an object trough 'buffer' in 'device'
 * 		Object pointed by '*pobj' and size in 'size'
 * 		Tries to replicate in backup page and
 * 		in backup device
 * 		Returns DFILE_OK or negative in error
 */

MInt
modify_page( DFLASH_T *pf )
{
	MInt status;
	DFLASH_T df;

	df = *pf;
	df_read_page( &df );
	df_write_data( pf );
	if( ( status = df_write_page( pf ) ) != DFILE_OK )
		return status;
	return df_copy_page( pf, 0 );
}

/*
 * 	copy_fw_file:
 * 		Copies firmware file from beginning
 * 		of FS 'from_device'to other device
 * 		as a whole
 */

void
copy_fw_file( MUInt from_device )
{
	DFLASH_T df;

	df.pbuff		= fs_buff;
	df.size			= WPAGE_SIZE;
	df.base_page	= FILE_SYSTEM_START;
	df.wbuff		= BUFFER1;
	df.address		= 0;

	for( df.page = 0 ; df.page < NUM_FW_PAGES ; ++df.page )
	{
		df.device = from_device;
		dataflash_main_memory_page_read( &df );
		df.device ^= 1;
		program_page( &df );
	}
}

/*
 *	complement_check_and_write:
 *		Used only for test purposes
 */

void
complement_check_and_write( DFLASH_T *pf )
{
	ushort check_read;

	pf->pbuff = (uchar *)&check_read;
	pf->size = sizeof( check_read );
	pf->address = page_size - sizeof( check_read );
	df_read_data( pf );
	check_read ^= 0xffff;
	df_write_data( pf );
	dataflash_buffer_to_main_memory_program( pf );
}
