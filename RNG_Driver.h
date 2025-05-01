#ifndef INC_RNG_DRIVER_H_
#define INC_RNG_DRIVER_H_

#include "stm32f4xx_hal.h"

void RNG_Init();
void RNG_Get_Number(uint32_t *randomNumber);

#endif /* INC_RNG_DRIVER_H_ */
