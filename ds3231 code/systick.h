#ifndef _SYSTICK_H
#define _SYSTICK_H

typedef struct
{
	uint32_t start;
	uint32_t ticksToWait;
}systick_t;

extern void SysTick_Init(void);
extern uint32_t SysTick_GetTick(void);
extern void SysTick_DelayMs(uint32_t delayTime);

#endif //_SYSTICK_H
