/*
 * csframe.h
 *
 * 		Specification of ctsafe (CS) protocol's frame.
 */

/*
 * 	Frame specs:
 *
 * 	NoBytes:	|	  1		|  1  |	 1  |   2   |  n   |  1  |
 * 	
 * 				| PROT_MARK | DEV | CMD | NDATA | DATA | CHK |
 */

/* -------------------------- Fields -------------------------- */


/*
 * Field: MARK or SYNC
 *
 * 		Indicate that possible frame is arriving.
 */

#define PROT_MARK			0xF8

/*
 * Field: DEV
 *
 * 		Indicate the desired device. See 'csdevs' include
 * 		file for available devices.
 */

/*
 * Field: CMD
 *
 * 		Indicate the desired command to execute on
 * 		received device. Here is described the possible
 * 		direction of frame that support it.
 * 		See 'cscmds' include file for available devices.
 */

/*
 * Field: DATA
 *
 * 		Contains payload.
 */

/*
 * Field: NDATA
 * 
 * 		Length of payload field.
 */

/*
 * Field: CHK
 *
 * 		The last character of the line is a checksum for the frame.
 * 		The checksum value is calculated by taking the two's
 * 		complement of the sum of all the preceeding data bytes,
 * 		excluding the checksum byte itself and the mark at the 
 * 		beginning of the frame.
 *
 * 		Two's complement:
 *
 * 		In finding the two's complement of a binary number, the 
 * 		bits are inverted, by using the bitwise NOT operation; 
 * 		the value of 1 is then added to the resulting value. 
 * 		Bit overflow is ignored.
 */

/*
 * 		The size of NDATA field in bytes.
 */

#define NDATA_SIZE			sizeof(ushort)

/*
 * 		Here are defined the position of every fields.
 */

enum
{
	MARK_POS, DEV_POS, CMD_POS,	NDATA_POS
};

#define DATA_POS			(NDATA_POS + NDATA_SIZE)

#define MAX_NDATA_LEN		512	
#define VAL_MAX_FRAME_SIZE	128
#define MAX_FRAME_SIZE		(MAX_NDATA_LEN + 4 + NDATA_SIZE)
