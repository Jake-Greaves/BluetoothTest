/*
 * LibIIO_Utility.c - Utility functions for LibIIO_Interface.c
 *
 * Author: Jake Greaves
 */

#include "Communications.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


int serial_write_data(char *data, int len)
{
        //write all data to UART
	Uart_Write(data);

	return 0;
}

int serial_read_data(char *buf, int len) {

	int ret;
        
        //read data of length len from UART
	ret = Uart_Read(buf, len);

	return ret;
}

int serial_blocking_read_next(void *buf) {

	int ret;
        
        //read next char from UART
        ret = Uart_Read(buf, 1);

        return ret;
}

int serial_read_line(char *buf, int len) {

	int i;
	bool found = false;
        
        //read until either buffer is filled or \n is found
	for (i = 0; i < len - 1; i++) {
		serial_blocking_read_next(&buf[i]);
                
		if (buf[i] != '\n')
			found = true;
              
		else if (found)
			break;
	}
        
	if (!found || i == len - 1)
		return 0;

        //return pos of \n
	return i + 1;
}

int serial_read_integer(int *val) {

	unsigned int i;
	char buf[128], *ptr = NULL, *end;
	int ret = 0;
	int value;
        
	do {
                //read line
		ret = serial_read_line(buf, sizeof(buf));
                
                //find \n and set ptr to start of number
		for (i = 0; i < (unsigned int) ret; i++) {
			if (buf[i] != '\n') {
				if (!ptr)
					ptr = &buf[i];
			} else if (!!ptr) {
				break;
			}
		}
	} while (!ptr);
        
        //end buffer after integer
	buf[i] = '\0';

        //parse string to integer
	value = (int) strtol(ptr, &end, 10);
	if (ptr == end)
		return 1;

	*val = value;
	return 0;
}

int execute_command(char *cmd) {

	int resp, ret = 0;
        
        //send command to device
	ret = serial_write_data(cmd, strlen(cmd));
        if(ret < 0)
          return ret;
        
        //read response
	ret = serial_read_integer(&resp);

        //return integer
	return resp;
}
