// ****************************************************************************
/*
 * HDW_LED.h
 *
 *  Created on: Mar 7, 2024
 *      Author: Marcus Lechner
 *      Based on Example code provided by Nico Korn
 */

// ****************************************************************************

// Define to prevent recursive inclusion **************************************
#ifndef __WS2812B_H
#define __WS2812B_H

// Include ********************************************************************
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
// Exported defines ***********************************************************
//#define ONBOARD_LED   // used if a ws2812b led stripe is attached to GPIO Port 0, Pin 16


#define USE_BRIGHTNESS 0



#define STRIPE_LED   // used if a ws2812b led stripe is attached to GPIO Port 0, Pin 4
#define LED_LOWER    //lower 10 LEDs

#ifdef STRIPE_LED
#define PIXEL_COUNT              ( 125u )
#define WS2812B_PIN              ( 4u )
#define WS2812B_PORT             ( 0u )
#endif

#ifdef LED_LOWER
#define LED_LOWER_PIXEL_COUNT    ( 10u )
#define LED_LOWER_PIN              ( 5u )
#define LED_LOWER_PORT             ( 0u )
#endif

#ifdef ONBOARD_LED
#define PIXEL_COUNT              ( 1u )
#define WS2812B_PIN              ( 16u )
#define WS2812B_PORT             ( 0u )
#endif

#define HDW_LED_RED_OFFSET      8
#define HDW_LED_GREEN_OFFSET    0
#define HDW_LED_BLUE_OFFSET     16

#define HDW_LED_ANIMATION_QUEUE_LEN 10

// Exported types *************************************************************


typedef enum
{
   WS2812B_OK       = 0x00U,
   WS2812B_ERROR    = 0x01U,
   WS2812B_BUSY     = 0x02U,
   WS2812B_TIMEOUT  = 0x03U,
   WS2812B_READY    = 0x04U,
   WS2812B_RESET    = 0x05U
} WS2812B_StatusTypeDef;



typedef enum HDW_LED_ANIMATION
{
	HDW_LED_START_UP = 0X00,
	HDW_LED_ALL_OFF,
	HDW_LED_RAINBOW,
	HDW_LED_GLOW,
	HDW_LED_DANCING,
	HDW_LED_TWINKLE,
	HDW_LED_TAIL_INCREASE,
	HDW_LED_TAIL_DECREASE,
    HDW_LED_ANIMATION_NUM,
}HDW_LED_ANIMATION;

typedef void (*pfAnnimation)(void);

typedef struct
{
	HDW_LED_ANIMATION annimation;
	pfAnnimation function;
}HDW_LED_annimation_list_t;

typedef enum HDW_LED_ON_COLOUR
{

	HDW_LED_RED = 0x00,
	HDW_LED_ORANGE,
	HDW_LED_YELLOW,
	HDW_LED_GREEN,
	HDW_LED_CYAN,
	HDW_LED_BLUE,
	HDW_LED_MAGENTA,
	HDW_LED_PINK,
	HDW_LED_WHITE,
	HDW_LED_OFF,
	HDW_LED_ON_STATE_NUM,
}HDW_LED_ON_COLOUR;

typedef struct HDW_LED_ANIMATION_STEP
{
	enum HDW_LED_ON_COLOUR LEDState[PIXEL_COUNT];
	uint32_t HoldTime; //time held in state (ms)
}HDW_LED_ANIMATION_STEP;

typedef struct HDW_LED_ANIMATION_TABLE
{
	const HDW_LED_ANIMATION_STEP *pLEDStateTable; 
	uint8_t NumSteps; // number of steps in table
	uint8_t Repititions; // number of times to animation
	bool bLoop; //true to loop forever
}HDW_LED_ANIMATION_TABLE;

typedef enum HDW_LED_RUN_STATE
{
	HDW_LED_SET_SHADOW = 0x00,
	HDW_LED_WRITE_ON_CTRL,
	HDW_LED_WAIT_HOLD,
	HDW_LED_RUN_STATE_NUM,
}HDW_LED_RUN_STATE;

typedef struct HDW_LED_ANIMATION_QUEUE
{
	enum HDW_LED_ANIMATION Queue[HDW_LED_ANIMATION_QUEUE_LEN]; //queue of pointers to constant transfer config structs and flags
	uint8_t Size;   //Number of enteries pushed onto queue
	uint8_t Head;   //Head of queue
	uint8_t Tail;   //Tail of queue
	
}HDW_LED_ANIMATION_QUEUE;


// Exported functions *********************************************************
void   					HDW_LED_Config            ( void );
void 					HDW_LED_Init( void );
void                    HDW_LED_process(void);



void HDW_LED_set_annimation(HDW_LED_ANIMATION new_annimation);
HDW_LED_ANIMATION HDW_LED_get_annimation();
void HDW_LED_set_brightness(uint8_t brightness);
#endif // __WS2812B_H
