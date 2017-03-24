/*
 * LibIIO_Interface.c - Temp-Module main functions
 *
 * Author: Jake Greaves
 */

#include "Communications.h"
#include <drivers/uart/adi_uart.h>
#include "LibIIO_Utility.h"
#include "LibIIO_Interface.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int LibIIO_Open(void) {

	char *buf;
	char *ptr;
	int ret;
        int i = 1;

        //mux to Sense1000
        Uart_Mux(LIBIIO_MUX);
        
        //allocate memory to buffer
        buf = (char*)malloc(32*sizeof(char));
        
        //build string for command
	snprintf(buf, 32,"OPEN %s %lu ", "iio:device0", (unsigned long) 1);

        //ptr = end of string
	ptr = buf + strlen(buf);

	//add mask value to command
	snprintf(ptr, (ptr - buf) + i * 8, "%08lu", (unsigned long)0x03);

        //end of string
        ptr += 8;
        
        //add \r\n to command
	strcpy(ptr,"\r\n");

        //execute command
	ret = execute_command(buf);
        
        free(buf);

	return ret;
}

int LibIIO_Print(char *xml) {
        
	int ret;

        Uart_Mux(LIBIIO_MUX);
        
	ret = execute_command("PRINT\r\n");
	if (ret < 0)
		return ret;
        
        //trouble using malloc. Returns null pointer so large buffer used for now
	xml = malloc(ret* sizeof(char));
	if (!xml)
		return 1;
        
        int packetSize = 200;
        //check how many read iterations are required
        uint32_t iterations = ret/packetSize;//how many loops needed to send payload with limited UART memory
        uint32_t remainder = ret%packetSize; //check if length matches UART memory length exactly
        
        //Stuck here, data reads in about 800 bytes and then hangs on a BusFault
        //send data
        for(uint32_t i = 0 ; i < iterations ; i++)
        {
          Uart_Read(xml+i*packetSize, packetSize);
          if(i == 3)
            i=3;
        }
        
        //send remainder
        if(remainder > 0)
        {
          Uart_Read(xml+iterations*packetSize, remainder);
        }
        
	//ret = serial_read_all(Buf, xml_len + 1);
	if (ret != 0)
		return ret;
        
        free(xml);
        
	return 0;
}

int LibIIO_ReadBuf(char *retBuf) {

	char *buf; //command and response data buf
	int ret = 0;
	char *pos;
	int offset;

        Uart_Mux(LIBIIO_MUX);
        
        buf = (char*)malloc(32*sizeof(char));
        
        //build LibIIO command
	snprintf(buf, 32, "READBUF %s %lu\r\n",
			"iio:device0", (unsigned long) 64);

        //execute command
	ret = execute_command(buf);
        
        //check that ret is valid
        if(ret < 0)
          return ret;
        
        buf = realloc(buf, ret*sizeof(char));
        
        
        //read the corresponding response. ret value is the length
	ret = serial_read_data((void*)buf, ret);
        
        if(ret < 0)
              return ret;
        //data contains the mask delimited by \n
	pos = strstr(buf, "\n");

        //get offset to remove mask
	offset = pos-buf;
        offset++;
        
        //copy data without mask
	strcpy(retBuf, buf + offset);
        
        free(buf);

	return ret;
}

int LibIIO_ReadAttr(char *dev_id, char *chn, char* attr, bool output, char *retBuf)
{
        char *buf;
        int ret, len;
        
        Uart_Mux(LIBIIO_MUX);
        
        buf = (char*)malloc(64*sizeof(char));
        //only reading of channels is supported for now
        if (chn)
                //build command
		snprintf(buf, 64, "READ %s %s %s %s\n\r", dev_id,
				output ? "OUTPUT" : "INPUT",
				chn, attr ? attr : "");
        
        else
          //build command
		snprintf(buf, 64,"READ %s %s\n\r", dev_id, attr ? attr : "");

        //execute command to read the return length
	ret = execute_command(buf);
        
        if (ret < 0)
          return ret;

        len = ret;
	/* +1: Also read the trailing \n */
	ret = serial_read_data(retBuf, len + 1);
        
        retBuf[len + 1] = '\0';
        
        free(buf);

	if (ret < 0)                
                return ret;
        
        return 0;
}

int LibIIO_WriteAttr(char *dev_id, char *chn, char* attr, bool output, char *writeVal)
{
	char *buf;
	int ret;
	int resp;
        unsigned long len = strlen(writeVal)+1;

        Uart_Mux(LIBIIO_MUX);
        
	if (!attr)
		if (!chn)
			return 1;
        
        buf = (char*)malloc(64*sizeof(char));
        
        if(chn)
            snprintf(buf, 64, "WRITE %s %s %s %s %lu\r\n", dev_id,
			output ? "OUTPUT" : "INPUT",
			chn, attr ? attr : "",
			(unsigned long) len);
        else
            snprintf(buf, 64, "WRITE %s %s %lu\r\n", dev_id,
                          attr ? attr : "",
                            (unsigned long) len);
        
	ret = serial_write_data(buf, strlen(buf));
	if (ret < 0)
		return ret;

	ret = serial_write_data(writeVal, len);
	if (ret < 0)
		return ret;

	ret = serial_read_integer(&resp);
	if (ret < 0)
		return ret;
        
        free(buf);

	return resp;
}

int Sense_Wait_For_Boot(void)
{
        //wait for Sense1000 calibrations and boot
	Delay_ms(SENSE1000_RESET_LENGTH);

	return 0;
}

int Sense_Activate(void)
{
	//Ensure reset pin is high
	adi_gpio_SetHigh(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN);
	adi_gpio_OutputEnable(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN, true);

	//wait for calibration
	Sense_Wait_For_Boot();

	//Open device
	LibIIO_Open();

	return 0;
}

int Sense_Deactivate(void)
{
        //hold reset pin low
	adi_gpio_SetLow(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN);
	adi_gpio_OutputEnable(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN, true);

	return 0;
}
