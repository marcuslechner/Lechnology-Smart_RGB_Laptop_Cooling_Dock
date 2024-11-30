/*
 * SYS_TIMER.h
 *
 *  Created on: Feb 22, 2024
 *      Author: marcu
 */

#ifndef INC_SYS_TIMER_H_
#define INC_SYS_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define SYS_TIMER_MAX_NUM_HANDLES		30

/* Typedefs---------------------------------------------------------- */
typedef struct SYS_TIMER_STATUS
{
	uint64_t RTCoverflowoffset;
	uint8_t SYS_TIMER_handle_counter;
	bool bLF_CLK_active;
}SYS_TIMER_STATUS;

//
typedef enum eSYS_TIMER_MODE
{
	TIMER_MODE_SINGLE_SHOT =0x00,
	TIMER_MODE_REPEATED,
	TIMER_MODE_NUM,

}eSYS_TIMER_MODE;


typedef struct SYS_TIMER_INSTANCE_SPEC
{
	bool bEnabled;
	uint64_t startCount;
	eSYS_TIMER_MODE timerMode;  //kept app_timer struct to avoid changing API
	uint32_t timerPeriodMS;

} SYS_TIMER_INSTANCE_SPEC;

void SYS_TIMER_init(void);
uint8_t  SYS_TIMER_create(eSYS_TIMER_MODE mode);
void SYS_TIMER_start(uint8_t *pHandle, uint32_t time_in_ms);
void SYS_TIMER_stop(uint8_t Handle);
bool SYS_TIMER_expired(uint8_t Handle);
uint32_t SYS_TIMER_get_elapsed_ms(uint8_t Handle);
void SYS_TIMER_stop_all(void);
uint32_t SYS_TIMER_get_RTC_time_ms(void);
uint64_t SYS_TIMER_get_RTC_time_us(void);

#endif /* INC_SYS_TIMER_H_ */
