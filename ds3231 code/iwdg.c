#include "stm32f4xx.h"                  // Device header
#include "iwdg.h"

//IWDG default settings: 512ms timeout

void IWDG_Init(void)
{
	RCC->CSR |= RCC_CSR_LSION; //enable LSI clock
	while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY); //wait for LSION to be ready
	IWDG->KR |= 0xCCCC; //start watchdog
}

void IWDG_Refresh(void)
{
	IWDG->KR |= 0xAAAA;
}	
