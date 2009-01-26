/*
 *	ioports.h
 *	
 *	Routines for PORTS initialization.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#ifndef __IOPORTS_H__
#define __IOPORTS_H__

#include "derivative.h"

/*********************************/
/* HARDWARE CONCTIONS DEFINITION */
/*********************************/
/* DATA_REGISTERS */
/* PORT A Data Register */
#define LOCK1S		PTAD_PTAD0	/* input */
#define DOOR1S		PTAD_PTAD1	/* input */
#define LOCK2S		PTAD_PTAD2	/* input */
#define DOOR2S		PTAD_PTAD3	/* input */
#define LOCK1_CURR	PTAD_PTAD4	/* input */
#define LOCK2_CURR	PTAD_PTAD5	/* input */
#define VAL1_STACKS	PTAD_PTAD6	/* input */
#define VAL2_STACKS	PTAD_PTAD7	/* input */
#define SENSORS_IN	PTAD
/* PORT B Data Register */
#define BATT_LVL	PTBD_PTBD0	/* A/D input */
/* PORT C Data Register */
#define UART2_TXD	PTCD_PTCD0	/* SCI2 TX output */
#define UART2_RXD	PTCD_PTCD1	/* SCI2 RX input */
#define LED1		PTCD_PTCD2	/* output */
#define LED2		PTCD_PTCD3	/* output */
#define BVSEL		PTCD_PTCD4	/* output */
#define UART1DIR	PTCD_PTCD5	/* output */
#define MCU_ID		PTCD_PTCD6	/* input */
#define KEYS		PTCD_PTCD7	/* input */
/* PORT D Data Register */
#define DURESS_AL	PTDD_PTDD0	/* input */
#define BURGLAR_AL	PTDD_PTDD1	/* input */
#define WDOG_CTRL	PTDD_PTDD2	/* output */
#define VLOCKS_CTRL	PTDD_PTDD3	/* output */
#define VRLY_CTRL	PTDD_PTDD4	/* output */
#define VAL0PWR		PTDD_PTDD5	/* output */
#define AUX2		PTDD_PTDD6	/* input */
#define VAL1PWR		PTDD_PTDD7	/* output */
/* PORT E Data Register */
#define UART1_TXD	PTED_PTED0	/* SCI1 TX output */
#define UART1_RXD	PTED_PTED1	/* SCI1 RX output */
#define SPI_CS_PM	PTED_PTED2	/* output */
#define SPI_MISO	PTED_PTED3	/* SPI MISO input */
#define SPI_MOSI	PTED_PTED4	/* SPI MOSI output */
#define SPI_CLK		PTED_PTED5	/* SPI CLK output */
#define SPI_CS_M	PTED_PTED6	/* output */
#define E_MEMCTRL	PTED_PTED7	/* output */ 
/* PORT F Data Register */
#define LOCK1A		PTFD_PTFD0	/* output */
#define LOCK1B		PTFD_PTFD1	/* output */
#define LOCK2A		PTFD_PTFD2	/* output */
#define LOCK2B		PTFD_PTFD3	/* output */
#define HOST_PWR	PTFD_PTFD4	/* output */
#define CHRG_CTRL1	PTFD_PTFD5	/* output */
#define BATT_CTRL	PTFD_PTFD6	/* output */
#define CHRG_CTRL0	PTFD_PTFD7	/* output */
/* PORT G Data Register */
#define PRIPWRCTRL 	PTGD_PTGD3	/* output */
#define CIM_ID0		PTGD_PTGD4	/* input */
#define CIM_ID1		PTGD_PTGD5	/* input */
#define CIM_ID2		PTGD_PTGD6	/* input */
#define CIM_ID3		PTGD_PTGD7	/* input */
/*********************************/
/*********************************/
/* DATA DIRECTION REGISTERS */
/* PORT A Data Direction Register */
#define D_LOCK1S		PTADD_PTADD0	/* input */
#define D_DOOR1S		PTADD_PTADD1	/* input */
#define D_LOCK2S		PTADD_PTADD2	/* input */
#define D_DOOR2S		PTADD_PTADD3	/* input */
#define D_LOCK1_CURR	PTADD_PTADD4	/* input */
#define D_LOCK2_CURR	PTADD_PTADD5	/* input */
#define D_VAL1_STACKS	PTADD_PTADD6	/* input */
#define D_VAL2_STACKS	PTADD_PTADD7	/* input */
/* PORT B Data Direction Register */
#define D_BATT_LVL		PTBDD_PTBDD0	/* A/D input */
/* PORT C Data Direction Register */
#define D_UART2_TXD		PTCDD_PTCDD0	/* SCI2 TX output */
#define D_UART2_RXD		PTCDD_PTCDD1	/* SCI2 RX input */
#define D_LED1			PTCDD_PTCDD2	/* output */
#define D_LED2			PTCDD_PTCDD3	/* output */
#define D_BVSEL			PTCDD_PTCDD4	/* output */
#define D_UART1DIR		PTCDD_PTCDD5	/* output */
#define D_MCU_ID		PTCDD_PTCDD6	/* input */
#define D_KEYS			PTCDD_PTCDD7	/* input */
/* PORT D Data Direction Register */
#define D_DURESS_AL		PTDDD_PTDDD0	/* input */
#define D_BURGLAR_AL	PTDDD_PTDDD1	/* input */
#define D_WDOG_CTRL		PTDDD_PTDDD2	/* output */
#define D_VLOCKS_CTRL	PTDDD_PTDDD3	/* output */
#define D_VRLY_CTRL		PTDDD_PTDDD4	/* output */
#define D_VAL0PWR		PTDDD_PTDDD5	/* output */
#define D_AUX2			PTDDD_PTDDD6	/* input */
#define D_VAL1PWR		PTDDD_PTDDD7	/* output */
/* PORT E Data Direction Register */
#define D_UART1_TXD		PTEDD_PTEDD0	/* SCI1 TX output */
#define D_UART1_RXD		PTEDD_PTEDD1	/* SCI1 RX output */
#define D_SPI_CS_PM		PTEDD_PTEDD2	/* output */
#define D_SPI_MISO		PTEDD_PTEDD3	/* SPI MISO input */
#define D_SPI_MOSI		PTEDD_PTEDD4	/* SPI MOSI output */
#define D_SPI_CLK		PTEDD_PTEDD5	/* SPI CLK output */
#define D_SPI_CS_M		PTEDD_PTEDD6	/* output */
#define D_E_MEMCTRL		PTEDD_PTEDD7	/* output */ 
/* PORT F Data Direction Register */
#define D_LOCK1A		PTFDD_PTFDD0	/* output */
#define D_LOCK1B		PTFDD_PTFDD1	/* output */
#define D_LOCK2A		PTFDD_PTFDD2	/* output */
#define D_LOCK2B		PTFDD_PTFDD3	/* output */
#define D_HOST_PWR		PTFDD_PTFDD4	/* output */
#define D_CHRG_CTRL1	PTFDD_PTFDD5	/* output */
#define D_BATT_CTRL		PTFDD_PTFDD6	/* output */
#define D_CHRG_CTRL0	PTFDD_PTFDD7	/* output */
/* PORT G Data Direction Register */
#define D_PRIPWRCTRL	PTGDD_PTGDD3	/* input */
#define D_CIM_ID0		PTGDD_PTGDD4	/* input */
#define D_CIM_ID1		PTGDD_PTGDD5	/* input */
#define D_CIM_ID2		PTGDD_PTGDD6	/* input */
#define D_CIM_ID3		PTGDD_PTGDD7	/* input */
/*********************************/
/*********************************/
/* PULL UP REGISTERS */
/* PORT A PullUp Enable Register */
#define P_LOCK1S		PTAPE_PTAPE0	/* input */
#define P_DOOR1S		PTAPE_PTAPE1	/* input */
#define P_LOCK2S		PTAPE_PTAPE2	/* input */
#define P_DOOR2S		PTAPE_PTAPE3	/* input */
#define P_LOCK1_CURR	PTAPE_PTAPE4	/* input */
#define P_LOCK2_CURR	PTAPE_PTAPE5	/* input */
#define P_VAL1_STACKS	PTAPE_PTAPE6	/* input */
#define P_VAL2_STACKS	PTAPE_PTAPE7	/* input */
/* PORT B PullUp Enable Register */
#define P_BATT_LVL		PTBPE_PTBPE0	/* A/D input */
/* PORT C PullUp Enable Register */
#define P_UART2_TXD		PTCPE_PTCPE0	/* SCI2 TX output */
#define P_UART2_RXD		PTCPE_PTCPE1	/* SCI2 RX input */
#define P_LED1			PTCPE_PTCPE2	/* output */
#define P_LED2			PTCPE_PTCPE3	/* output */
#define P_BVSEL			PTCPE_PTCPE4	/* output */
#define P_UART1DIR		PTCPE_PTCPE5	/* output */
#define P_MCU_ID		PTCPE_PTCPE6	/* input */
#define P_KEYS			PTCPE_PTCPE7	/* input */
/* PORT D PullUp Enable Register */
#define P_DURESS_AL		PTDPE_PTDPE0	/* input */
#define P_BURGLAR_AL	PTDPE_PTDPE1	/* input */
#define P_WDOG_CTRL		PTDPE_PTDPE2	/* output */
#define P_VLOCKS_CTRL	PTDPE_PTDPE3	/* output */
#define P_VRLY_CTRL		PTDPE_PTDPE4	/* output */
#define P_VAL0PWR		PTDPE_PTDPE5	/* output */
#define P_AUX2			PTDPE_PTDPE6	/* input */
#define P_VAL1PWR		PTDPE_PTDPE7	/* output */
/* PORT E PullUp Enable Register */
#define P_UART1_TXD		PTEPE_PTEPE0	/* SCI1 TX output */
#define P_UART1_RXD		PTEPE_PTEPE1	/* SCI1 RX output */
#define P_SPI_CS_PM		PTEPE_PTEPE2	/* output */
#define P_SPI_MISO		PTEPE_PTEPE3	/* SPI MISO input */
#define P_SPI_MOSI		PTEPE_PTEPE4	/* SPI MOSI output */
#define P_SPI_CLK		PTEPE_PTEPE5	/* SPI CLK output */
#define P_SPI_CS_M		PTEPE_PTEPE6	/* output */
#define P_E_MEMCTRL		PTEPE_PTEPE7	/* output */ 
/* PORT F PullUp Enable Register */
#define P_LOCK1A		PTFPE_PTFPE0	/* output */
#define P_LOCK1B		PTFPE_PTFPE1	/* output */
#define P_LOCK2A		PTFPE_PTFPE2	/* output */
#define P_LOCK2B		PTFPE_PTFPE3	/* output */
#define P_HOST_PWR		PTFPE_PTFPE4	/* output */
#define P_CHRG_CTRL1	PTFPE_PTFPE5	/* output */
#define P_BATT_CTRL		PTFPE_PTFPE6	/* output */
#define P_CHRG_CTRL0	PTFPE_PTFPE7	/* output */
/* PORT G PullUp Enable Register */
#define P_PRIPWRCTRL	PTGPE_PTGPE3	/* input */
#define P_CIM_ID0		PTGPE_PTGPE4	/* input */
#define P_CIM_ID1		PTGPE_PTGPE5	/* input */
#define P_CIM_ID2		PTGPE_PTGPE6	/* input */
#define P_CIM_ID3		PTGPE_PTGPE7	/* input */

void ports_init(void);
void loging_led( void );

#endif






