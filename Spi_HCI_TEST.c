

#include <drivers/spi/adi_spi.h>
#include "common.h"


ADI_SPI_RESULT          eSpiResult;                     //SPI error variable
static ADI_SPI_HANDLE   hSPIDevice;                     //SPI handle
static uint8_t          SPIMem[ADI_SPI_MEMORY_SIZE];    //SPI memory size
static ADI_SPI_TRANSCEIVER transceive;                  //transceive struct for SPI Read/Writes


bool flow_Flag = 0;
uint8_t *msg_ptr = NULL;


void SPICallback( void *pAppHandle, uint32_t nEvent, void *pArg)
{
  //CASEOF (event type)
    switch (nEvent)
    {
        //CASE (TxBuffer has been cleared, Data sent) 
        case ADI_SPI_EVENT_READY_EDGE:
                spi_receive_hci_msg();
                break;
                
    default: break;
    }
}


/**********************************************************************************************
* Function Name: Spi_Init                                                                   
* Description  : This function initializes SPI and creates a handle which is configured accordingly
* Arguments    : void                                                                       
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Spi_hci_Init(void)
{
  eSpiResult = adi_spi_Open(SPI_DEV_NUM,SPIMem,ADI_SPI_MEMORY_SIZE,&hSPIDevice);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Set the SPI clock rate 
  eSpiResult = adi_spi_SetBitrate(hSPIDevice,SPI_BITRATE);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //set HCI flow control for slave device, flow control through DRDY pin
  eSpiResult = adi_spi_SetFlowMode(hSPIDevice,ADI_SPI_FLOW_MODE_RDY);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //callback for DRDY signal
  eSpiResult = adi_spi_RegisterCallback(hSPIDevice,SPICallback,hSPIDevice);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Set the chip select 
  eSpiResult = adi_spi_SetChipSelect(hSPIDevice, SPI_CS_NUM);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Enable continue mode (Chip Select remains low until the end of the transaction
  eSpiResult = adi_spi_SetContinousMode(hSPIDevice, true);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Disable DMA
  eSpiResult = adi_spi_EnableDmaMode(hSPIDevice, false);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  else
    return 0;  
}


/**********************************************************************************************
* Function Name: spi_receive_hci_msg                                                                  
* Description  : This function is called by ISR to read message sent by slave
* Arguments    : msg_ptr                                                                       
* Return Value : msg_type    
**********************************************************************************************/
uint16_t spi_receive_hci_msg(uint8_t *msg_ptr)
{
    uint16_t i, size, msg_type;
    
    uint8_t * rd_ptr;
    unsigned char *msg;
    
    rd_ptr = msg_ptr+2;         // Discard 0x0500;
    
    Spi_Write(DREADY_ACK,1);    // Write DREADY acknowledge
    
    Spi_ReadWrite(0x00, 1, *msg_ptr++, 1);
    
    if (*(msg_ptr-1) == 0x05)                   // HCI Message
    {
        msg_ptr++;                              // Align 16-bit
        for (i=0;i<6;i++)
        {
            Spi_ReadWrite(0x00, 1, *msg_ptr++, 1);
        }
        Spi_ReadWrite(0x00, 1, *msg_ptr, 1);
        size = *msg_ptr++;
        Spi_ReadWrite(0x00, 1, *msg_ptr, 1);
        size += *msg_ptr++<<8;
        for (i=0; i<size; i++)
        {
            Spi_ReadWrite(0x00, 1, *msg_ptr++, 1);
        }
        
        msg = malloc(size+8);
        memcpy(msg,rd_ptr,size+8);
        EnQueue(&SPIRxQueue, msg);
        app_env.size_rx_queue++;
        msg_type = 1;                               // GTL Message
    }
    else if (*(msg_ptr-1) == FLOW_ON_BYTE)          // Flow ON
    {
        app_env.slave_on_sleep = SLAVE_AVAILABLE;
        msg_type = 2;                               // Flow Message
    }
    else if (*(msg_ptr-1) == FLOW_OFF_BYTE)         // Flow OFF
    {
        app_env.slave_on_sleep = SLAVE_UNAVAILABLE;
        msg_type = 3;                               // Flow Message
    }
    else
    {
        msg_type = 0;   // Error    
    }
        
    spi_cs_high();      // Close CS

    return msg_type;
}