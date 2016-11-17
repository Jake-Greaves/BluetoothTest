
#ifndef __DIALOG_SPI_M350_H
#define __DIALOG_SPI_M350_H


/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/

#include <services/gpio/adi_gpio.h>

/******************************************************************************/
/* BLE parameters                                                             */
/******************************************************************************/


#define SPI_ACK 0x02
#define SPI_NACK 0x20

#define RESET_LENGTH     10 //ms
#define MAX_ATTEMPTS     5000

#define BLE_RST_PIN     ADI_GPIO_PIN_12
#define BLE_RST_PORT    ADI_GPIO_PORT0
#define BLE_LED_PIN    ADI_GPIO_PIN_4
#define BLE_LED_PORT    ADI_GPIO_PORT2

#define RESET_LENGTH     10 //ms

/******************************************************************************/
/* Function Prototypes                                                       */
/******************************************************************************/

//boot BLE module using SPI interface
uint32_t Ble_Spi_Boot(uint8_t const * bin, uint32_t length);

//calculate check value
uint8_t calc_crc(uint8_t const * bin, uint32_t length);

#endif /* __DIALOG_SPI_M350_H */
