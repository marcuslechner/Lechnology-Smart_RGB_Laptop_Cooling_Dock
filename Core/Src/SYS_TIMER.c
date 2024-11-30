/*
 * SYS_TIMER.c
 *
 *  Created on: Feb 22, 2024
 *      Author: marcus Lechner
 */

#include "SYS_TIMER.h"
#include "main.h"

static SYS_TIMER_STATUS SYS_TIMER_status;
static SYS_TIMER_INSTANCE_SPEC SYS_timer_instance_spec[SYS_TIMER_MAX_NUM_HANDLES];
static uint32_t SYS_TIMER_get_RTC_tick(void);

void SYS_TIMER_init(void)
{
	memset(&SYS_TIMER_status, 0x00, sizeof(SYS_TIMER_status));
	memset(SYS_timer_instance_spec, 0x00, sizeof(SYS_timer_instance_spec));
}

uint8_t  SYS_TIMER_create(enum eSYS_TIMER_MODE mode)
{
	uint8_t Handle = 0xFF;
	if (SYS_TIMER_status.SYS_TIMER_handle_counter < SYS_TIMER_MAX_NUM_HANDLES)
	{//if handles left
		Handle = SYS_TIMER_status.SYS_TIMER_handle_counter;
		SYS_TIMER_status.SYS_TIMER_handle_counter++;
		SYS_timer_instance_spec[Handle].timerMode = mode;
		SYS_timer_instance_spec[Handle].bEnabled = false;
	}
	return (Handle);
}

void SYS_TIMER_start(uint8_t *pHandle, uint32_t time_in_ms)
{
	if (*pHandle < SYS_TIMER_status.SYS_TIMER_handle_counter)
	{
		SYS_timer_instance_spec[*pHandle].startCount = SYS_TIMER_get_RTC_tick();
		SYS_timer_instance_spec[*pHandle].bEnabled = true;
		SYS_timer_instance_spec[*pHandle].timerPeriodMS = time_in_ms;
	}
}

void SYS_TIMER_stop(uint8_t Handle)
{
	if (Handle < SYS_TIMER_status.SYS_TIMER_handle_counter)
	{
		SYS_timer_instance_spec[Handle].bEnabled = false;
	}
}

void SYS_TIMER_stop_all(void)
{
	uint8_t Handle;
	for (Handle = 0; Handle< SYS_TIMER_status.SYS_TIMER_handle_counter; Handle++)
	{
		SYS_TIMER_stop(Handle);
	}
}

bool SYS_TIMER_expired(uint8_t Handle)
{
	uint64_t  countDiff;
	bool bExpired = false;

	if (Handle < SYS_TIMER_status.SYS_TIMER_handle_counter)
	{
		if (SYS_timer_instance_spec[Handle].bEnabled)
		{
			countDiff = SYS_TIMER_get_RTC_tick() - SYS_timer_instance_spec[Handle].startCount;
			//APP_DEBUG_LOG_printf("CountDiff: %d", countDiff, "Start CNT : ", SYS_timer_instance_spec[Handle].startCount);
			if ((countDiff) >= SYS_timer_instance_spec[Handle].timerPeriodMS)
			{
				//APP_DEBUG_LOG_printf("Expired");
				bExpired = true;
				if (SYS_timer_instance_spec[Handle].timerMode == TIMER_MODE_SINGLE_SHOT)
				{
					SYS_timer_instance_spec[Handle].bEnabled = false;
				}
				else
				{
					SYS_timer_instance_spec[Handle].startCount += (SYS_timer_instance_spec[Handle].timerPeriodMS);
				}
			}
		}
	}
	return (bExpired);
}

uint32_t SYS_TIMER_get_elapsed_ms(uint8_t Handle)
{
	uint64_t  countDiff;
	uint32_t elapsedTimeMS = 0;

	if (Handle < SYS_TIMER_status.SYS_TIMER_handle_counter)
	{
		if (SYS_timer_instance_spec[Handle].bEnabled)
		{
			countDiff = SYS_TIMER_get_RTC_tick() - SYS_timer_instance_spec[Handle].startCount;
			elapsedTimeMS = (countDiff);
		}
	}
	return (elapsedTimeMS);
}

uint32_t SYS_TIMER_get_RTC_time_ms(void)
{
	uint32_t CurrTimeMS = HAL_GetTick();//(uint32_t ) SYS_TIMER_RTC_CNT_2_MS(SYS_TIMER_get_RTC_count());
	return (CurrTimeMS);
}


static uint32_t SYS_TIMER_get_RTC_tick(void)
{
	uint32_t CurrTimeMS = HAL_GetTick();//(uint32_t ) SYS_TIMER_RTC_CNT_2_MS(SYS_TIMER_get_RTC_count());
	return (CurrTimeMS);
}

