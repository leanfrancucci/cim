/*
 * 	dffatal.h
 * 		Data Flash fatal errors
 */

#include "mytypes.h"

enum
{
	DFOK,
	BAD_OPCODE, BAD_DEVICE, BAD_ADDRESS,
	BAD_POINTER,

	NUM_FATAL_ERRORS
};

MUInt dataflash_fatal( MUInt error_code );

