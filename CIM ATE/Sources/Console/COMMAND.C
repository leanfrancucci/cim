/*
 *	command.c
 */

#include "command.h"
#include "console.h"
#include "mytypes.h"
#include "conser.h"
#include "formats.h"

/*
 * 		Here, include all include files of 
 * 		your commands. Complete the 'MK_CMD_TBL_ENTRY' 
 * 		macro, on this file, for your commands.
 */

#include "cmdshell.h"
#include "cmdset.h"

#include <string.h>

#ifdef DOS_PLATFORM
#include <conio.h>
#endif


#define	CMD_TBL_HELP	MK_CMD_TBL_ENTRY(											\
	"help",		1,	MAXARGS, do_help,												\
	"help\t- Print online help\n",													\
	"[command ...]\n"																\
	"\t- Show help information for command\n"										\
	"\t  Help prints online help for the monitor commands.\n\n"						\
	"\t  Without arguments, it prints a short usage message for all commands.\n\n" 	\
	"\t  To get detailed help information for specific commands you can type\n"		\
	"\t  'help' with one or more command names as arguments.\n"						\
    ),

#define	CMD_TBL_QUES	MK_CMD_TBL_ENTRY(		\
	"?",		1,	MAXARGS,	do_help,		\
	"?\t- Alias for 'help'\n",					\
	NULL										\
    ),

#define CMD_TBL_ECHO	MK_CMD_TBL_ENTRY(					\
	"echo",		4,	MAXARGS,	do_echo,					\
	"echo\t- Echo args to console\n",						\
	"[args..]\n"											\
	"\t- Echo args to console; \\c suppresses newline\n"	\
    ),

static MInt do_echo( const CMD_TABLE *cmdtp, MInt argc, char *argv[] ),
			do_help( const CMD_TABLE *cmdtp, MInt argc, char *argv[] );


/*
 * 		The commands in this table are sorted alphabetically by the
 * 		command name and in descending order by the command name string
 * 		length. This is to prevent conflicts in command name parsing.
 * 		Please ensure that new commands are added according to that rule.
 */

static const CMD_TABLE cmd_tbl[] =
{
#if HELP
	CMD_TBL_QUES
#endif
#if ECHO
	CMD_TBL_ECHO
#endif
#if HELP
	CMD_TBL_HELP
#endif
	CMD_TBL_SHELL
	CMD_TBL_SETB
	CMD_TBL_CLRB
	CMD_TBL_GETB
	CMD_TBL_GETA
	CMD_TBL_OUTP
	CMD_TBL_INP
	CMD_TBL_STAT

	/* the following entry terminates this table */
	MK_CMD_TBL_ENTRY( NULL, 0, 0, NULL, NULL, NULL )
};


#if ECHO
static
MInt
do_echo( const CMD_TABLE *cmdtp, MInt argc, char *argv[] )
{
	MUInt i, putnl = 1;
	char c, *p;

	for( i = 1; i < argc; i++ )
	{
		p = argv[ i ];

		if( i > 1 )
			conser_putc( ' ' );
		while( ( c = *p++ ) != '\0' )
			if( c == '\\' && *p == 'c' )
			{
				putnl = 0;
				p++;
			}
			else
				conser_putc( c );
	}

	if( putnl )
		conser_putc( '\n' );
	return 0;
}
#endif

/*
 * do_help:
 *
 * 		Use conser_puts() instead of myprintf() to avoid
 *		myprintf buffer overflow for long help messages.
 */

#if HELP
static
MInt
do_help( const CMD_TABLE *cmdtp, MInt argc, char *argv[] )
{
	MUInt i;
	MUInt rcode = 0;

	/* Print short help (usage) */

	if( argc == 1 )
	{
		for( cmdtp = &cmd_tbl[0]; cmdtp->name; cmdtp++ )
		{
			if( cmdtp->usage == NULL )
				continue;
			conser_puts( cmdtp->usage );
		}
		return 0;
	}

	/* Command help (long version) */

	for( i = 1; i < argc; ++i )
	{
		if( ( cmdtp = find_cmd( argv[i] ) ) != NULL )
		{
#ifdef LONGHELP
			/* found - print (long) help info */
			conser_puts( cmdtp->name );
			conser_putc(' ');
			if( cmdtp->help )
				conser_puts( cmdtp->help );
			else
			{
				conser_puts("- No help available.\n");
				rcode = 1;
			}
			conser_putc('\n');
#else	/* no long help available */
			if( cmdtp->usage )
				conser_puts( cmdtp->usage );
#endif
		}
		else
		{
#ifdef PRINT_FORMATS
			myprintf( 0, "Unknown command '%s' - try 'help'"
				" without arguments for list of all"
				" known commands\n\n", argv[i] );
#else
			conser_puts( "Unknown command - try 'help'"
				" without arguments for list of all"
				" known commands\n\n" );
#endif
			rcode = 1;
		}
	}
	return rcode;
}
#endif

/*
 * find_cmd:
 *
 * 		Find command table entry for a command.
 */

const 
CMD_TABLE *
find_cmd( const char *cmd )
{
	const CMD_TABLE *p;

	for( p = &cmd_tbl[ 0 ]; p->name != NULL ; ++p )
#if ABBREVIATED 
		if( strncmp( cmd, p->name, p->lmin ) == 0 )
#else
		if( strcmp( cmd, p->name ) == 0 )
#endif
			return p;
	return NULL;
}
