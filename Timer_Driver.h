#ifndef INC_TIMER_DRIVER_H_
#define INC_TIMER_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "InterruptControl.h"

void Timer_Init();
void Timer_Start();
void Timer_Stop();
void Timer_Reset();
uint32_t Timer_GetGameTime();

#endif /* INC_TIMER_DRIVER_H_ */
