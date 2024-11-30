/*
 * HDW_ENCODER.c
 *
 *  Created on: Mar 7, 2024
 *      Author: Marcus Lechner
 */


#include "HDW_ENCODER.h"
#include "APP_DISPLAY.h"

extern TIM_HandleTypeDef htim2;
HDW_ENCODER_Status encoderStatus;
static uint16_t newCount;
static uint16_t prevCount;
uint32_t elapsed_time;
uint8_t count = 0;
uint8_t count_counter = 1;

static HDW_ENCODER_Status HDW_ENCODER_Get_Status();

void HDW_ENCODER_Init(void)
{
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
  elapsed_time = HAL_GetTick();
}

void HDW_ENCODER_process(void)
{
	//elapsed_time_custom_WDG;
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) != GPIO_PIN_SET) //Encoder button pressed
	{
		if (HAL_GetTick() - elapsed_time > 200)  //MUST BE DEBOUNCED
		{
			elapsed_time = HAL_GetTick();
			APP_DISPLAY_increment_page_number();
			APP_DISPLAY_refresh_screen();

		}
	}

	encoderStatus = HDW_ENCODER_Get_Status(); // get the status of the encoder
	switch(encoderStatus) //has the encoder turned clockwise or anti-clockwise
	{
		case Decremented:
			APP_DISPLAY_decrement_tab_number();
			APP_DISPLAY_refresh_screen();
			break;
		case Incremented:
			APP_DISPLAY_increment_tab_number();
			APP_DISPLAY_refresh_screen();
			break;
		case Neutral:
			// don't do anything
			break;
	}
}

uint16_t HDW_ENCODER_Read()
{
  uint16_t val = __HAL_TIM_GET_COUNTER(&htim2);
  return val >> 1;
}

static HDW_ENCODER_Status HDW_ENCODER_Get_Status(void)
{
  newCount = HDW_ENCODER_Read();
  if (newCount != prevCount) {
    if (newCount < prevCount) {
      prevCount = newCount;
      return Incremented;
    } else {
      prevCount = newCount;
      return Decremented;
    }
  }
  return Neutral;
}

/**
  * @brief TIM2 Initialization Function (Encoder Mode)
  * @param None
  * @retval None
  */
void HDW_ENCODER_Config(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC2Filter = 0;

  HAL_TIM_Encoder_Init(&htim2, &sConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
}

uint8_t HDW_ENCODER_get_count(void)
{
	return count;
}
