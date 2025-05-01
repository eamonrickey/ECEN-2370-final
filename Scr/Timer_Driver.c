void Timer_Init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = (SystemCoreClock / 10000) - 1;  // 10kHz counter clock
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 10000 - 1;  // 10,000 counts @ 10kHz = 1 second
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim2);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void Timer_Start()
{
	HAL_TIM_Base_Start_IT(&htim2);
}

void Timer_Stop()
{
	HAL_TIM_Base_Stop_IT(&htim2);
}

void Timer_Reset()
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	game_time = 0;
}

uint32_t Timer_GetGameTime()
{
	return game_time;
}
void TIM2_IRQHandler()
{
	IRQ_disableInterrupt(TIM2_IRQn);
	IRQ_clearPendingInterrupt(TIM2_IRQn);
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	game_time++;
	IRQ_enableInterrupt(TIM2_IRQn);
}
