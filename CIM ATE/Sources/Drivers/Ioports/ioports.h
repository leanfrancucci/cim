/*
 * ioports.h
 */

#include "iodefs.h"

MInt set_output( const PIN_TABLE *ppin, const PORT_TABLE *pport );
MInt clear_output( const PIN_TABLE *ppin, const PORT_TABLE *pport );
MInt read_input( const PIN_TABLE *ppin, const PORT_TABLE *pport );
MInt outport( const PORT_TABLE *pport, unsigned char value );
MInt inport( const PORT_TABLE *pport, unsigned char *value );
MInt read_adc( const PIN_TABLE *ppin, const PORT_TABLE *pport, int *val );

const PIN_TABLE * find_pin( const char *pin, void ** pport );
const PORT_TABLE * find_port( const char *port );
