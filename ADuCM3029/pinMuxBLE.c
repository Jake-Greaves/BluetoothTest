/*
 **
 ** Source file generated on October 4, 2016 at 13:39:30.	
 **
 ** Copyright (C) 2016 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** SPI0 (SCLK, MOSI, MISO, CS_0)
 ** I2C0 (SCL0, SDA0)
 ** UART0 (Tx, Rx)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** P0_00, P0_01, P0_02, P0_03, P0_04, P0_05, P0_10, P0_11
 */

#include <sys/platform.h>
#include <stdint.h>

#define SPI0_SCLK_PORTP0_MUX  ((uint16_t) ((uint16_t) 1<<0))
#define SPI0_MOSI_PORTP0_MUX  ((uint16_t) ((uint16_t) 1<<2))
#define SPI0_MISO_PORTP0_MUX  ((uint16_t) ((uint16_t) 1<<4))
#define SPI0_CS_0_PORTP0_MUX  ((uint16_t) ((uint16_t) 1<<6))
#define UART0_TX_PORTP0_MUX  ((uint32_t) ((uint32_t) 1<<20))
#define UART0_RX_PORTP0_MUX  ((uint32_t) ((uint32_t) 1<<22))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *((volatile uint32_t *)REG_GPIO0_CFG) = SPI0_SCLK_PORTP0_MUX | SPI0_MOSI_PORTP0_MUX
     | SPI0_MISO_PORTP0_MUX | SPI0_CS_0_PORTP0_MUX | UART0_TX_PORTP0_MUX | UART0_RX_PORTP0_MUX;

    return 0;
}

