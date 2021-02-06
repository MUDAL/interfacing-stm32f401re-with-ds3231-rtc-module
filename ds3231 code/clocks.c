#include "stm32f4xx.h"
#include "clocks.h"

void Clocks_Init(void)
{
	//wait for HSI clock to stabilize
	while (!(RCC->CR & RCC_CR_HSIRDY));
	//clock enable for GPIOA,GPIOB
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);
	//clock enable for I2C1
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; 
}

