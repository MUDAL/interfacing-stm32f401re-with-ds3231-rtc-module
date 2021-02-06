#ifndef _GPIO_H
#define _GPIO_H

enum Gpio_reg_level
{
	REG_LOW = 0,
	REG_HIGH
};

extern void GPIO_Output_Init(GPIO_TypeDef* gpioPort, uint32_t gpioMode);
extern void GPIO_Output_On(GPIO_TypeDef* gpioPort, uint32_t gpioPin);
extern void GPIO_Output_Off(GPIO_TypeDef* gpioPort, uint32_t gpioPin);
extern void GPIO_Output_Toggle(GPIO_TypeDef* gpioPort, uint32_t gpioPin);
extern void GPIO_Alternate_Function_Init(GPIO_TypeDef* gpioPort, uint32_t gpioOutputType,
									 uint32_t gpioMode, uint8_t gpioAF_Level, uint32_t gpioAF);

#endif //_GPIO_H
