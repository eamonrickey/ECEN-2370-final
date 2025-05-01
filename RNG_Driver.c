#include "RNG_Driver.h"

static RNG_HandleTypeDef hrng;

void RNG_Init()
{
	__HAL_RCC_RNG_CLK_ENABLE();
	hrng.Instance = RNG;
	HAL_RNG_Init(&hrng);
}
void RNG_Get_Number(uint32_t *randomNumber)
{
	HAL_RNG_GenerateRandomNumber(&hrng, randomNumber);
}

