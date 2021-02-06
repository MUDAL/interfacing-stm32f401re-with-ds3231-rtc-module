#include "stm32f4xx.h"
#include "gpio.h"

void GPIO_Output_Init(GPIO_TypeDef* gpioPort, uint32_t gpioMode)
{
	gpioPort->MODER |= gpioMode; 
}

void GPIO_Output_On(GPIO_TypeDef* gpioPort, uint32_t gpioPin)
{
	gpioPort->ODR |= gpioPin;
}

void GPIO_Output_Off(GPIO_TypeDef* gpioPort, uint32_t gpioPin)
{
	gpioPort->ODR &= ~gpioPin;
}

void GPIO_Output_Toggle(GPIO_TypeDef* gpioPort, uint32_t gpioPin)
{
	gpioPort->ODR ^= gpioPin;
}

void GPIO_Alternate_Function_Init(GPIO_TypeDef* gpioPort, uint32_t gpioOutputType,
									 uint32_t gpioMode, uint8_t gpioAF_Level, uint32_t gpioAF)
{
	gpioPort->OTYPER |= gpioOutputType;
	gpioPort->MODER |= gpioMode;
	gpioPort->AFR[gpioAF_Level] |= gpioAF;
}

