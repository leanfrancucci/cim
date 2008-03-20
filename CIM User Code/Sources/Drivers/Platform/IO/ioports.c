/*
 *	ioports.c
 *	
 *	Routines for PORTS initialization.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#include "derivative.h"
#include "ioports.h"
#include "sleep.h"

void
ports_init(void)
{
	/******** LOCKERS **********/
	/** Inputs **/
	/* Locker 1 Sensor */
	D_LOCK1S = 0;
	P_LOCK1S = 1;
	/* Locker 1 Current Sensor */
	D_LOCK1_CURR = 0;
	/* Door 1 Sensor */
	D_DOOR1S = 0;
	P_DOOR1S = 1;
	/*-------------------------*/
	/* Locker 2 Sensor */
	D_LOCK2S = 0;
	P_LOCK2S = 1;
	/* Door 2 Sensor */
	D_DOOR2S = 0;
	P_DOOR2S = 1;
	/* Locker 2 Current Sensor */
	D_LOCK2_CURR = 0;
	/* Validators Stacker Sensors */
	D_VAL1_STACKS = 0;
	P_VAL1_STACKS = 1;
	D_VAL2_STACKS = 0;
	P_VAL2_STACKS = 1;
	/* Key Switch */
	D_KEYS = 0;
	P_KEYS = 1;
	/***************************/
	/** Outputs **/
	/* Locker 1 */
	LOCK1A = 0;
	D_LOCK1A = 1;
	LOCK1B = 0;
	D_LOCK1B = 1;
	/* Locker 2 */
	LOCK2A = 0;
	D_LOCK2A = 1;
	LOCK2B = 0;
	D_LOCK2B = 1;
	/***************************/

	/* Battery Control */
	D_BATT_LVL = 0;
	CHRG_CTRL0 = 1;
	D_CHRG_CTRL0 = 1;
	CHRG_CTRL1 = 0;
	D_CHRG_CTRL1 = 1;
	BATT_CTRL = 0;
	D_BATT_CTRL = 1;

	/* LED's */
	LED1 = 0;
	D_LED1 = 1;
	LED2 = 0;
	D_LED2 = 1;

	/* Monostables Outputs */
	WDOG_CTRL = 0;
	D_WDOG_CTRL = 1;
	VLOCKS_CTRL = 0;
	D_VLOCKS_CTRL = 1;
	VRLY_CTRL = 0;
	D_VRLY_CTRL = 1;

	/* Bill Validators */
	UART2_TXD = 1;
	D_UART2_TXD = 1;
	D_UART2_RXD = 0;
	P_UART2_RXD = 1;
	BVSEL = 0;
	D_BVSEL = 1;
	
	/* Host interface */
	UART1DIR = 0;
	D_UART1DIR = 1;
	UART1_TXD = 1;
	D_UART1_TXD = 1;
	D_UART1_RXD = 0;
	P_UART1_RXD = 0;
	HOST_PWR = 0;
	D_HOST_PWR = 1;

	/* MCU Primary/Secondary */
	D_MCU_ID = 0;
	P_MCU_ID = 0;
	PRIPWRCTRL = 0;
	D_PRIPWRCTRL = 1;

	/* SPI interface */
	SPI_CS_PM = 1;
	D_SPI_CS_PM = 1;
	D_SPI_MISO = 0;
	SPI_MOSI = 1;
	D_SPI_MOSI = 1;
	SPI_CLK = 1;
	D_SPI_CLK = 1;
	SPI_CS_M = 1;
	D_SPI_CS_M = 1;

	E_MEMCTRL = 0;
	D_E_MEMCTRL = 1;

	/* Alarm */
	DURESS_AL = 0;
	D_DURESS_AL = 1;
	BURGLAR_AL = 0;
	D_BURGLAR_AL = 1;

	/* CIM Id */
	D_CIM_ID0 = 0;
	P_CIM_ID0 = 1;
	D_CIM_ID1 = 0;
	P_CIM_ID1 = 1;
	D_CIM_ID2 = 0;
	P_CIM_ID2 = 1;
	D_CIM_ID3 = 0;
	P_CIM_ID3 = 1;
}

void
loging_led( void )
{
	LED1 ^= 1;
}


