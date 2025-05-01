#include "Button_Driver.h"

void ButtonInit()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct_Button = {0};
	GPIO_InitStruct_Button.Pin = GPIO_PIN_0;
	GPIO_InitStruct_Button.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct_Button.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct_Button.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_Button);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

bool ButtonIsPressed()
{
	GPIO_PinState button_val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	if(button_val == BUTTON_IS_PRESSED)
	{
		return BUTTON_IS_PRESSED;
	}
	else
	{
		return BUTTON_IS_NOT_PRESSED;
	}
}