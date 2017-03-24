
#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_

#include "string.h"

#define TOTAL_COMMAND_LENGTH 10

static char* READ_DATA_TAG = "s"; //data is part of the data stream
static char* PRINT_XML_TAG = "x"; //data relates to the xml schema of the sense1000
static char* READ_ATTR_TAG = "r"; //data is a response to the read attr tag

int App_Read_Command();

#endif // _APP_INTERFACE_H_