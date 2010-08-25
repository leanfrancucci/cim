/*
 * cmdset.h
 */


#include "mytypes.h"
#include "command.h"

#define SETB_MAX_ARGS	11
#define SETB_MIN_ARGS	2
#define SETB_LMIN		3
#define CMD_TBL_SETB	MK_CMD_TBL_ENTRY(								\
	"set",	SETB_LMIN,	SETB_MAX_ARGS,	do_set,						 	\
	"set\t- Sets to HIGH LEVEL the specified list of pin numbers\n",	\
	"[[pin number1][pin label1]] [[pin number2][pin laber2]] ...\n"		\
	"\n\t- Sets the specified list of pin numbers\n"					\
	"\t  The limit of pin numbers that can be listed is 10\n"			\
    ),

MInt do_set( const CMD_TABLE *p, MInt argc, char *argv[] );

#define CLRB_MAX_ARGS	11
#define CLRB_MIN_ARGS	2
#define CLRB_LMIN		3
#define CMD_TBL_CLRB	MK_CMD_TBL_ENTRY(								\
	"clr",	CLRB_LMIN,	CLRB_MAX_ARGS,	do_clear,						\
	"clr\t- Clears to LOW LEVEL the specified list of pin numbers\n",	\
	"[[pin number1][pin label1]] [[pin number2][pin laber2]] ...\n"		\
	"\n\t- Clears the specified list of pin numbers\n"					\
	"\t  The limit of pin numbers that can be listed is 10\n"			\
    ),

MInt do_clear( const CMD_TABLE *p, MInt argc, char *argv[] );

#define GETB_MAX_ARGS	2
#define GETB_MIN_ARGS	2
#define GETB_LMIN		4
#define CMD_TBL_GETB	MK_CMD_TBL_ENTRY(			\
	"getb",	GETB_LMIN,	GETB_MAX_ARGS,	do_getb,	\
	"getb\t- Gets the specified I/O pin level\n",	\
	"[[pin number1][pin label1]]\n"					\
	"\n\t- Gets the specified I/O pin level\n"		\
	"\t  for analogs pins use: <geta> command.\n"	\
	),

MInt do_getb( const CMD_TABLE *p, MInt argc, char *argv[] );

#define OUTP_MAX_ARGS	3
#define OUTP_MIN_ARGS	3
#define OUTP_LMIN		2
#define CMD_TBL_OUTP	MK_CMD_TBL_ENTRY(							\
	"outp",	OUTP_LMIN,	OUTP_MAX_ARGS,	do_outport,					\
	"outp\t- Configures the Outputs pins of the specified port\n",	\
	"[[port number][port label]] [value]\n"							\
	"\n\t- Outs [value] in the specified [port]\n"					\
    ),

MInt do_outport( const CMD_TABLE *p, MInt argc, char *argv[] );

#define INP_MAX_ARGS	2
#define INP_MIN_ARGS	2
#define INP_LMIN		2
#define CMD_TBL_INP	MK_CMD_TBL_ENTRY(									\
	"inp",	INP_LMIN,	INP_MAX_ARGS,	do_inport,						\
	"inp\t- Gets the specified port status\n",							\
	"[[port number][port label]]\n"								  		\
	"\n\t- Gets the specified port status\n"							\
	"\t  ADC pins reads as 0, for analogs pins use: <geta> command.\n"	\
	),

MInt do_inport( const CMD_TABLE *p, MInt argc, char *argv[] );

#define GETA_MAX_ARGS	2
#define GETA_MIN_ARGS	2
#define GETA_LMIN		4
#define CMD_TBL_GETA	MK_CMD_TBL_ENTRY(					\
	"geta",	GETA_LMIN,	GETA_MAX_ARGS,	do_geta,			\
	"geta\t- Gets the specified ADC pin value in Volts\n",	\
	"[[pin number][pin label]]\n"							\
	"\n\t- Gets the specified ADC pin value in Volts\n"		\
	"\t  for logics pins use: <getb> command.\n"			\
	),

MInt do_geta( const CMD_TABLE *p, MInt argc, char *argv[] );

#define STAT_MAX_ARGS	1
#define STAT_MIN_ARGS	1
#define STAT_LMIN		2
#define CMD_TBL_STAT	MK_CMD_TBL_ENTRY(			\
	"stat",	STAT_LMIN,	STAT_MAX_ARGS,	do_status,	\
	"stat\t- Gets all I/O, ADC pin status\n",		\
	"\n\t- Gets all I/O, ADC pin status\n"			\
	"\t  for detailed info about each pin use:\n"	\
	"\t  <getb>, <getb> commands.\n"				\
	),

MInt do_status( const CMD_TABLE *p, MInt argc, char *argv[] );
