/*
 * iodefs.c
 */
#include "porthdl.h"
#include "ioports.h"
#include "mytypes.h"
#include "iodefs.h"
#include <string.h>

const PIN_TABLE port0_pintbl[] =
{
	{ "VD_PLUNGER",	"00",	"CLOSED",		"OPENED",		BIT0,	OUTPUT	},	
	{ "VD_SW",		"01",	"UNLOCKED",		"LOCKED",		BIT1,	OUTPUT	},
	{ "VD_LOAD",	"02",	"CONNECTED",	"UNCONNECTED",	BIT2,	OUTPUT	},
	{ "MD_PLUNGER",	"03",	"CLOSED",		"OPENED",		BIT3,	OUTPUT	},	
	{ "MD_SW",		"04",	"UNLOCKED",		"LOCKED",		BIT4,	OUTPUT	},
	{ "MD_LOAD",	"05",	"CONNECTED",	"UNCONNECTED",	BIT5,	OUTPUT	},
	{ "STACKER1",	"06",	"INSTALLED",	"UNINSTALLED",	BIT6,	OUTPUT	},
	{ "STACKER2",	"07",	"INSTALLED",	"UNINSTALLED",	BIT7,	OUTPUT	},
	{ NULL,			NULL,	NULL,			NULL,			0	,	0		}
};

const PIN_TABLE port1_pintbl[] =
{
	{ "PRIVDC",		"10",	"ANALOG",	"ANALOG",	BIT0,	ADC 	},	
	{ "SECVDC",		"11", 	"ANALOG",	"ANALOG",	BIT1,	ADC 	},
	{ "VBATT",		"12", 	"ANALOG",	"ANALOG",	BIT2,	ADC 	},
	{ "VDC_VAL1",	"13", 	"ANALOG",	"LOW",		BIT3,	ADC		},
	{ "VDC_VAL2",	"14", 	"ANALOG",	"LOW",		BIT4,	ADC		},
	{ "TP3",		"15", 	"HIGH",		"LOW",		BIT5,	INPUT	},
	{ "TP4",		"16", 	"HIGH",		"LOW",		BIT6,	INPUT	},
	{ "TP5",		"17", 	"HIGH",		"LOW",		BIT7,	INPUT	},
	{ NULL,			NULL,	NULL,		NULL,		0,		0		}
};

const PIN_TABLE port2_pintbl[] =
{
	{ "TP6", 		"22", 	"HIGH",		"LOW",	BIT2,	INPUT	},	
	{ "TP7",		"23", 	"HIGH",		"LOW",	BIT3,	INPUT	},
	{ "TIP_S",		"24", 	"HIGH",		"LOW",	BIT4,	INPUT	},
	{ "RING_S",		"25", 	"HIGH",		"LOW",	BIT5,	INPUT	},
	{ "TIP_O",		"26", 	"HIGH",		"LOW",	BIT6,	OUTPUT	},
	{ "RING_O",		"27", 	"HIGH",		"LOW",	BIT7,	OUTPUT	},
	{ NULL,			NULL,	NULL,		NULL,	0,		0		}
};

const PIN_TABLE port3_pintbl[] =
{
	{ "BATTERY", 	"30", 	"UNCONNECTED",	"CONNECTED",	BIT0,	OUTPUT	},	
	{ "VALSW", 		"31", 	"VALIDATOR2",	"VALIDATOR1",	BIT1,	OUTPUT	},	
	{ "RS485SW", 	"32", 	"SECONDARY",	"PRIMARY",		BIT2,	OUTPUT	},	
	{ "PRI_PWR", 	"33", 	"ON",			"OFF",			BIT3,	OUTPUT	},	
	{ "SEC_PWR", 	"34", 	"ON",			"OFF",			BIT4,	OUTPUT	},	
	{ "KEY_SW", 	"35", 	"CLOSE",		"OPEN",			BIT5,	OUTPUT	},	
	{ "VD_CURR", 	"36", 	"NOT_DRIVED",	"DRIVED",		BIT6,	INPUT	},	
	{ "MD_CURR", 	"37", 	"NOT_DRIVED",	"DRIVED",		BIT7,	INPUT	},	
	{ NULL,			NULL,	NULL,			NULL,			0,		0		}
};

const PIN_TABLE port4_pintbl[] =
{
	{ "TP8", 		"42", 	"HIGH",		"LOW",	BIT2,	INPUT	},	
	{ "TP9", 		"43", 	"HIGH",		"LOW",	BIT3,	INPUT	},	
	{ "TP10", 		"44", 	"HIGH",		"LOW",	BIT4,	INPUT	},	
	{ "TP11", 		"45", 	"HIGH",		"LOW",	BIT5,	INPUT	},	
	{ "TP12", 		"46", 	"HIGH",		"LOW",	BIT6,	INPUT	},	
	{ "TP13", 		"47", 	"HIGH",		"LOW",	BIT7,	INPUT	},	
	{ NULL,			NULL,	NULL,		NULL,	0,		0		}
};

const PIN_TABLE port5_pintbl[] =
{
	{ "LED0",		"50",	"ON",	"OFF",	BIT0, 	OUTPUT	},
	{ "LED1",		"51",	"ON",	"OFF",	BIT1, 	OUTPUT	},
	{ "LED2",		"52",	"ON",	"OFF",	BIT2, 	OUTPUT	},
	{ "LED3",		"53",	"ON",	"OFF",	BIT3, 	OUTPUT	},
	{ "LED4",		"54",	"ON",	"OFF",	BIT4, 	OUTPUT	},
	{ "LED5",		"55",	"ON",	"OFF",	BIT5, 	OUTPUT	},
	{ "LED6",		"56",	"ON",	"OFF",	BIT6, 	OUTPUT	},
	{ "LED7",		"57",	"ON",	"OFF",	BIT7, 	OUTPUT	},
	{ NULL,			NULL,	NULL,	NULL,	0,		0		}
};

const PIN_TABLE port6_pintbl[] =
{
	{ "DURESS_NC",	"63",	"UNACTIVATED",	"ACTIVATED",BIT3,	INPUT	},
	{ "DURESS_NO",	"64",	"UNACTIVATED",	"ACTIVATED",BIT4,	INPUT	},
	{ "BURGLAR_NC",	"65",	"UNACTIVATED",	"ACTIVATED",BIT5,	INPUT	},
	{ "BURGLAR_NO",	"66",	"UNACTIVATED",	"ACTIVATED",BIT6,	INPUT	},
	{ "TP14",		"67",	"HIGH",			"LOW",		BIT7,	INPUT	},
	{ NULL,			NULL,	NULL,			NULL,		0,		0		}
};

const PORT_TABLE port_tbl[] =
{
	{ "PORT0",	"0",	PORT0,	port0_pintbl },	
	{ "PORT1",	"1",	PORT1,	port1_pintbl },
	{ "PORT2",	"2",	PORT2,	port2_pintbl },
	{ "PORT3",	"3",	PORT3,	port3_pintbl },
	{ "PORT4",	"4",	PORT4,	port4_pintbl },
	{ "PORT5",	"5",	PORT5,	port5_pintbl },
	{ "PORT6",	"6",	PORT6,	port6_pintbl },
	{ NULL, 	NULL, 	0, 		NULL }
};

