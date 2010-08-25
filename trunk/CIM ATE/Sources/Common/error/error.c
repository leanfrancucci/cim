/*
 * error.c
 */
#include "error.h"
#include "mytypes.h"
#include "formats.h"
#include <string.h>

static const char *error_tbl[ERRORS_NUM] =
{
	"Invalid PIN Number",
	"Invalid PORT Number",
	"PIN must be output",
	"PIN must be logic I/O",
	"PIN must be ADC input",
	"At Least one PIN must be assigned",
	"At Least one PORT must be assigned",
	"Invalid Command Option",
};


void
log_error( MUInt error_num )
{
	if( error_num < ERRORS_NUM )
		myprintf( 0, "\nError: %u - %s.\n", error_num, error_tbl[error_num] );
}
