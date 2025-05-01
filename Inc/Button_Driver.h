#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "InterruptControl.h"
#include <stdbool.h>

#define USER_BUTTON_PORT_VAL	GPIOA
#define USER_BUTTON_PIN_NUM		0
#define BUTTON_IS_PRESSED		1
#define BUTTON_IS_NOT_PRESSED	0

void ButtonInit();
void ButtonInit_InterruptMode();
bool ButtonIsPressed();
#endif /* BUTTON_DRIVER_H_ */