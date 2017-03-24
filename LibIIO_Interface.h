/*
 * LibIIO_Interface.h
 *
 *  Created on: 25 Feb 2017
 *      Author: jtgre
 */

#ifndef LIBIIO_INTERFACE_H_
#define LIBIIO_INTERFACE_H_

#include <services/gpio/adi_gpio.h>

#define SENSE1000_RESET_PORT 	ADI_GPIO_PORT1
#define SENSE1000_RESET_PIN 	ADI_GPIO_PIN_2

#define SENSE1000_RESET_LENGTH 	15000 //15 seconds approx


extern void Delay_ms(unsigned int mSec);


//Function prototypes

//open LibIIO Device
int LibIIO_Open(void);

//Read Sensor values
int LibIIO_ReadBuf(char Buf[]);

//read an attribute
int LibIIO_ReadAttr(char *dev_id, char *chn, char* attr, bool output, char *retBuf);

//write an attribute
int LibIIO_WriteAttr(char *dev_id, char *chn, char* attr, bool output, char *writeVal);

//read the devices xml schema
int LibIIO_Print(char *xml);

//wait for sense to calibrate and boot
int Sense_Wait_For_Boot(void);

//Sense reset pin high
int Sense_Activate(void);

//Sense reset pin low
int Sense_Deactivate(void);

#endif /* LIBIIO_INTERFACE_H_ */
