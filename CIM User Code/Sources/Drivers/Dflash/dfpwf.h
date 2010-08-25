/*
 * 	dfpwf.h
 * 		Power fail routines
 * 		used for dataflash
 *
 * 		Version 0.1	7/11/07 12:36 ARG
 *
 * 		Delsat Group S.A.
 * 		Eduardo Martinez
 *
 */

void disable_pwf_interrupt( void );

void enable_pwf_interrupt( void );

void dfpwf_interrupt( void );

