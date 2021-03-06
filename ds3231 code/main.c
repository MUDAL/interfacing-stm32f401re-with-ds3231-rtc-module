#include <stdint.h>

#include "clocks.h"
#include "systick.h"
#include "iwdg.h"

#include "ds3231.h"

/*
Interfacing the stm32f401re with the ds3231 rtc module.
For debugging,

ds3231_t struct variable contains 4 members:
1.)seconds
2.)minutes
3.)hours
4.)period (AM or PM)

period = 0 for AM
period = 1 for PM
period = 2 (no AM or PM due to 24 hour format)

Watchdog timer is used to reset the system in case
there's a software failure.

*/


ds3231_t time;

int main(void)
{
	Clocks_Init();
	SysTick_Init();
	IWDG_Init();
	DS3231_Init();
	
	//Example 1: Sets time to 10:45 pm
	
	//DS3231_Set_12_Hour_Format(DS3231_PERIOD_PM);
	//DS3231_Set_Time(10,45);
	
	//Example 2: Sets time to 16:28
	
	//DS3231_Set_24_Hour_Format();
	//DS3231_Set_Time(16,28);
	
	//Example 3: Sets time to 8:20 am
	
	//DS3231_Set_12_Hour_Format(DS3231_PERIOD_AM);
	//DS3231_Set_Time(8,20);
	
	systick_t watchdogTimer;
	watchdogTimer.start = SysTick_GetTick();
	watchdogTimer.ticksToWait = 100;
	
	while(1)
	{
		DS3231_Get_Time(&time);
		
		if ( (SysTick_GetTick() - watchdogTimer.start) >= watchdogTimer.ticksToWait )
		{
			IWDG_Refresh();
			watchdogTimer.start = SysTick_GetTick();
		}
	}
	
}
