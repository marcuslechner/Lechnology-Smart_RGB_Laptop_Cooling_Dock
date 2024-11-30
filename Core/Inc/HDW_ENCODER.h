/*
 * HDW_ENCODER.h
 *
 *  Created on: Mar 7, 2024
 *      Author: Marcus Lechner
 */

#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef ENCODER_TIM
#define ENCODER_TIM                            htim2
#endif

#ifndef ENCODER_GPIO_PORT
#define ENCODER_GPIO_PORT                      GPIOA
#endif
#ifndef ENCODER_GPIO_CH1
#define ENCODER_GPIO_CH1                       GPIO_PIN_0
#endif

#ifndef ENCODER_GPIO_CH2
#define ENCODER_GPIO_CH2                       GPIO_PIN_1
#endif


extern TIM_HandleTypeDef htim2;

typedef enum {
  Incremented = 1,
  Decremented = -1,
  Neutral = 0,
} HDW_ENCODER_Status;


void HDW_ENCODER_Config(void);
void HDW_ENCODER_Init(void);
uint16_t HDW_ENCODER_Read();
void HDW_ENCODER_process(void);
uint8_t HDW_ENCODER_get_count(void);

#endif
