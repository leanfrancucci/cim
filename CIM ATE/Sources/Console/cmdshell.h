/*
 * cmdshell.h
 */


#include "mytypes.h"
#include "command.h"


#define CMD_TBL_SHELL	MK_CMD_TBL_ENTRY(				\
	"shell",	4,	1,	do_shell,						\
	"shell\t- show command shell setup\n",				\
	NULL												\
    ),

MInt do_shell( const CMD_TABLE *p, MInt argc, char *argv[] );
