/*
 * pwrhdl.h
 */
#include "mytypes.h"

#define BATTLVL_PROMEDIATION

#ifdef BATTLVL_PROMEDIATION
#define BATTLVL_BUFF_SIZE	10
#endif

void init_pwrhdl( void );

/*
 * MCU Id functions
 */
MUInt get_mcu_id( void );

/*
 * Battery functions
 */
MUInt get_batt_lvl( void );
void connect_battery( void );
void disconnect_battery( void );

/*
 * Battery Charger functions
 */
void connect_bchrg( void );
void disconnect_bchrg( void );

/*
 * Power Source detection functions
 */
MUInt in_pwf( void );
void proc_pwrstatus_irq( void );
void proc_pwrstatus_rti( void );

/*
 * Host Power control function
 */
void set_host_pwr( MUInt ctrl );
/*
 * Forced Primary shutdown from Secondary MCU
 */
void primary_shut_down( void );
