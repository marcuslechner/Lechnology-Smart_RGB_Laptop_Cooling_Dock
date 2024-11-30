/*
 * APP_DISPLAY.h
 *
 *  Created on: Nov 20, 2023
 *      Author: Marcus Lechner
 */

#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_
#include <stdbool.h>
#include <stdint.h>



typedef enum APP_DISPLAY_PAGE_NAME
{
	APP_DISPLAY_HOME_PAGE = 0x00,
	APP_DISPLAY_FAN_SPEED = 0x01,
	APP_DISPLAY_ANNIMATION = 0x02,
	APP_DISPLAY_TEMPERATURE = 0x03,
	APP_DISPLAY_BRIGHTNESS = 0X04,
	APP_DISPLAY_PID = 0x05,
	APP_DISPLAY_MAX_PAGE_NUM
}APP_DISPLAY_PAGE_NAME;

typedef struct APP_DISPLAY_STATUS_FLAGS
{
	APP_DISPLAY_PAGE_NAME page_number; //for now we wont save what tab each page is on
	uint8_t tab_number[APP_DISPLAY_MAX_PAGE_NUM]; //every page should be a struct that holds the tab number
	bool	new_update;
	bool bStarted;
}APP_DISPLAY_STATUS_FLAGS;

void APP_DISPLAY_init(void);
void APP_DISPLAY_process(void);
void APP_DISPLAY_increment_page_number(void);
void APP_DISPLAY_increment_tab_number(void);
void APP_DISPLAY_decrement_tab_number(void);
void APP_DISPLAY_refresh_screen(void);

#endif /* APP_DISPLAY_H_ */
