/*
 * HDW_LED.c
 *
 *  Created on: Mar 7, 2024
 *      Author: Marcus Lechner
 *      Based on Example code provided by Nico Korn
 */

// Include ********************************************************************
#include <HDW_LED.h>
#include <HDW_BME280.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

// Private define *************************************************************
#define PIXEL_BIT_SIZE           ( 24u )
#define WS2812B_PWM_TRANSFER_TIME            ( 20u ) // 20 ticks @ 16 MHz equals 1250 ns
#define WS2812B_1                60u//( 15u | 0x8000 ) // 15 ticks @ 16 MHz equals 937.5 ns, the MSB defines the PWM polarity during the sequence
#define WS2812B_0                30u//( 6u | 0x8000 )  // 6 ticks @ 16 MHz equals 375 ns, the MSB defines the PWM polarity during the sequence
#define WS2812B_PINOUT                  NRF_GPIO_PIN_MAP(WS2812B_PORT,WS2812B_PIN)
#define WS2812B_RESET_LOW        50

extern TIM_HandleTypeDef htim3;
static uint32_t HDW_LED_elapsed_time;
static uint8_t LED_Data[PIXEL_COUNT][4];
static uint16_t WS2812B_Buffer[(PIXEL_BIT_SIZE*PIXEL_COUNT)+WS2812B_RESET_LOW];
static WS2812B_StatusTypeDef WS2812B_State = WS2812B_RESET;
static HDW_LED_ANIMATION HDW_LED_current_annimation = HDW_LED_ALL_OFF;
static uint8_t HDW_LED_max_brightness = 200;

// Private types     **********************************************************
uint8_t HDW_LED_buffer_offset[3] =
{
      HDW_LED_RED_OFFSET,
      HDW_LED_GREEN_OFFSET,
      HDW_LED_BLUE_OFFSET,
};



static void HDW_LED_set_pixel (int LEDnum, int Red, int Green, int Blue);
static void HDW_LED_set_buffer (int Red, int Green, int Blue);
static void HDW_LED_send_buffer (void);
static void HDW_LED_clear_buffer (void);


static void HDW_LED_set_pixel (int LEDnum, int Red, int Green, int Blue)
{
	if(LEDnum < PIXEL_COUNT)
	{
		LED_Data[LEDnum][0] = LEDnum;
		LED_Data[LEDnum][1] = Green;
		LED_Data[LEDnum][2] = Red;
		LED_Data[LEDnum][3] = Blue;
	}
}

static void HDW_LED_set_buffer (int Red, int Green, int Blue)
{
	for (int i=0; i<PIXEL_COUNT; i++)
	{
		LED_Data[i][0] = i;
		LED_Data[i][1] = Green;
		LED_Data[i][2] = Red;
		LED_Data[i][3] = Blue;
	}
}



static void HDW_LED_send_buffer (void)
{
	uint32_t indx=0;
	uint32_t color;

	for (int i= 0; i<PIXEL_COUNT; i++)
	{
#if USE_BRIGHTNESS
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
#else
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
#endif

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				WS2812B_Buffer[indx] = WS2812B_1;  // 2/3 of 90
			}
			else WS2812B_Buffer[indx] = WS2812B_0;  // 1/3 of 90
			{
				indx++;
			}
		}
	}

	for (int i=0; i<50; i++)
	{
		WS2812B_Buffer[indx] = 0;
		indx++;
	}

	WS2812B_State = WS2812B_BUSY;
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)WS2812B_Buffer, indx);
	while(WS2812B_State != WS2812B_READY );

}

void HDW_LED_clear_buffer (void)
{
	for (int i=0; i<PIXEL_COUNT; i++)
	{
		LED_Data[i][0] = i;
		LED_Data[i][1] = 0;
		LED_Data[i][2] = 0;
		LED_Data[i][3] = 0;
	}
}

// Private variables **********************************************************


// Private function prototypes ************************************************
static void HDW_LED_start_up(void);
static void HDW_LED_glow();
static void HDW_LED_single_with_tail_incrementing();
static void HDW_LED_single_with_tail_decrementing();
static void HDW_LED_twinkle();
static void HDW_LED_dancing();
static void HDW_LED_rainbow_effect_left();
static void HDW_LED_all_off();

static const HDW_LED_annimation_list_t HDW_LED_annimation_functions[] =
{
		{HDW_LED_START_UP,		HDW_LED_start_up},
		{HDW_LED_ALL_OFF, 		HDW_LED_all_off},
		{HDW_LED_RAINBOW, 		HDW_LED_rainbow_effect_left},
		{HDW_LED_GLOW, 			HDW_LED_glow},
		{HDW_LED_DANCING, 		HDW_LED_dancing},
		{HDW_LED_TWINKLE, 		HDW_LED_twinkle},
		{HDW_LED_TAIL_INCREASE,	HDW_LED_single_with_tail_incrementing},
		{HDW_LED_TAIL_DECREASE, HDW_LED_single_with_tail_decrementing},
		{HDW_LED_ANIMATION_NUM, 0x00},

};

// Global variables ***********************************************************

// Functions ******************************************************************


void HDW_LED_Config( void ) //can use WS2812B_StatusTypeDef and return status to the function called in main and use the error handlers there
{

	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};

	  /* USER CODE BEGIN TIM3_Init 1 */

	  /* USER CODE END TIM3_Init 1 */
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = 0;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim3.Init.Period = 90-1;
	  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	  {
//	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
//	    Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
//	    Error_Handler();
	  }
	  /* USER CODE BEGIN TIM3_Init 2 */

	  /* USER CODE END TIM3_Init 2 */
//	  HAL_TIM_MspPostInit(&htim3); no idea why this cant be called
}
void HDW_LED_Init( void )
{
	HDW_LED_elapsed_time = HAL_GetTick();
	    for(uint8_t i=0; i<WS2812B_RESET_LOW; i++)
	    {
	      WS2812B_Buffer[PIXEL_COUNT*PIXEL_BIT_SIZE+i] = 0x8000;
	    }
	WS2812B_State = WS2812B_READY;
	HDW_LED_set_annimation(HDW_LED_START_UP);
}

static void HDW_LED_get_colour(enum HDW_LED_ON_COLOUR desired_colour, uint8_t desired_brightness, uint8_t rgb_colours[])
{
	float magnitude = (float)desired_brightness  / 255; //spits out a percentage
	switch (desired_colour)
			{
			case HDW_LED_WHITE:
	                        rgb_colours[0] = 255 * magnitude;
	                        rgb_colours[1] = 255 * magnitude;
	                        rgb_colours[2] = 255 * magnitude;
				break;
			case HDW_LED_RED:
							rgb_colours[0] = 255 * magnitude;
	                        rgb_colours[1] = 0;
	                        rgb_colours[2] = 0;
				break;
			case HDW_LED_GREEN:
							rgb_colours[0] = 0;
	                        rgb_colours[1] = 255 * magnitude;
	                        rgb_colours[2] = 0;
				break;
			case HDW_LED_BLUE:
							rgb_colours[0] = 0;
	                        rgb_colours[1] = 0;
	                        rgb_colours[2] = 255 * magnitude;
				break;
			case HDW_LED_YELLOW:
	                        rgb_colours[0] = 255 * magnitude;
	                        rgb_colours[1] = 255 * magnitude;
	                        rgb_colours[2] = 0;
				break;
			case HDW_LED_MAGENTA:
							rgb_colours[0] = 255 * magnitude;
							rgb_colours[1] = 0;
							rgb_colours[2] = 144 * magnitude;
				break;
			case HDW_LED_CYAN:
							rgb_colours[0] = 0;
							rgb_colours[1] = 255 * magnitude;
							rgb_colours[2] = 255 * magnitude;
				break;
			case HDW_LED_ORANGE:
							rgb_colours[0] = 255 * magnitude;
							rgb_colours[1] = 0x80 * magnitude;
							rgb_colours[2] = 0 * magnitude;
				break;
			case HDW_LED_PINK:
							rgb_colours[0] = 255 * magnitude;
							rgb_colours[1] = 0 * magnitude;
							rgb_colours[2] = 0x80 * magnitude;
				break;
			case HDW_LED_OFF: //leave cleared do nothing
	                        rgb_colours[0] = 0;
	                        rgb_colours[1] = 0;
	                        rgb_colours[2] = 0;
				break;
			default:
				break;
			}; //end of switch
}

//
uint16_t effStep = 0;

static void HDW_LED_glow()
{
	//TODO: brightness should not effect glow speed

//	get colour
//	uint8_t colour = HDW_LED_RED; //ROYGBIV
	uint8_t max_glow_brightness = HDW_LED_max_brightness;
	uint8_t min_glow_brightness = HDW_LED_max_brightness / 12;
	uint8_t step_size = 1;
//	calculate what rgb should be at max brightness
	uint8_t rgb_colours[3];
	static uint8_t glow_step = 255;
	static bool is_dimming = true;

	if(is_dimming)
	{
		if (glow_step <= min_glow_brightness)
		{
			is_dimming = false;
			glow_step = min_glow_brightness;//glow_step + step_size;
		}
		else
		{
			glow_step = glow_step - step_size;
		}
	}
	else
	{
		if (glow_step >= max_glow_brightness)
		{
			is_dimming = true;
			glow_step = max_glow_brightness;//glow_step - step_size;
		}
		else
		{
			glow_step = glow_step + step_size;
		}
	}

	float magnitude = (float)glow_step  / 255; //spits out a percentage

	float temperature = HDW_BME280_get_temperature();
	uint8_t temp_min = 30;
	uint8_t temp_max = 38;
	float offset_temp = 0;

	uint8_t colour_step = 0;

	if(temperature > temp_max)
	{
		colour_step = 255;
	}
	else if (temperature <= temp_min)
	{
		colour_step = 0;
	}
	else
	{	//map temp to 0 - 255
		offset_temp = temperature - temp_min;
		colour_step = 255 - ((((float)(temp_max - temp_min) - offset_temp)/(float)(temp_max - temp_min))*255);
	}

	/* TESTING ALGO
	offset_temp = 0.2;
	colour_step = 255 - ((((float)(temp_max - temp_min) - offset_temp)/(float)(temp_max - temp_min))*255);

	offset_temp = 7.9;
	colour_step = 255 - ((((float)(temp_max - temp_min) - offset_temp)/(float)(temp_max - temp_min))*255);

	 **/


	if (colour_step <= 127) //0x00,0xFF,0x00 ->  0xFF, 0xFF 0x00
	{
		rgb_colours[0] = colour_step*2  * magnitude;
		rgb_colours[1] = 0xFF * magnitude;
		rgb_colours[2] = 0x00;
	}
	else //glow_step 128 - > 255  0xFF, 0xFF, 0x00 - >  0xFF, 0x00, 0x00
	{
		rgb_colours[0] = 0xFF * magnitude;
		rgb_colours[1] = (0xFF - (colour_step - 128) * 2) * magnitude;
		rgb_colours[2] = 0x00;
	}



	HDW_LED_set_buffer(rgb_colours[0], rgb_colours[1], rgb_colours[2]);

	HDW_LED_send_buffer();

}

static void HDW_LED_single_with_tail_incrementing()
{
	uint8_t tail_length = 32;
	static uint8_t colour = HDW_LED_GREEN; //ROYGBIV
	uint8_t max_brightness = HDW_LED_max_brightness;
	uint8_t min_brightness = 0;
	uint8_t brightness_step = (max_brightness - min_brightness)/tail_length;
	static uint8_t step_number = 0;
	uint8_t rgb_colours[3];
	uint8_t brightness = 0;

	if(HAL_GetTick() - HDW_LED_elapsed_time > 15) //TODO: change to use timer
	{
		HDW_LED_elapsed_time = HAL_GetTick();
		HDW_LED_clear_buffer();
		for(uint8_t i = 0; i < tail_length; i++)
		{//for 6 - 0
			brightness = max_brightness - (brightness_step * i);
			HDW_LED_get_colour( colour, brightness ,  &rgb_colours[0]);
			HDW_LED_set_pixel ((PIXEL_COUNT + step_number - i) % PIXEL_COUNT, rgb_colours[0], rgb_colours[1], rgb_colours[2]);
		}
		step_number++; //if not bigger than number of pixels
		if(step_number > PIXEL_COUNT)
		{
			step_number = 0;
			colour = (colour + 1) % HDW_LED_WHITE;
		}
		HDW_LED_send_buffer();
	}
}

static void HDW_LED_single_with_tail_decrementing()
{
	uint8_t tail_length = 22;
	static uint8_t colour = HDW_LED_GREEN; //ROYGBIV
	uint8_t max_brightness = HDW_LED_max_brightness;
	uint8_t min_brightness = 0;
	uint8_t brightness_step = (max_brightness - min_brightness)/tail_length;
	static uint8_t step_number = PIXEL_COUNT;
	uint8_t rgb_colours[3];
	uint8_t brightness = 0;

	if(HAL_GetTick() - HDW_LED_elapsed_time > 15) //TODO: change to use timer
	{
		HDW_LED_elapsed_time = HAL_GetTick();
		HDW_LED_clear_buffer();
		for(uint8_t i = 0; i < tail_length; i++)
		{//for 6 - 0
			brightness = max_brightness - (brightness_step * i);
			HDW_LED_get_colour( colour, brightness ,  &rgb_colours[0]);
			HDW_LED_set_pixel ((PIXEL_COUNT - step_number + i) % PIXEL_COUNT, rgb_colours[0], rgb_colours[1], rgb_colours[2]);
			HDW_LED_set_pixel (((PIXEL_COUNT) - step_number + i + PIXEL_COUNT/2) % PIXEL_COUNT, rgb_colours[0], rgb_colours[1], rgb_colours[2]);
		}
		step_number++; //if not bigger than number of pixels
		if(step_number > PIXEL_COUNT )
		{
			step_number = 0;
			colour = (colour + 1) % HDW_LED_WHITE;
		}
		HDW_LED_send_buffer();
	}
}
static void HDW_LED_twinkle()
{
	uint8_t colour_sequence_start[HDW_LED_ON_STATE_NUM];
	uint8_t brightness = HDW_LED_max_brightness;
	uint8_t sequence_length = 32;
	uint8_t step_overlap = sequence_length/2;
	uint8_t rgb_colours[3];
	uint8_t rollover_steps = 0;
	uint8_t number_of_colours = 8;
	uint8_t annimation_steps = (sequence_length - step_overlap) * number_of_colours;
	uint8_t num_reps = (PIXEL_COUNT/number_of_colours);
	uint8_t step_in_sequence = 0;
	uint16_t intensity = 0;
	static uint8_t step_counter = 0;

	if(HAL_GetTick() - HDW_LED_elapsed_time > 50)
	{
		HDW_LED_elapsed_time = HAL_GetTick();
		HDW_LED_clear_buffer();
		colour_sequence_start[HDW_LED_RED] = 0;
		colour_sequence_start[HDW_LED_GREEN] = colour_sequence_start[HDW_LED_RED] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_MAGENTA] = colour_sequence_start[HDW_LED_GREEN] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_ORANGE] = colour_sequence_start[HDW_LED_MAGENTA] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_CYAN] = colour_sequence_start[HDW_LED_ORANGE] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_PINK] = colour_sequence_start[HDW_LED_CYAN] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_YELLOW] = colour_sequence_start[HDW_LED_PINK] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_BLUE] = colour_sequence_start[HDW_LED_YELLOW] + sequence_length - step_overlap;
		rollover_steps = colour_sequence_start[HDW_LED_BLUE] + sequence_length - annimation_steps;

		//could all be a for loop
		for(uint8_t i = HDW_LED_RED; i < HDW_LED_WHITE; i++)
		{
			if((colour_sequence_start[i] <= step_counter) && (step_counter < (colour_sequence_start[i] + sequence_length)))
			{
				step_in_sequence =  step_counter - colour_sequence_start[i];

				if(step_in_sequence < sequence_length/2)
				{
					intensity = ((step_in_sequence+1) * brightness) / (sequence_length/2);
				}
				else if(step_in_sequence >= sequence_length/2)
				{
					intensity = ((sequence_length - step_in_sequence)* brightness) / (sequence_length/2);
				}
				HDW_LED_get_colour( i, intensity ,  &rgb_colours[0]);
				for(uint8_t j = 0; j <= num_reps; j++)
				{
					HDW_LED_set_pixel (i + ((number_of_colours) * j), rgb_colours[0], rgb_colours[1], rgb_colours[2]);
				}
			}
		}
		if(0<= step_counter && step_counter < rollover_steps)
		{
			step_in_sequence =  step_counter + step_overlap;
			if(step_in_sequence < sequence_length/2)
			{
				intensity = ((step_in_sequence+1) * brightness) / (sequence_length/2);
			}
			else if(step_in_sequence >= sequence_length/2)
			{
				intensity = ((sequence_length - step_in_sequence)* brightness) / (sequence_length/2);
			}
			HDW_LED_get_colour( HDW_LED_BLUE, intensity ,  &rgb_colours[0]);
			for(uint8_t j = 0; j <= num_reps; j++)
			{
				HDW_LED_set_pixel (HDW_LED_BLUE + ((number_of_colours) * j), rgb_colours[0], rgb_colours[1], rgb_colours[2]);
			}
		}
		step_counter = (step_counter + 1) % annimation_steps;
		HDW_LED_send_buffer();
	}
}

static void HDW_LED_dancing()
{
	uint8_t colour_sequence_start[HDW_LED_ON_STATE_NUM];
	uint8_t sequence_length = 8;
	uint8_t step_overlap = 4;
	uint8_t rgb_colours[3];
	uint8_t brightness = HDW_LED_max_brightness;
	uint8_t rollover_steps = 0;
	uint8_t number_of_colours = 8;
	uint8_t annimation_steps = (sequence_length - step_overlap) * number_of_colours;
	uint8_t num_reps = (PIXEL_COUNT/number_of_colours);
	static uint8_t steps = 0;

	if(HAL_GetTick() - HDW_LED_elapsed_time > 100)
	{
		HDW_LED_elapsed_time = HAL_GetTick();
		HDW_LED_clear_buffer();
		colour_sequence_start[HDW_LED_RED] = 0;
		colour_sequence_start[HDW_LED_GREEN] = colour_sequence_start[HDW_LED_RED] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_MAGENTA] = colour_sequence_start[HDW_LED_GREEN] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_ORANGE] = colour_sequence_start[HDW_LED_MAGENTA] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_CYAN] = colour_sequence_start[HDW_LED_ORANGE] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_PINK] = colour_sequence_start[HDW_LED_CYAN] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_YELLOW] = colour_sequence_start[HDW_LED_PINK] + sequence_length - step_overlap;
		colour_sequence_start[HDW_LED_BLUE] = colour_sequence_start[HDW_LED_YELLOW] + sequence_length - step_overlap;
		rollover_steps = colour_sequence_start[HDW_LED_BLUE] + sequence_length - annimation_steps;

		for(uint8_t i = HDW_LED_RED; i < HDW_LED_WHITE; i++)
		{
			if((colour_sequence_start[i] <= steps) && (steps < (colour_sequence_start[i] + sequence_length)))
			{
				HDW_LED_get_colour( i, brightness ,  &rgb_colours[0]);
				for(uint8_t j = 0; j <= num_reps; j++)
				{
					HDW_LED_set_pixel (i + ((number_of_colours) * j), rgb_colours[0], rgb_colours[1], rgb_colours[2]);
				}
			}
		}
		if(0<= steps && steps < rollover_steps)
		{
			HDW_LED_get_colour( HDW_LED_BLUE, brightness ,  &rgb_colours[0]);
			for(uint8_t j = 0; j <= num_reps; j++)
			{
				HDW_LED_set_pixel (HDW_LED_BLUE + ((number_of_colours) * j), rgb_colours[0], rgb_colours[1], rgb_colours[2]);
			}
		}
		steps = (steps + 1) % annimation_steps;
		HDW_LED_send_buffer();
	}
}

static void HDW_LED_rainbow_effect_left()
{
	// 1400mA @ 200 amplitude
	// 1700mA @ 255 amplitude

	if(HAL_GetTick() - HDW_LED_elapsed_time > 75)
	{
		HDW_LED_elapsed_time = HAL_GetTick();
		uint8_t num_LEDS = 30;
		uint8_t red_magnitude = HDW_LED_max_brightness;
		uint8_t green_magnitude = HDW_LED_max_brightness;
		uint8_t blue_magnitude = HDW_LED_max_brightness;
		uint8_t steps = 32;
		float steps_per_colour = steps/3;
		float steps_per_led = steps / num_LEDS;
		float factor1, factor2;
		uint16_t ind;
		for(uint16_t j=0;j<PIXEL_COUNT;j++)
		{
		ind = effStep + j * steps_per_led;
		switch((int)((ind % steps) / steps_per_colour))
			{
			case 0:
			  factor1 = 1.0 - ((float)(ind % steps - 0 * steps_per_colour) / steps_per_colour);
			  factor2 = (float)((int)(ind - 0) % steps) / steps_per_colour;
			  HDW_LED_set_pixel(j, red_magnitude * factor1 + 0 * factor2, 0 * factor1 + green_magnitude * factor2, 0 * factor1 + 0 * factor2);
			  break;
			case 1:
			  factor1 = 1.0 - ((float)(ind % steps - 1 * steps_per_colour) / steps_per_colour);
			  factor2 = (float)((int)(ind - steps_per_colour) % steps) / steps_per_colour;
			  HDW_LED_set_pixel(j, 0 * factor1 + 0 * factor2, green_magnitude * factor1 + 0 * factor2, 0 * factor1 + blue_magnitude * factor2);
			  break;
			case 2:
			  factor1 = 1.0 - ((float)(ind % steps - 2 * steps_per_colour) / steps_per_colour);
			  factor2 = (float)((int)(ind - (steps_per_colour * 2)) % steps) / steps_per_colour;
			  HDW_LED_set_pixel(j, 0 * factor1 + red_magnitude * factor2, 0 * factor1 + 0 * factor2, blue_magnitude * factor1 + 0 * factor2);
			  break;

			default:
				break;
			}
		}
		HDW_LED_send_buffer();

		if(effStep >= steps - 1)
		{
			effStep=0;
		}
		else
		{
			effStep++;
		}
	}
}
static void HDW_LED_start_up(void)
{
	static uint8_t colour = HDW_LED_GREEN; //ROYGBIV
	static uint8_t step_number = 0;
	uint8_t rgb_colours[3];
	uint8_t brightness = 255;

	if(HAL_GetTick() - HDW_LED_elapsed_time > 22) //TODO: change to use timer
	{
		HDW_LED_elapsed_time = HAL_GetTick();
		HDW_LED_clear_buffer();



		for(uint8_t i = 0; i < step_number; i++)
		{//for 6 - 0
//			brightness = max_brightness - (brightness_step * i);
			HDW_LED_get_colour( colour, brightness ,  &rgb_colours[0]);
			HDW_LED_set_pixel ((PIXEL_COUNT + step_number - i) % PIXEL_COUNT, rgb_colours[0], rgb_colours[1], rgb_colours[2]);
			HDW_LED_set_pixel ((PIXEL_COUNT - step_number + i) % PIXEL_COUNT, rgb_colours[0], rgb_colours[1], rgb_colours[2]);

		}
		step_number++; //if not bigger than number of pixels
		if(step_number > PIXEL_COUNT) //technically this allows the pixel buffer to overwrite itself but its a nice delay
		{
			HDW_LED_set_annimation(HDW_LED_ALL_OFF);
		}


		HDW_LED_send_buffer();
	}
}


static void HDW_LED_all_off()
{
	HDW_LED_clear_buffer();
	HDW_LED_send_buffer();
}

void HDW_LED_set_annimation(HDW_LED_ANIMATION new_annimation)
{
	if(new_annimation < HDW_LED_ANIMATION_NUM)
	{
		HDW_LED_current_annimation = new_annimation;
	}
	else
	{
		HDW_LED_current_annimation = HDW_LED_ALL_OFF;
	}
}

HDW_LED_ANIMATION HDW_LED_get_annimation()
{
	return(HDW_LED_current_annimation);
}

void HDW_LED_process(void)
{
	(HDW_LED_annimation_functions[HDW_LED_current_annimation].function)();
}

void HDW_LED_set_brightness(uint8_t brightness)
{
	HDW_LED_max_brightness = brightness;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
	WS2812B_State = WS2812B_READY;
}
