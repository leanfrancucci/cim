/*
 * 	dffatal.c
 * 		Data Flash fatal errors
 */

#include "mytypes.h"
#include "dffatal.h"
#include "reset.h"

MUInt
dataflash_fatal( MUInt error_code )
{
	reset_now();
	return error_code;
}

