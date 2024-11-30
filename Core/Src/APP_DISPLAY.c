/*
 * APP_DISPLAY.c
 *
 *  Created on: Nov 20, 2023
 *      Author: Marcus Lechner
 */
/* NOTES
 *
 *
 *
 *
 *
 * TO DO
 *
 *
 */

#define NUM_MAX_TABS 10	//number of messages we will be displaying
#define NUM_MAX_PAGES 3


/* Includes ------------------------------------------------------------------*/
#include "APP_DISPLAY.h"
#include "main.h"

#include "gfx.h"
#include "fonts.h"
#include "ftoa.h"
#include "oled.h"
#include "HDW_LED.h"
#include "HDW_FAN.h"
#include "HDW_BME280.h"
#include "SYS_TIMER.h"
#include <stdio.h>
#include <string.h>

char buffer[8];
static APP_DISPLAY_STATUS_FLAGS APP_DISPLAY_status_flags;

static void APP_DISPLAY_start_screen(void);
static void APP_DISPLAY_fan_speed(void);
static void APP_DISPLAY_display_RGB_control(void);
static void APP_DISPLAY_temperature(void);
static void APP_DISPLAY_brightness(void);
static void APP_DISPLAY_pid(void);
static void APP_DISPLAY_update_screen(void);

static uint8_t APP_DISPLAY_refresh_timer;

void APP_DISPLAY_init(void)
{
	APP_DISPLAY_status_flags.page_number = APP_DISPLAY_HOME_PAGE;
	APP_DISPLAY_status_flags.new_update = false;
	APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_BRIGHTNESS] = 40;
	APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_TEMPERATURE] = 10;
	APP_DISPLAY_refresh_timer = SYS_TIMER_create(TIMER_MODE_SINGLE_SHOT);
	SYS_TIMER_start(&APP_DISPLAY_refresh_timer, 1000);

}

void APP_DISPLAY_process(void)		//paints the screen based on the number you have selected
{
//	clear();//clears OLED
	//TODO: change to if statements
	//TODO: change to function pointers
	if(APP_DISPLAY_status_flags.new_update || SYS_TIMER_expired(APP_DISPLAY_refresh_timer))
	{
		SYS_TIMER_start(&APP_DISPLAY_refresh_timer, 1000);
		switch(APP_DISPLAY_status_flags.page_number) //count of the encoder position
		{
			case APP_DISPLAY_HOME_PAGE: //out of range negative
			{
				APP_DISPLAY_start_screen();
				break;
			}
			case APP_DISPLAY_FAN_SPEED: //out of range positive
			{
				APP_DISPLAY_fan_speed();
				break;
			}

			case APP_DISPLAY_ANNIMATION: //out of range positive
			{
				APP_DISPLAY_display_RGB_control();
				break;
			}
			case APP_DISPLAY_TEMPERATURE: //out of range positive
			{
				APP_DISPLAY_temperature();
				break;
			}
			case APP_DISPLAY_BRIGHTNESS: //out of range positive
			{
				APP_DISPLAY_brightness();
				break;
			}
			case APP_DISPLAY_PID: //out of range positive
			{
				APP_DISPLAY_pid();
				break;
			}
			case APP_DISPLAY_MAX_PAGE_NUM: //out of range positive
			{
				APP_DISPLAY_start_screen();
				APP_DISPLAY_status_flags.page_number = 0;
				break;
			}
			default: //all other cases will print out values
			{
				break;
			}
		}
	}

}



static void APP_DISPLAY_start_screen(void)
{
	clear();
	graphics_text(4,0,FONT_NINE_DOT, "   Lechnology  "); //4 is a half space so that it's all centered
	graphics_text(0,16,FONT_NINE_DOT,"   Engineering   ");
	APP_DISPLAY_update_screen();
}

static void APP_DISPLAY_fan_speed(void)
{
	uint8_t display_percent = 0;
	static uint8_t max_speed_steps = 20;
//	static uint8_t max_fan_speed = 100;
	static uint8_t fan_increment = 5;//max_fan_speed / max_speed_steps;
	char display_text[16];

	if(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_FAN_SPEED] > max_speed_steps)
	{
		APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_FAN_SPEED] = max_speed_steps;
	}
	clear();
	display_percent = APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_FAN_SPEED] * fan_increment;
	HDW_FAN_set_speed(display_percent);
	graphics_text(0,0,FONT_NINE_DOT,"Fan Speed: ");
	strcpy(&display_text[0],	ftoa(display_percent, buffer, 0)); // converts float to a string
	graphics_text(0,16,FONT_NINE_DOT,&display_text[0]);
	APP_DISPLAY_update_screen();
}

static void APP_DISPLAY_display_RGB_control(void)
{
//	static uint8_t previous_case = 0;
//	if(previous_case != APP_DISPLAY_status_flags.tab_number)
	//TODO: don't call graphics text every second
//	{
		clear();
		graphics_text(0,0,FONT_NINE_DOT,"LED Colour: ");
		switch(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_ANNIMATION])
		{
			case HDW_LED_ALL_OFF:
			{
				graphics_text(0,16,FONT_NINE_DOT,"OFF");
				HDW_LED_set_annimation(HDW_LED_ALL_OFF);
				break;
			}
			case HDW_LED_RAINBOW:
			{
				graphics_text(0,16,FONT_NINE_DOT,"RAINBOW");
				HDW_LED_set_annimation(HDW_LED_RAINBOW);
				break;
			}
			case HDW_LED_GLOW:
			{
				graphics_text(0,16,FONT_NINE_DOT,"GLOW");
				HDW_LED_set_annimation(HDW_LED_GLOW);
				break;
			}
			case HDW_LED_DANCING:
			{
				graphics_text(0,16,FONT_NINE_DOT,"DANCE");
				HDW_LED_set_annimation(HDW_LED_DANCING);
				break;
			}
			case HDW_LED_TWINKLE:
			{
				graphics_text(0,16,FONT_NINE_DOT,"TWINKLE");
				HDW_LED_set_annimation(HDW_LED_TWINKLE);
				break;
			}
			case HDW_LED_TAIL_INCREASE:
			{
				graphics_text(0,16,FONT_NINE_DOT,"TAIL UP");
				HDW_LED_set_annimation(HDW_LED_TAIL_INCREASE);
				break;
			}
			case HDW_LED_TAIL_DECREASE:
			{
				graphics_text(0,16,FONT_NINE_DOT,"TAIL DOWN");
				HDW_LED_set_annimation(HDW_LED_TAIL_DECREASE);
				break;
			}
			case HDW_LED_ANIMATION_NUM:
			{
				graphics_text(0,16,FONT_NINE_DOT,"TAIL DOWN");
				HDW_LED_set_annimation(HDW_LED_TAIL_DECREASE);
				APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_ANNIMATION] = HDW_LED_TAIL_DECREASE;
				break;
			}
			default:
			{
				graphics_text(0,16,FONT_NINE_DOT,"OFF");
				HDW_LED_set_annimation(HDW_LED_ALL_OFF);
				APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_ANNIMATION] = HDW_LED_ALL_OFF;
				break;
			}
		}
//		previous_case = APP_DISPLAY_status_flags.tab_number;
		APP_DISPLAY_update_screen();
	}
//}

static void APP_DISPLAY_temperature(void)
{
	const uint8_t offset_temp_setting = 28;
	const uint8_t min_temp_steps = 0;
	const uint8_t max_temp_steps = 30; //28 to 30 based on 0.2 increment
	double target_temp =  0.0;
	double display_temp = 0.0;
	char display_text[16];

	//bounds the steps to calculate temp between 28 and 30
	if(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_TEMPERATURE] > max_temp_steps)
	{
		APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_TEMPERATURE] = max_temp_steps;
	}
	else if(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_TEMPERATURE] < min_temp_steps)
	{
		APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_TEMPERATURE] = min_temp_steps;
	}

	target_temp = offset_temp_setting + 0.2*APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_TEMPERATURE];

//	APP_PID_set_target_temp(target_temp);
	clear();
	if(HDW_FAN_is_PID_enabled())
	{
		display_temp = 	HDW_BME280_get_temperature();
		graphics_text(0,0,FONT_NINE_DOT,"Temp: ");
		strcpy(&display_text[0],	ftoa(display_temp, buffer, 2)); // converts float to a string
		strcat(&display_text[0], " C"); //TODO: make this the celcius symbol
		graphics_text(63,0,FONT_NINE_DOT,&display_text[0]);

		graphics_text(0,16,FONT_NINE_DOT,"Set: ");
		strcpy(&display_text[0],	ftoa(target_temp, buffer, 2)); // converts float to a string
		strcat(&display_text[0], " C"); //TODO: make this the celcius symbol
		graphics_text(63,16,FONT_NINE_DOT,&display_text[0]);
	}
	else
	{
		display_temp = 	HDW_BME280_get_temperature();
		graphics_text(0,0,FONT_NINE_DOT,"Temperature: ");
		strcpy(&display_text[0],	ftoa(display_temp, buffer, 2)); // converts float to a string
		strcat(&display_text[0], " Celcius"); //TODO: make this the celcius symbol
		graphics_text(0,16,FONT_NINE_DOT,&display_text[0]);
	}

	//else: use old display code

	APP_DISPLAY_update_screen();
}

static void APP_DISPLAY_brightness(void)
{
	uint8_t percent_brightness = 0;
	static uint8_t max_brightness = 51;
//	static uint8_t max_fan_speed = 100;
	static uint8_t fan_increment = 5;//max_fan_speed / max_speed_steps;
	char display_text[16];

	if(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_BRIGHTNESS] > max_brightness)
	{
		APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_BRIGHTNESS] = max_brightness;
	}
	clear();
	percent_brightness = APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_BRIGHTNESS] * fan_increment;
	HDW_LED_set_brightness(percent_brightness);
	graphics_text(0,0,FONT_NINE_DOT,"Brightness: ");
	strcpy(&display_text[0],	ftoa(percent_brightness, buffer, 0)); // converts float to a string
	graphics_text(0,16,FONT_NINE_DOT,&display_text[0]);
	APP_DISPLAY_update_screen();
}

static void APP_DISPLAY_pid(void)
{
	clear();
	if(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_PID] >= 1)
	{
		APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_PID] = 1;
		HDW_FAN_set_PID_enabled(true);
		graphics_text(0,0,FONT_NINE_DOT,"PID Control:");
		graphics_text(0,16,FONT_NINE_DOT,"Enabled");
	}
	else
	{
		HDW_FAN_set_PID_enabled(false);
		graphics_text(0,0,FONT_NINE_DOT,"PID Control:");
		graphics_text(0,16,FONT_NINE_DOT,"Disabled");
	}
	APP_DISPLAY_update_screen();

}

static void APP_DISPLAY_update_screen(void)
{
	oled_update();
	APP_DISPLAY_status_flags.new_update = false;
}

void APP_DISPLAY_increment_page_number(void)
{
	APP_DISPLAY_status_flags.page_number = (APP_DISPLAY_status_flags.page_number + 1) % APP_DISPLAY_MAX_PAGE_NUM;


}

void APP_DISPLAY_increment_tab_number(void)
{//increment based on what page its on
	//TODO: have an array of size APP_DISPLAY_MAX_PAGE_NUM that says what the max number of tabs is allowed so it doesnt need to be handled in every APP_DISPLAY function
	APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_status_flags.page_number] = (APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_status_flags.page_number] + 1);
}

void APP_DISPLAY_decrement_tab_number(void)
{
	if(APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_status_flags.page_number])
	{
		APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_status_flags.page_number] = (APP_DISPLAY_status_flags.tab_number[APP_DISPLAY_status_flags.page_number] - 1);
	}
}

void APP_DISPLAY_refresh_screen(void)
{
	APP_DISPLAY_status_flags.new_update = true;
}

