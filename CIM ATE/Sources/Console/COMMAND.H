/*
 *	command.h
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "mytypes.h"

/*
 * 		Maximum arguments to execute a command
 */

#define MAXARGS				11

/*
 * 		Help info for every command. Used by 'help'
 * 		command.
 */

#define LONGHELP			1

/*
 * 		Abbreviated length for every command
 */

#define ABBREVIATED			1

/*
 * 		Include 'echo' command
 */

#define ECHO				1

/*
 * 		Include 'help' command
 */

#define HELP				1


#if	LONGHELP
#define	MK_CMD_TBL_ENTRY(name,lmin,maxargs,cmd,usage,help)	\
				{ name, lmin, maxargs, cmd, usage, help }
#else
#define	MK_CMD_TBL_ENTRY(name,lmin,maxargs,cmd,usage,help)	\
				{ name, lmin, maxargs, cmd, usage }
#endif


typedef struct cmd_tbl_s
{
	char *name;			/* command name					*/
	MInt lmin;			/* minimum abbreviated length	*/
	MInt maxargs;		/* maximum number of arguments	*/
	MInt (*cmd)( const struct cmd_tbl_s *tbl, MInt argc, char *argv[] );
	char *usage;		/* Usage message	(short)		*/
#if	LONGHELP
	char *help;			/* Help  message	(long)		*/
#endif
} CMD_TABLE;

const CMD_TABLE *find_cmd( const char *cmd );

#endif
