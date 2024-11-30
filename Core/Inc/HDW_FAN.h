/*
 * HDW_FAN.h
 *
 *  Created on: Mar 7, 2024
 *      Author: marcu
 */

#ifndef INC_HDW_FAN_H_
#define INC_HDW_FAN_H_

#include <stdio.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"



void HDW_FAN_init(void);
void HDW_FAN_set_speed(uint8_t percent_fan_speed);
uint8_t HDW_FAN_get_speed();
bool HDW_FAN_is_PID_enabled(void);
void HDW_FAN_set_PID_enabled(bool set);
void HDW_FAN_process(void);


#endif /* INC_HDW_FAN_H_ */
