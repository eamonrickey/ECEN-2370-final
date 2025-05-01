#include "InterruptControl.h"

void IRQ_enableInterrupt(uint8_t IRQ_num)
{
	if(IRQ_num < 32)
	{
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		//*NVIC_ISER0_ADDR |= (1 << IRQ_num);
	}

}
void IRQ_disableInterrupt(uint8_t IRQ_num)
{
	if(IRQ_num < 32)
	{
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		//*NVIC_ICER0_ADDR |= (1 << IRQ_num);
	}

}
void IRQ_clearPendingInterrupt(uint8_t IRQ_num)
{
	if(IRQ_num < 32)
	{
		HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
		//*NVIC_ICPR0_ADDR |= (1 << IRQ_num);
	}

}
void IRQ_setPendingInterrupt(uint8_t IRQ_num)
{
	if(IRQ_num < 32)
	{
		HAL_NVIC_SetPendingIRQ(EXTI0_IRQn);
		//*NVIC_ISPR0_ADDR |= (1 << IRQ_num);
	}

}
void EXTI_clearpPendingBit(uint8_t reg_pin)
{
	EXTI->PR |= (1 << reg_pin);
}
