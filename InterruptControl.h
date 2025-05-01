#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_

#include "stm32f4xx_hal.h"

#define EXTI0_IRQ_NUMBER	6

void IRQ_enableInterrupt(uint8_t IRQ_num);
void IRQ_disableInterrupt(uint8_t IRQ_num);
void IRQ_clearPendingInterrupt(uint8_t IRQ_num);
void IRQ_setPendingInterrupt(uint8_t IRQ_num);
void EXTI_clearpPendingBit(uint8_t reg_pin);

#endif /* INTERRUPTCONTROL_H_ */
