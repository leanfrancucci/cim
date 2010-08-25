/*
 * 	dfdata.h
 * 		Data structures and
 * 		definitions for new file system
 *
 * 		Version 1.0 9/3/2007	ARG
 *
 * 		DelsatGroup S.A.
 * 		Eduardo Martinez
 *
 * 		Version 1.1 3/6/2008	ARG
 * 		EAM
 * 		Changed NUM_FDS from 10 to 11 according with space
 * 		in information area of files (5 bits) and fd2bit array
 * 		fd = NUM_FDS-1 reserved for firmware image (FD_FW)
 * 		Defined FW_IMAGE
 *
 */

#define K	1024

/*
 * 	File information structure
 */

/*
 *	Configuration as well as status data
 */

typedef struct
{
	uchar	unit_size;		/*	Size of file record					*/
	uchar	file_type;		/*	Type of file (RANDOM or QUEUE		*/
	ulong	num_units;		/*	File capacity in terms of records	*/

	ulong	out_index;		/*	indices are measured in terms of....	*/
	ulong	in_index;		/*	....number of elements from beginning	*/
	ulong	num_elems;		/*	Elements between in_index and out_index	*/

	ulong	position;		/*	Current position for RANDOM	*/
} FILESYS_T;
	
typedef struct
{
	ushort fd_used;					/*	bit field of file descriptors used 		*/
	ushort fd_inerror;				/*	bit field of file descriptors in error	*/
	uint fs_page_errors;			/*	file system pages in error				*/
	ulong dfs_size;					/*	file system size						*/
} DFS_INFO_T;


/*
 * 	Access types
 */

enum
{
	RANDOM, QUEUE,
	NUM_ACCESS
};

/*
 * 	whence types
 */

enum
{
	FD_SEEK_SET, FD_SEEK_CUR, FD_SEEK_END,
	NUM_WHENCE
};

/*
 * 	File descriptor numbers
 */

enum
{
	DF_FIRST_DESC = 0,		/*	First descriptor number	*/
	NUM_FDS = 11
};

#define FD_FW		(NUM_FDS-1)

/*
 * 	MAX_UNIT_SIZE
 * 		Is the maximum size that a record can have
 */

#define MAX_UNIT_SIZE		255

/*
 * 	FS_NO_FORMAT
 * 		When 'fs_page_errors' is equal to 'fs_num_pages', the flash
 * 		memory does not have format.
 */

#define FS_NO_FORMAT		0xFFFF

/*
 *	Size of firmware image file
 */

#define FW_IMAGE			(64L*K)



