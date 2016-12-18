

#include "BLE_Module.h"
#include <system.h>
#include "common.h"
#include "Communications.h"
#include "sps_device_580_FINAL.h"


#define BLE_BINARY sps_device_580_Final_bin

char BLE_TxPayload[255];
char BLE_RxBuffer[255];

/* Pin muxing */
extern int32_t adi_initpinmux(void);


static uint8_t GPIOCallbackMem[ADI_GPIO_MEMORY_SIZE];
char BLE_Payload[255];

//communications flag and RxBuffer
extern bool data_received;
extern unsigned char RxBuffer[255];
void Delay_ms(unsigned int mSec)
{
  int d1, d2;
  d1 = mSec;
  d2 = mSec;
  for ( d1 = 0; d1 < mSec; d1++ )
  {
   for ( d2 = 0; d2 < 3400; d2++ )
   {
   }
  }
}


unsigned char Micro_Init(void)
{
  /* Clock initialization */
    SystemInit();
    
    //set pins
    adi_initpinmux();
    
    /* test system initialization */
    test_Init();
          
    if(adi_pwr_Init()!= ADI_PWR_SUCCESS)
    {
        return 1;
    }
    
    if(ADI_PWR_SUCCESS != adi_pwr_SetClockDivider(ADI_CLOCK_HCLK,1))
    {
        return 1;
    }
     
    if(ADI_PWR_SUCCESS != adi_pwr_SetClockDivider(ADI_CLOCK_PCLK,1))
    {
        return 1;
    }  
    
    if(adi_gpio_Init(GPIOCallbackMem, ADI_GPIO_MEMORY_SIZE)!= ADI_GPIO_SUCCESS)
    {
      return 1;
    }
    
    return 0;
}


void main(void)
{
  char err;
  
  err = Micro_Init();
  if(err)
  {
    DEBUG_MESSAGE("Micro failed to boot\n");
	}
	
	if(Ble_Spi_Boot(BLE_BINARY, IMAGE_SIZE) != 0)
    DEBUG_MESSAGE("Dialog14580 failed to boot\n");
	
	Uart_Init();
	
	while(1)
	{
		sprintf(BLE_TxPayload, "Hello World!\n");
		Ble_Uart_Write(BLE_TxPayload);
		//DEBUG_MESSAGE(BLE_RxBuffer);
    Delay_ms(500);
	}

}