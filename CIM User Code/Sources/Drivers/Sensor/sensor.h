/*
 * 	sensor.h
 */

#include "mytypes.h"


#define LOCK1S_MASK	0x01
#define PLG1S_MASK	0x02
#define LOCK2S_MASK	0x04
#define PLG2S_MASK	0x08

#define VAL1ST_MASK	0x40
#define VAL2ST_MASK	0x80

#define KEYS_MASK	0x04

void init_sensors( void );
void proc_sensors( void );

MUInt is_key_switch_open( void );
