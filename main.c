
#include "BLE_Module.h"
#include <system.h>
#include "common.h"
#include "Communications.h"
#include "Wireless_Sense_Module.h"
#include "Placeholder_TempInterface.h"
#include "ADuCM3029_Sleep_Interface.h"
#include "App_Interface.h"
#include "LibIIO_Interface.h"

#define BLE_BINARY sps_device_580_bin

//Pin muxing
extern int32_t adi_initpinmux(void);

//communications flag for commands
extern bool data_received;

bool deep_Sleep = true;

//rudimentary delay
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
    //GPIO memory for init
    static uint8_t GPIOCallbackMem[ADI_GPIO_MEMORY_SIZE];
  
    //Clock initialization
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


int main(void)
{
  char err;
  
  //init micro
  err = Micro_Init();
  if(err)
    DEBUG_MESSAGE("Micro failed to boot\n");
  
  //boot BLE
  if(Ble_Spi_Boot((uint8_t const*)BLE_BINARY, IMAGE_SIZE) != 0)
    return 1;
  
  //init UART comms
  Uart_Init();
  
  //reset Sense1000 on power cycle
  Sense_Deactivate();
    
  //ensure SENSE is on
  Sense_Activate();
  
  //Read/Write Attributes test
  //char readDevAttr[] = "READ_ATTR iio:device0 PowerMode";
  //char readChannelAttr[] = "READ_ATTR iio:device0 INPUT temp1 SensorType";
  //char writeChannelAttr[] = "WRITE_ATT iio:device0 INPUT temp1 SensorType T";
  //App_Read_Command(readDevAttr);
  
  //WIP. Currently gets stuck reading xml file from device
  //App_Parse_Print_Xml();
  
  //sanity check. Clear command received flag
  data_received = false;
  
  while(1)
  {
    
    App_Read_Command();
    
  }//while loop end

}//main end



