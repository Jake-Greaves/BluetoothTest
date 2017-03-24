#ifndef _TIMER_INTERFACE_H_
#define _TIMER_INTERFACE_H_

#include <services/tmr/adi_tmr.h>

#define TIMER_DEVICE_0 0

typedef enum {
              SLEEP_TIMER = 0,
              UART_READ_TIMER
              } Timer_Enum;

char Timer_Count(uint32_t nDeviceNumber);
char Handle_Timeout(void);
void GPTimer0Callback(void *pCBParam, uint32_t Event, void *pArg);
void GPTimer1Callback(void *pCBParam, uint32_t Event, void *pArg);
char Close_Timer(void);
#endif //_TIMER_INTERFACE_H_
