/*
 * App_Interface - Main interface to app
 *
 * Author: Jake Greaves
 */

#include "string.h"
#include "stdlib.h"
#include "BLE_Module.h"
#include "App_Interface.h"
#include "Placeholder_TempInterface.h"
#include "LibIIO_Interface.h"
#include "Communications.h"

extern bool deep_Sleep;

int App_Parse_Deep_Sleep(char *str)
{
        char *ptr;
        int pwr_Mode;
        
        pwr_Mode = strtol(str, &ptr, 10);
        
        if(pwr_Mode == 0)
          deep_Sleep = false;
        else if(pwr_Mode == 1)
          deep_Sleep = true;
        else
          return 1;
        
        return 0;
}

int App_Parse_Print_Xml(void)
{
        char *BLE_Payload;//*BLE_Payload;
        char *buf = "";
        
	//Read Sensor data
	LibIIO_Print(buf);
  
        BLE_Payload = (char*)malloc((strlen(buf)+strlen(PRINT_XML_TAG) + 1)*sizeof(char));
        
        strcpy(BLE_Payload, PRINT_XML_TAG);
        strcat(BLE_Payload, buf);
        strcat(BLE_Payload, "\n");
        
	//Write sensor value to BLE
	Ble_Uart_Write(BLE_Payload);
        
        free(BLE_Payload);

	return 0;
}

int App_Parse_Read_Data(void)
{
    int ret = 0;
    char *Data_Buffer; //data Buffer
    char BLE_TxPayload[64] = "";
    
    //data returned from sense1000 is 2 floats
    //allocate this memory space
    Data_Buffer = (char*)malloc(2*sizeof(float));
      
    //Read Temp-Module sensor value
    ret = LibIIO_ReadBuf(Data_Buffer);
    
    //if data is valid
    if(ret >= 0 && Data_Buffer != NULL)
    {
      //convert data to string for App
      snprintf(BLE_TxPayload, 64,"sCJT:%f;TCT:%f;\n\r", ((float*)Data_Buffer)[0], ((float*)Data_Buffer)[1]);
      
      //send data to App
      Ble_Uart_Write(BLE_TxPayload);
    }
    
    //free buffer
    free(Data_Buffer);
    
    return 0;
}

int App_Parse_Read_Write_Attr(char* str, bool write)
{
        int ret;
        char *device, *channel, *attr, *ptr, *writeVal;
        bool is_channel = false, output = false, debug = false;
        char BLE_Payload[64];//BLE_Payload;
        char value[32];
        //search for following space. variable are denoted by spaces
        ptr = strchr(str, ' ');
	if (!ptr) {
                free(str);
		return 1;
        }
        
        //end string
	*ptr = '\0';
        //previous string up to the \0 is the device
	device = str;
        //str now with device removed
	str = ptr + 1;
        
        //check nature of read. INPUT is only supported on temp-module
        if (!strncmp(str, "INPUT ", sizeof("INPUT ") - 1)) {
		is_channel = true;
		str += sizeof("INPUT ") - 1; //remove INPUT from str
	} else if (!strncmp(str, "OUTPUT ", sizeof("OUTPUT ") - 1)) {
		is_channel = true;
		output = true;
		str += sizeof("OUTPUT ") - 1;
	} else if (!strncmp(str, "DEBUG ", sizeof("DEBUG ") - 1)) {
		debug = true;
		str += sizeof("DEBUG ") - 1;
	}
        
        //should be channel
        if (is_channel) {
		ptr = strchr(str, ' ');
		if (!ptr) {
                        free(str);
			return 1;
                }
                
		*ptr = '\0';
		channel = str;
		str = ptr + 1;
	} else {
		channel = NULL;
	}

	
        
        if (write) {
                ptr = strchr(str, ' ');
		*ptr = '\0';
                attr = str;
		str = ptr + 1;
	} else {
                attr = str;
		ret = LibIIO_ReadAttr(device, channel, attr, output, value);
                
                if(ret < 0) {
                        free(str);
			return ret;
                }
        
                strcpy(BLE_Payload, READ_ATTR_TAG);
                strcat(BLE_Payload, device);
                strcat(BLE_Payload, " ");
                if (is_channel) {
                    strcat(BLE_Payload, "INPUT ");
                    strcat(BLE_Payload, channel);
                    strcat(BLE_Payload, " ");
                }
                strcat(BLE_Payload, attr);
                strcat(BLE_Payload, " ");
                strcat(BLE_Payload, value);
                strcat(BLE_Payload, "\r");
                
                //Write sensor value to BLE
                Ble_Uart_Write(BLE_Payload);
                
                free(str);
		return 0;
	}

	writeVal = str;

	LibIIO_WriteAttr(device, channel, attr, output, writeVal);
        free(str);
	return ret;
}

int App_Parse_String(char *command)
{
        //DEEP_SLEEP command
        if (!strncmp(command, "DEEP_SLEEP ", sizeof("DEEP_SLEP ") -1))
		return App_Parse_Deep_Sleep(command + sizeof("DEEP_SLEP ") - 1);
        
        //DEEP_SLEP command
        if (!strncmp(command, "READ_DATA", sizeof("READ_DATA ") -1))
		return App_Parse_Read_Data();
        
        //PRINT_XML command
        if (!strncmp(command, "PRINT_XML", sizeof("PRINT_XML") -1))
		return App_Parse_Print_Xml();
        //READ_ATTR command
        if (!strncmp(command, "READ_ATTR ", sizeof("READ_ATTR") -1))
		return App_Parse_Read_Write_Attr(command + sizeof("READ_ATTR ") - 1, false);
        //WRITE_ATTR command
        if (!strncmp(command, "WRITE_ATTR ", sizeof("WRITE_ATTR") -1))
		return App_Parse_Read_Write_Attr(command + sizeof("WRITE_ATTR ") - 1, true);
        
	//Add commands here
	//Template
	//if (!strncmp(str, "COMMAND", sizeof("COMMAND") -1))
	//		return App_Parse_COMMAND(str + sizeof("COMMAND") - 1);
	//App_Parse_COMMAND also would need added to handle the action required

	return 1;
}

int App_Read_Command() {
  
	int ret;
        char buf[128];
        
        Ble_Uart_Read_Line(buf, 128);
          
	//Parse command to carry out command
	ret = App_Parse_String(buf);

	return ret;
}
