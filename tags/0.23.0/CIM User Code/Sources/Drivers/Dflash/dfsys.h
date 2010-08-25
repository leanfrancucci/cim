/*
 * 		dfsys.h
 * 			internal view
 *
 * 		Data Flash fILE SYStem
 * 		New file system for vault project
 *
 * 		Delsat Group S.A.
 * 		Modification: 8/9/2007 15:38 Version 0.1
 *
 * 		Version 0.1
 *
 * 		Eduardo Martinez
 */

#include "mytypes.h"
/*
 *	In .c file must be included
#include "dfdata.h"
*/

/*
 * 	Module public functions
 */

/*
 * dfilesys_init:
 * 		Inits the file system
 * 		Inputs:
 * 			No inputs
 * 		Outputs:
 * 			No outputs.
 *
 * 		Must be called after calling
 * 		'dfmanage_init'
 */

uint dfilesys_init( ushort *pdf_ex, ushort *pdf_err );


/*
 * 	dfilsys_object_size:
 * 		Returns object size of file
 * 		fd.
 * 		If positive, is size object
 * 		If negative, error
 */

int
dfilsys_object_size( MUInt df_desc ); 

/*
 * 	dfilesys_sync:
 * 		This call permits all transactional data
 * 		as well as status data to be written to file array
 * 		from caching in RAM
 * 		It's main use is to be called in power fail interrupt
 * 		No input values to function
 * 		Returns DFILE_OK or negative in error case
 */

MInt dfilesys_sync( void );
