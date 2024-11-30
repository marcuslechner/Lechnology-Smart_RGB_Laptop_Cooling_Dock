/*
 * HDW_FAN.c
 *
 *  Created on: Mar 7, 2024
 *      Author: Marcus Lechner
 */


#include "HDW_FAN.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "SYS_TIMER.h"

extern TIM_HandleTypeDef htim1;
static uint8_t HDW_FAN_increment_timer;
static uint8_t HDW_FAN_fan_speed;
static bool HDW_FAN_PID_enabled;


static void HDW_FAN_update_speed(uint8_t percent_fan_speed);
static void HDW_FAN_PID_process(void);


void HDW_FAN_set_speed(uint8_t percent_fan_speed)
{ //set by others
	if(percent_fan_speed > 100)
	{
		HDW_FAN_fan_speed = 100;
	}
	else
	{
		HDW_FAN_fan_speed = percent_fan_speed;
	}
}

uint8_t HDW_FAN_get_speed()
{
	return HDW_FAN_fan_speed;
}

bool HDW_FAN_is_PID_enabled(void)
{
	return(HDW_FAN_PID_enabled);
}

void HDW_FAN_set_PID_enabled(bool set)
{
	HDW_FAN_PID_enabled = set;
}
static void HDW_FAN_update_speed(uint8_t percent_fan_speed)
{
	uint16_t speed_step = 620; //sets the max speed to 62000 ouf 65535
	uint16_t cap_comp_reg_value = 0;
	static uint8_t current_fan_speed = 0;


	if(current_fan_speed < percent_fan_speed)
	{
		current_fan_speed++;
		cap_comp_reg_value = current_fan_speed * speed_step; //calculate speed step
		if(current_fan_speed == 100)
		{
			cap_comp_reg_value = 0xFFFF;//65535
		}
		TIM1->CCR1 = cap_comp_reg_value;
	}
	else if (current_fan_speed > percent_fan_speed)
	{
		current_fan_speed = percent_fan_speed;//no hesitation to drop speed
		cap_comp_reg_value = current_fan_speed * speed_step; //calculate speed step
		TIM1->CCR1 = cap_comp_reg_value;
	}
}

static void HDW_FAN_PID_process(void)
{
	/*****************
	 * PID is in beta testing and will be added to public repo shortly
	const double kp = 1.0;
	const double ki = 1.0;
	const double kd = 1.0;
	****************/

	HDW_FAN_update_speed(HDW_FAN_get_speed());
}

void HDW_FAN_init()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	HDW_FAN_increment_timer = SYS_TIMER_create(TIMER_MODE_REPEATED);
	SYS_TIMER_start(&HDW_FAN_increment_timer, 300);
	HDW_FAN_set_speed(0);
	HDW_FAN_PID_enabled = false;
}
void HDW_FAN_process()
{
	if(SYS_TIMER_expired(HDW_FAN_increment_timer))
	{
		HDW_FAN_update_speed(HDW_FAN_get_speed());
	}
	if(HDW_FAN_PID_enabled)
	{
		HDW_FAN_PID_process();
	}
}

