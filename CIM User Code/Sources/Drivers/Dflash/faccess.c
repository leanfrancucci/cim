/*
 * 	faccess.c
 * 		Permits access to file
 * 		details
 *
 * 	Changed on 9/22/2007 19:56 ARG
 * 		For queue files power up reflects
 * 		if a file is full or empty when both
 * 		pointers matches
 * 		Also, each pointer wraps to next page
 * 		when pointer advances to next to last page byte.
 * 		If the next page where the pointer goes is the
 * 		next to last one, wraps to first page
 * 	Changed on 3/7/2008
 * 		'blank_whole_system' now formats
 * 		also the second memory
 * 		'create_file' now writes in second memory
 * 		if fd == FW_IMAGE
 */

#include <string.h>

#include "allinc.h"
#include "dfdata.h"

#include "faccess.h"

#include "dfmanage.h"


/*
 *	Structure in page
 *		This structure permits to know in each
 *		page, information about to which file it pertains (if to any).
 *			used: 0, page not used, 1 page used;
 *			if( used )
 *				access: type of access
 *				fd: file descriptor number
 *				unit_size: size of object unit in file (max 511)
 *				if( access == QUEUE )
 *					if( index_in > 0 )
 *						index_in in page
 *					if( index_out > 0 )
 *						indecx_out in page
 *		Also, calculates the first page available for
 *		using in a .new file, because all files has their
 *		pages contiguous amd the files itselfs are
 *		contiguous.
 */

typedef struct
{
	bitf	used:1;			/*	page used									*/
	bitf	access:1;		/*	access type									*/
	bitf	fdesc:5;		/*	file descriptor <0-31>						*/
	bitf	qfull:1;		/*	For QUEUE files: full						*/
	bitf	unit_size:8;	/*	Size of each register						*/
	short	index_in;		/*	Measured in bytes from beginning of page	*/
	short	index_out;		/*	Measured in bytes from beginning of page	*/
} PAGE_INFO_T;

typedef struct
{
	FILESYS_T	file_data;
	ushort		beg_page;
	ushort		last_page;
} DIRINFO_T;

/*	For page buffering	*/

#define NO_PAGE	((uint)(-1))

/*	For writing index pinfo members */

enum
{
	ADD_VALUE, SET_VALUE
};

static PAGE_INFO_T pinfo;
static int current_fd;
static ushort first_avail_page;
static DIRINFO_T directory[ NUM_FDS ], *pcurr;
static ushort page_in_buffer = NO_PAGE;
static MUInt dirty = 0;
static uchar queue_full;


/*
 * 	Structure for reading and writing in flash from file system
 * 	This module uses only BUFFER1
 */

static DFLASH_T df =
{
/*	pbuff		size			base_page			page	address	offset	copy	both	berase	device			wbuff	*/
	NULL,		0,			FILE_SYSTEM_START,		0,		0,		0,		0,		0,		1,		MAIN_DEVICE,	BUFFER1
};

/*
 * -----	Static functions
 */

/*
 * 	read_page:
 * 		Reads a flash page
 * 		See if is there a page change.
 * 			In that case, if page_in_buffer exists and dirty
 * 				writes old page
 * 		Reads new page and saves in page_in_buffer
 */

static
MInt
read_page( DFLASH_T *pf )
{
	if( pf->page == page_in_buffer )
		return DFILE_OK;
	flush_page_in_buffer( 0 );
	page_in_buffer = pf->page;
	return df_read_page( pf );
}

/*
 * 	write_page:
 * 		Remembers that the page in buffer
 * 		is now dirty
 * 		It doesn't write the page
 */

static
MInt
write_page( DFLASH_T *pf )
{
	if( page_in_buffer != pf->page )
		flush_page_in_buffer( 0 );
	page_in_buffer = pf->page;
	dirty = 1;
	return DFILE_OK;
}

static
void
read_pinfo( void ) 
{
	DFLASH_T df1;

	df1 = df;
	df.address = page_eff_size;
	df.pbuff = (uchar *)&pinfo;
	df.size = sizeof( pinfo );
	df_read_data( &df );
	df = df1;
}

static
void
write_pinfo( void )
{
	DFLASH_T df1;

	df1 = df;
	df.address = page_eff_size;
	df.pbuff = (uchar *)&pinfo;
	df.size = sizeof( pinfo );
	df_write_data( &df );
	df = df1;
}


static
MUInt
adjust_out( MUInt code, int value )
{
	MUInt to_next;

	to_next = 0;
	read_pinfo();
	if( code == ADD_VALUE )
	{
#ifdef WITH_WRAP_BUG
		if( ( pinfo.index_in += (short)value ) >= page_eff_size )
#else
		pinfo.index_out += value;
		if( ( page_eff_size - pinfo.index_out ) < pinfo.unit_size )
#endif
		{
			pinfo.index_out = -1;
			to_next = 1;
		}
	}
	else
		pinfo.index_out = (short)value;
	if( value >= 0 )
		pinfo.qfull = 0;
	write_pinfo();
	write_page( &df );
	return to_next;
}

static
MUInt
adjust_in( MUInt code, int value )
{
	MUInt to_next;

	to_next = 0;
	read_pinfo();
	if( code == ADD_VALUE )
	{
#ifdef WITH_WRAP_BUG
		if( ( pinfo.index_in += (short)value ) >= page_eff_size )
#else
		pinfo.index_in += value;
		if( ( page_eff_size - pinfo.index_in ) < pinfo.unit_size )
#endif
		{
			pinfo.index_in = -1;
			to_next = 1;
		}
	}
	else
		pinfo.index_in = (short)value;
	if( pinfo.index_in >= 0 && pinfo.index_out >= 0 )
		pinfo.qfull = pinfo.index_in == pinfo.index_out;
	else
		pinfo.qfull = 0;
	write_pinfo();
	write_page( &df );
	return to_next;
}


/*
 * 	erase_page_info:
 * 		Set the page as not used
 * 		Writes it in page
 */

static
void
erase_page_info( void )
{
	memset( &pinfo, 0, sizeof(pinfo) );
	write_pinfo();
}

/*
 * 	erase_dir_buffer:
 * 		Erases directory information in memory
 * 		Also, 'first_avail_page' is reset to 0
 * 		in order to dispose of all space
 */

static
void
erase_dir_buffer( void )
{
	memset( directory, 0, sizeof( directory ) );
	current_fd = -1;
	first_avail_page = 0;
}

/*
 *-----	The following three functions
 *		are used for constructing directory
 *		area
 */

/*
 * 	info_open:
 * 		From a first page ocuppied from
 * 		an existing file, sets directory curren pointer to
 * 		it and fills access and unit size information as
 * 		well as first file page and begins to count how
 * 		many num units the file has
 */

static
void
info_open( void )
{
	pcurr = &directory[ current_fd ];
	pcurr->file_data.file_type = (uchar)pinfo.access;
	pcurr->file_data.unit_size = (uchar)pinfo.unit_size;
	pcurr->beg_page = df.page;
	queue_full = 0;
}

static
ulong
convert_to_external_index( int ix, uint page )
{
	return (ix + (ulong)page_eff_size * (page - pcurr->beg_page))/pcurr->file_data.unit_size;
}

/*
 * 	info_add:
 * 		Another page for the same file has been encountered
 * 		If is of the QUEUE type, see if one or both indexes are in the page
 * 		In any case, num_units is actualized
 */

static
void
info_add( void )
{
	if( pcurr->file_data.file_type == QUEUE )
	{
		if( pinfo.index_in >= 0 )
			pcurr->file_data.in_index = convert_to_external_index( pinfo.index_in, df.page );
		if( pinfo.index_out >= 0 )
			pcurr->file_data.out_index = convert_to_external_index( pinfo.index_out, df.page );
		if( pinfo.qfull )
			queue_full = 1;
	}
}

/*
 * 	info_close:
 * 		The file last page was the immediatly last one
 * 		Ending page is saved and, in case of QUEUE file,
 * 		'num_elems' contained in between our and in index are
 * 		calculated
 * 		Actual random position is cleared
 */


static
void
info_close( void )
{
	long num;

	if( current_fd == FD_FW )
	{
		pcurr->beg_page = 0;
		pcurr->last_page = NUM_FW_PAGES - 1;
	} else
		pcurr->last_page = df.page - 1;
	first_avail_page = df.page;
	pcurr->file_data.num_units = (ulong)(df.page - pcurr->beg_page)*page_eff_size/pcurr->file_data.unit_size;
	if( pcurr->file_data.file_type == QUEUE )
	{
#if 0
		pcurr->file_data.num_elems = (pcurr->file_data.in_index - pcurr->file_data.out_index);
		if( pcurr->file_data.num_elems < 0 )
			pcurr->file_data.num_elems += pcurr->file_data.num_units;
#else
		pcurr->file_data.num_elems = (pcurr->file_data.in_index - pcurr->file_data.out_index);
		num = (pcurr->file_data.in_index - pcurr->file_data.out_index);

		if( num < 0 )
			pcurr->file_data.num_elems = pcurr->file_data.num_units + num;
#endif
		if( pcurr->file_data.num_elems == 0 && queue_full )
			pcurr->file_data.num_elems = pcurr->file_data.num_units;
	}
	pcurr->file_data.position = 0L;
}

/*
 * 	open_sweep_dir:
 * 		This function is called before
 * 		a search for directory information is made
 * 		in whole file system
 */

static
void
open_sweep_dir( void )
{
	erase_dir_buffer();
}

/*
 * 	close_sweep_dir:
 * 		This function is called after all
 * 		search for directory information
 * 		was made
 */

static
void
close_sweep_dir( void )
{
	
}

/*
 * 	get_dir_info:
 * 		For each page, looks into the info
 * 		contained in page
 * 		Returns fd associated with page
 * 		or negative if no more pages
 */

static
MInt
get_info_dir( void )
{
	if( !pinfo.used )
	{
		if( current_fd >= 0 )
			info_close();
		return -1;
	}
	if( pinfo.fdesc != (uint)current_fd )
	{
		if( current_fd >= 0 )
			info_close();
		current_fd = pinfo.fdesc;
		info_open();
	}
	info_add();
	return (MInt)current_fd;
}

static
void
read_one( ulong position, uchar *p, MUInt access )
{
	MUInt unit_size, to_next;
	ulong actual_position;

	to_next = 0;
	actual_position = (ulong)(position * pcurr->file_data.unit_size);
	df.page = (ushort)( (ulong)(pcurr->beg_page) + actual_position / page_eff_size );
	read_page( &df );

	df.pbuff = p;
	df.address = (ushort)( actual_position % page_eff_size );
	if( df.address <= page_eff_size - (unit_size = pcurr->file_data.unit_size ) )
	{
		df.size = (uchar)unit_size;
		if( p != NULL )
			df_read_data( &df );
		if( access == QUEUE )
			to_next = adjust_out( ADD_VALUE, unit_size );
	}
	else
	{
		MUInt just_read;

		df.size = (ushort)(page_eff_size - df.address);
		just_read = ( MUInt )df.size;
		if( p != NULL )
			df_read_data( &df );

		if( ++df.page >= pcurr->last_page + 1 )
			df.page = pcurr->beg_page;
		read_page( &df );
		df.address = 0;
		if( access == QUEUE )
			adjust_out( SET_VALUE, -1 );

		df.pbuff = p + just_read;
		df.size = (uchar)(unit_size - just_read);
		if( p != NULL )
			df_read_data( &df );
		if( access == QUEUE )
			to_next = adjust_out( SET_VALUE, unit_size - just_read );
	}
	if( to_next )
	{
		if( ++df.page >= pcurr->last_page + 1 )
			df.page = pcurr->beg_page;
		read_page( &df );
		adjust_out( SET_VALUE, 0 );
	}
}

static
void
write_one( ulong position, uchar *p, MUInt access )
{
	MUInt unit_size, to_next;
	ulong actual_position;

	to_next = 0;
	actual_position = (ulong)(position * pcurr->file_data.unit_size);
	df.page = (ushort)( (ulong)(pcurr->beg_page) + actual_position / page_eff_size );
	read_page( &df );

	df.address = (ushort)( actual_position % page_eff_size );
	df.pbuff = p;
	if( df.address <= page_eff_size - (unit_size = pcurr->file_data.unit_size ) )
	{
		df.size = (uchar)unit_size;
		df_write_data( &df );
		if( access == QUEUE )
			to_next = adjust_in( ADD_VALUE, unit_size );
#if 1
		write_page( &df );
#else
		if( ( status = write_page( &df ) ) != DFILE_OK || ( status = df_copy_page( &df, emerg ) ) != DFILE_OK )
			dataflash_fatal( status );
#endif
	}
	else
	{
		MUInt just_write;

		df.size = (ushort)(page_eff_size - df.address);
		just_write = (MUInt)df.size;
		df_write_data( &df );
		if( access == QUEUE )
			adjust_in( SET_VALUE, -1 );
#if 1
		write_page( &df );
#else
		if( ( status = write_page( &df ) ) != DFILE_OK || ( status = df_copy_page( &df, emerg ) ) != DFILE_OK )
			dataflash_fatal( status );
#endif

		if( ++df.page >= pcurr->last_page + 1 )
			df.page = pcurr->beg_page;
		read_page( &df );

		df.address = 0;
		df.pbuff = p + just_write;
		df.size = (uchar)(unit_size - just_write);
		df_write_data( &df );
		if( access == QUEUE )
			to_next = adjust_in( SET_VALUE, unit_size - just_write );
#if 1
		write_page( &df );
#else
		if( ( status = write_page( &df ) ) != DFILE_OK || ( status = df_copy_page( &df, emerg ) ) != DFILE_OK )
			dataflash_fatal( status );
#endif
	}
	if( to_next )
	{
		if( ++df.page >= pcurr->last_page + 1 )
			df.page = pcurr->beg_page;
		read_page( &df );
		adjust_in( SET_VALUE, 0 );
	}
}

static
int
read_one_random( uchar *p )
{
	if( pcurr->file_data.position >= pcurr->file_data.num_units )
		return 0;		/*	In EOF	*/
	read_one( pcurr->file_data.position, p, RANDOM );
	pcurr->file_data.position++;
	return 1;
}


static
int
read_one_queue( uchar *p )
{
	if( pcurr->file_data.num_elems == 0 )
		return 0;		/*	No more items waiting	*/
	read_one( pcurr->file_data.out_index, p, QUEUE );
	pcurr->file_data.num_elems--;
	if( ++pcurr->file_data.out_index >= pcurr->file_data.num_units )
		pcurr->file_data.out_index = 0;
	return 1;
}


static
int
read_one_false_queue( uchar *p )
{
	long position;

	if( pcurr->file_data.num_elems == 0 )
		return 0;		/*	No more items waiting	*/
	if( pcurr->file_data.position >= pcurr->file_data.num_elems )
		return 0;		/*	In EOF	*/
	position = pcurr->file_data.position + pcurr->file_data.out_index;
	if( position >= (long)pcurr->file_data.num_units )
		position -= pcurr->file_data.num_units;
	read_one( position, p, RANDOM );
	if( ++pcurr->file_data.position > pcurr->file_data.num_elems )
		pcurr->file_data.position = 0;
	return 1;
}

static
int
write_one_random( uchar *p )
{
	if( pcurr->file_data.position >= pcurr->file_data.num_units )
		return 0;		/*	In EOF	*/
	write_one( pcurr->file_data.position, p, RANDOM );
	pcurr->file_data.position++;
	return 1;
}


static
int
write_one_queue( uchar *p )
{
	if( pcurr->file_data.num_elems >= pcurr->file_data.num_units )
		return 0;		/*	No more space for items	*/
	write_one( pcurr->file_data.in_index, p, QUEUE );
	pcurr->file_data.num_elems++;
	if( ++pcurr->file_data.in_index >= pcurr->file_data.num_units )
		pcurr->file_data.in_index = 0;
	return 1;
}


static
int
write_one_false_queue( uchar *p )
{
	if( pcurr->file_data.num_elems >= pcurr->file_data.num_units )
		read_one_queue( NULL );
	write_one( pcurr->file_data.in_index, p, QUEUE );
	pcurr->file_data.num_elems++;
	if( ++pcurr->file_data.in_index >= pcurr->file_data.num_units )
		pcurr->file_data.in_index = 0;
	pcurr->file_data.position = pcurr->file_data.out_index;
	return 1;
}

static
int
read_random_current_file( uint num_units, uchar *pbuff )
{
	uint num;
	uchar *p1;

	for( p1 = pbuff, num = 0 ; num < num_units ; ++num, pbuff += pcurr->file_data.unit_size )
		if( pbuff > p1 + LEO_BUFFER || read_one_random( pbuff ) == 0 )
			break;
	return num;
}

static
int
read_queue_current_file( uint num_units, uchar *pbuff )
{
	uint num;
	uchar *p1;

	for( p1 = pbuff, num = 0 ; num < num_units ; ++num, pbuff += pcurr->file_data.unit_size )
		if( pbuff > p1 + LEO_BUFFER || read_one_false_queue( pbuff ) == 0 )
			break;
	return num;
}

static
int
write_random_current_file( uint num_units, uchar *pbuff )
{
	uint num;

	for( num = 0 ; num < num_units ; ++num, pbuff += pcurr->file_data.unit_size )
		if( write_one_random( pbuff ) == 0 )
			break;
	return num;
}

static
int
write_queue_current_file( uint num_units, uchar *pbuff )
{
	uint num;

	for( num = 0 ; num < num_units ; ++num, pbuff += pcurr->file_data.unit_size )
		if( write_one_false_queue( pbuff ) == 0 )
			break;
	return num;
}

/*
 *	exists_at_beginning:
 *		Return:
 *			If there is a file or files that ocuppies the first
 *			pages where the FW file must be, returns FW_NOTFREE
 *			If there are space but there is no other file in that space
 *			or the file is FD_FW, returns FW_CANFILL.
 *			Else returns FW_OK
 */

MInt
exists_at_beginning( void )
{
	MUInt fd;
	DIRINFO_T *p;

	for( p = directory, fd = 0; fd < NUM_FDS; ++fd, ++p )
	{
		if( p->file_data.unit_size == 0 )				/*	Exists ?	*/
		{
			if( fd == FD_FW )
				return FW_NOFD;
			continue;
		}
		if( p->beg_page >= 0 && p->last_page < NUM_FW_PAGES )
			if( fd != FD_FW )
				return FW_NOTFREE;
	}
	return FW_CANFILL;
}

/*
 * 	get_page:
 * 		Reads a 'page' from 'device' and writes
 * 		it's checksum in '*pcheck' if no errors
 * 		Returns 0 if ok or -1 on error
 */

#define FW_PAGE_SIZE 512
static
MInt
get_page( DFLASH_T *pf, MUInt device, ushort *pcheck, ushort *pfcheck )
{
	MInt status;
	uchar *p;

	flush_page_in_buffer( 1 );
	pf->device = device;
	if( df_read_page( pf ) != DFILE_OK )
	{
		*pcheck = 0;
		status = -1;
	} else
	{
		*pcheck = *(ushort *)pf->pbuff;
		status = 0;
		for( p = fs_buff; p < (fs_buff+FW_PAGE_SIZE); ++p )
			*pfcheck += *p;
	}
	pf->device = MAIN_DEVICE;
	return status;
}

static
int
compare_versions( DFLASH_T *pf )
{
	MInt status;

	status = 0;
	pf->page = 0;
	flush_page_in_buffer( 1 );
	pf->device = MAIN_DEVICE;
	if( df_read_page( pf ) != DFILE_OK )
		status = -1;
	
	flush_page_in_buffer( 1 );
	pf->wbuff = 0;

	pf->device = SECONDARY_DEVICE;
	if( df_read_page( pf ) != DFILE_OK )
		status = -1;

	pf->wbuff = 1;
	
	pf->device = MAIN_DEVICE;

	if( status != -1 )
		return memcmp( fs_buff, user_buff, sizeof(fs_buff) );
	else
		return status;
}

static
int
verify_file_checksum( DFLASH_T *pf, MUInt device, ushort check  )
{
	ushort p;
	MInt status;

	status = 0;				/*	No estaba, daba error por falta de inicialización (EAM 2008 03 23)	*/
	pf->page = 0;
	flush_page_in_buffer( 1 );
	pf->device = device;
	if( df_read_page( pf ) != DFILE_OK )
		status = -1;

	check -= fs_buff[0];
	check -= fs_buff[1];

	if( ~check != *(ushort *)fs_buff )
		status = -1;

	pf->device = MAIN_DEVICE;
	return status;
}

/*
 * 	verify_both_memories:
 * 		Read both memories in each page that must
 * 		be occupied by FD_FW and returns:
 *
 * 		FW_BLANK:	Both files has pages in error
 * 		FW_21:		main file has pages in error but secondary is ok
 * 		FW_12:		main page is ok but secondary has pages in error
 * 		FW_DIFF:	main and secondary pages are ok but has some
 * 					pages where checksum differs
 * 		FW_OK:		main and secondary pages are ok and all pages
 * 					has same checksums in both
 */

MInt
verify_both_memories( void )
{
	static ushort check_main, check_sec, check_main_fw, check_sec_fw;
	MInt st_main, st_sec;
	uint main_errors, sec_errors, diffs, ver_diffs;

	main_errors = sec_errors = diffs = check_sec_fw = check_sec_fw = ver_diffs = 0;

	for( df.page = 0 ; df.page < NUM_FW_PAGES ; ++df.page )
	{
		if( ( st_main = get_page( &df, MAIN_DEVICE, &check_main, &check_main_fw ) ) < 0 )
			++main_errors;
		if( ( st_sec = get_page( &df, SECONDARY_DEVICE, &check_sec, &check_sec_fw ) ) < 0 )
			++sec_errors;
		if( st_main >= 0 && st_sec >= 0 && check_main != check_sec )
			++diffs;
	}

#ifndef TEST_NO_FW

	if( verify_file_checksum( &df, MAIN_DEVICE, check_main_fw ) < 0 )
		++main_errors;
	if( verify_file_checksum( &df, SECONDARY_DEVICE, check_sec_fw ) < 0 )
		++sec_errors;

	if( main_errors == 0 && sec_errors == 0 )
		ver_diffs = compare_versions( &df );

#endif

	if( main_errors != 0 )
	{
#if 0
		if( sec_errors != 0 )
			return FW_BLANK;
		return FW_21;
#else
		return FW_BLANK;
#endif
	}
	if( sec_errors != 0 || diffs != 0 || ver_diffs != 0 )
		return FW_12;
	return FW_OK;

}

static
void
create_and_blank_firmware( void )
{
	pcurr = &directory[ FD_FW ];

	pcurr->file_data.file_type = RANDOM;
	pcurr->file_data.unit_size = 1;
	pcurr->beg_page = 0;
	pcurr->last_page = NUM_FW_PAGES - 1;
	pcurr->file_data.num_units = FW_IMAGE;

	pinfo.access = RANDOM;
	pinfo.fdesc = FD_FW;
	pinfo.unit_size = 1;
	pinfo.used = 1;
	blank_current_file( 1 );
}

/*
 * 	----- Public functions
 */

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


MInt
verify_and_recover_firmware_file( void )
{
	MInt status;


	if( ( status = exists_at_beginning() ) ==  FW_NOTFREE || status == FW_NOFD )
		return status;
	flush_page_in_buffer( 1 );
	switch( status = verify_both_memories() )
	{
		case FW_BLANK:
			create_and_blank_firmware();
			break;
		case FW_21:
			copy_fw_file( SECONDARY_DEVICE );
			break;
		case FW_12:
			copy_fw_file( MAIN_DEVICE );
			break;
		case FW_NOTHING_TO_DO:
			break;
		default:
			break;
	}

	return status;
}

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

uint
construct_dir( ushort *pfd_ex, ushort *pfd_err )
{
	ushort fd_exists, fd_inerror, err_pages;
	MInt fd;

	fd =  -1;
	open_sweep_dir();
	fd_exists = fd_inerror = err_pages = 0;
	for( df.page = 0 ; df.page < fs_num_pages ; ++df.page )
	{
		if( read_page( &df ) != DFILE_OK )
		{
			++err_pages;
			if( fd > 0 )
				fd_inerror |= fd2bit[ fd ];
			continue;
		}

		df.address = page_eff_size;
		df.pbuff = (uchar *)&pinfo;
		df.size = sizeof( pinfo );
		df_read_data( &df );

		if( ( fd = get_info_dir() ) < 0 )		/*	Last page of information?	*/
			break;
		fd_exists |= fd2bit[ fd ];
	}
	if( df.page >= fs_num_pages && current_fd >= 0 )
		info_close();
	close_sweep_dir();
	if( pfd_ex != NULL )
		*pfd_ex = fd_exists;
	if( pfd_err != NULL )
		*pfd_err = fd_inerror;
	return err_pages;
}

/*
 * 	make_file_current:
 * 		Receives a fd and, if fd exists, set it as current
 * 		and returns TRUE; else returns FALSE
 */

MUInt
make_file_current( MUInt fd )
{
	if( directory[ fd ].file_data.unit_size == 0 )
		return 0;
	current_fd = fd;
	pcurr = &directory[ fd ];
	return 1;
}

/*
 * 	get_existing_space:
 * 		Receives 'unit_size' of object to be stored
 * 		Returns maximum 'unit_number' that can be covered
 * 		with the pages that are not assigned yet
 */

ulong
get_existing_space( MUInt unit_size )
{
	return (fs_num_pages - first_avail_page)*(ulong)page_eff_size/unit_size;
}

/*
 * 	get_current_file_status:
 * 		For a file known to exists and set as current
 * 		with a prior call to 'make_file_current'
 * 		returns as reference its status
 */

MInt
get_current_file_status( FILESYS_T *psta )
{
	*psta = pcurr->file_data;
	return DFILE_OK;
}

/*
 * 	blank_whole_system:
 * 		Formats each available page to
 * 		not used and also erase memory directory
 * 		Data contents are not initizialized
 */

void
blank_whole_system( void )
{
	MInt status;

	df.both = 1;
	for( df.page = 0; df.page < fs_num_pages ; ++df.page )
	{
		erase_page_info();
#if 0
		write_page( &df );
#else
	if( ( status = write_page( &df ) ) != DFILE_OK || ( status = df_copy_page( &df, emerg ) ) != DFILE_OK )
		dataflash_fatal( status );
#endif
	}
	erase_dir_buffer();
	df.both = 0;
}

/*
 * 	blank_current_file:
 * 		For an existing file and current with
 * 		a prior call to 'make_file_current'
 * 		set, for each page, its data contents to 0
 * 		and sets only for first page both indices to 0
 */ 		

MInt
blank_current_file( MUInt in_init )
{
	uchar buffer[16];
	MUInt first;

	for( first = 1, df.page = pcurr->beg_page ; df.page <= pcurr->last_page ; ++df.page )
	{
		read_page( &df );
		df.pbuff = buffer;
		df.size = sizeof( buffer );
		memset( buffer, 0, sizeof( buffer ) );
		for( df.address = 0 ; df.address < page_eff_size ; df.address += df.size )
			df_write_data( &df );
		if( !in_init )
			read_pinfo();
		pinfo.index_in = pinfo.index_out = ( first ? 0 : -1 );
		first = 0;
		pinfo.qfull = 0;
		write_pinfo();
#if 1
		write_page( &df );
#else
		if( ( status = write_page( &df ) ) != DFILE_OK || ( status = df_copy_page( &df, emerg ) ) != DFILE_OK )
		{
			df.both = 0;
			return status;
		}
#endif
	}
	pcurr->file_data.in_index = pcurr->file_data.out_index = 0;
	pcurr->file_data.num_elems = pcurr->file_data.position = 0L;
	return DFILE_OK;
}

/*
 * 	create_file:
 * 		For an fd that is known that doesn't exists
 * 		creates a new file.
 * 		First make entry in directory
 *		Then simply calls to blank_current_file
 */

long
create_file( MUInt fd, MUInt unit_size, MUInt access, ulong num_units )
{
	ushort num_pages;
	ulong num_bytes;

	pcurr = &directory[ fd ];

	memset( pcurr, 0, sizeof( pcurr ) );
	pcurr->file_data.file_type = (uchar)access;
	pcurr->file_data.unit_size = (uchar)unit_size;
	pcurr->beg_page = first_avail_page;
	num_bytes = (ulong)unit_size * num_units;
	num_pages = (ushort)(num_bytes / page_eff_size);
	if( (num_bytes % page_eff_size) != 0 )
		++num_pages;
	pcurr->last_page = (first_avail_page += num_pages) - 1;
	pcurr->file_data.num_units = num_pages * (ulong)page_eff_size / unit_size;
	pinfo.access = access;
	pinfo.fdesc = fd;
	pinfo.unit_size = unit_size;
	pinfo.used = 1;
	blank_current_file( 1 );
	return pcurr->file_data.num_units;
}

/*
 *	read_current_file:
 *		Reads a random or queue file
 *		Must be called after 'make_file_current'
 */

int
read_current_file( uint num_units, uchar *pbuff )
{
	return ( pcurr->file_data.file_type == RANDOM ) ?
		read_random_current_file( num_units, pbuff ):
		read_queue_current_file( num_units, pbuff );
}

/*
 *	write_current_file:
 *		Writes a random or queue file.
 *		Must be called after 'make_file_current'
 */

int
write_current_file( uint num_units, uchar *pbuff )
{
	return ( pcurr->file_data.file_type == RANDOM ) ?
		write_random_current_file( num_units, pbuff ):
		write_queue_current_file( num_units, pbuff );
}

/*
 * 	seek_current:
 * 		After a 'make_file_current' changes
 * 		position in a file opened as RANDOM or QUEUE
 */

MInt
seek_current( long offset, MUInt whence )
{
	long position;
	long num;

	num = pcurr->file_data.file_type == RANDOM ? 
				pcurr->file_data.num_units:
				pcurr->file_data.num_elems;

		switch( whence )
		{
			case FD_SEEK_CUR:
				position = pcurr->file_data.position;
				break;
			case FD_SEEK_END:
				position = num;
				break;
			case FD_SEEK_SET:
			default:
				position = 0L;
				break;
		}
		position += offset;
		if( position < 0 || position > num )
			return -ERR_DFILE_BAD_SEEK;
		pcurr->file_data.position = position;
		return DFILE_OK;
}

/*
 * 	get_current_unit_size:
 * 		gets object size of current file
 */

uint
get_current_unit_size( void )
{
	return pcurr->file_data.unit_size;
}

/*
 * 	flush_page_in_buffer
 * 		If there is a page in buffer and it's dirty
 * 		then writes it in flash. Clears dirty
 * 		If argument is true, then page_in_buffer
 * 		is voided
 */

MInt
flush_page_in_buffer( MUInt void_page )
{
	DFLASH_T dft;
	MInt status;

	if( page_in_buffer == NO_PAGE || !dirty )
		status = DFILE_OK;
	else
	{
		dft = df;
		dft.page = page_in_buffer;
		dirty = 0;
		status = df_write_page( &dft );
	}
	if( void_page )
		page_in_buffer = NO_PAGE;
	return status;
}

