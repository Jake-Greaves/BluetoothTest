/*
 * LibIIO_Utility.h
 *
 *  Created on: 25 Feb 2017
 *      Author: jtgre
 */

#ifndef LIBIIO_UTILITY_H_
#define LIBIIO_UTILITY_H_

//Function prototypes

//excecute a libIIO command and read the response
int execute_command(char *cmd);

//read a value returned by the device and parse to an integer
int serial_read_integer(int *val);

//write data array to UART
int serial_write_data(char *data, int len);

//read data to buf array through UART
int serial_read_data(char *buf, int len);

//read next char from UART
int serial_blocking_read_next(char *buf);

//read until the \n char is received
int serial_read_line(char *buf, int len);

#endif /* LIBIIO_UTILITY_H_ */
