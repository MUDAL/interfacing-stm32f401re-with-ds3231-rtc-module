#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "i2c.h"
#include "ds3231.h"

/*
The DS3231 uses Binary Coded Decimal (BCD) therefore all data must be
converted to BCD before being sent to the DS3231 module. Data read from
the module is in BCD form hence it must be converted to HEX in order
for the main application to use it for properly representing time.
*/

enum Register_address
{
	DS3231_ADDR = 0x68,
	SEC_REG_ADDR = 0x00,
	MIN_REG_ADDR = 0x01,
	HOUR_REG_ADDR = 0x02
};

static uint8_t BCD_To_Hex(uint8_t bcd)
{
	uint8_t hex;
	
	hex = (((bcd & 0xF0)>>4)*10) + (bcd&0x0F);
	return hex;
}

static uint8_t Hex_To_BCD(uint8_t hex)
{
	uint8_t bcd;
	
	uint8_t multipleOfTen = 0;
	while (hex >= 10)
	{
		hex -= 10;
		multipleOfTen++;
	}
	bcd = ( (multipleOfTen<<4) | hex );
	return bcd;
}

void DS3231_Init(void)
{
	GPIO_Alternate_Function_Init(GPIOB, (GPIO_OTYPER_OT8|GPIO_OTYPER_OT9), 
					(GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1), 
					REG_HIGH,(GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2));
	
	I2C_Init(I2C1,I2C_CR2_FREQ_4,80,17);
}

void DS3231_Get_Time(ds3231_t* pTime)
{
	uint8_t secondsBCD;
	uint8_t minutesBCD;
	uint8_t hoursBCD;
	uint8_t periodOfDay;
	
	I2C_Read_1Byte(I2C1, DS3231_ADDR, SEC_REG_ADDR, &secondsBCD);
	I2C_Read_1Byte(I2C1, DS3231_ADDR, MIN_REG_ADDR, &minutesBCD);
	I2C_Read_1Byte(I2C1, DS3231_ADDR, HOUR_REG_ADDR, &hoursBCD);
	
	if ( (hoursBCD & (1<<6)) == (1<<6) )
	{
		/*
		1.)read AM/PM status of 12 hour clock
		2.)if 12 hour format is the current clock format, 
		read only bits 4-0 of ds3231 hour register*/
		
		//periodOfDay = DS3231_PERIOD_AM or DS3231_PERIOD_PM
		periodOfDay = ( (hoursBCD & (1<<5)) >> 5 );
		hoursBCD = (hoursBCD & 0x1F);
	}
	else
	{
		/*
		1.)no AM/PM status for 24 hour clock
		2.)if 24 hour format is the current clock format, 
		read only bits 5-0 of ds3231 hour register*/
		periodOfDay = DS3231_NO_PERIOD;
		hoursBCD = (hoursBCD & 0x3F); 
	}
	
	pTime->seconds = BCD_To_Hex(secondsBCD);
	pTime->minutes = BCD_To_Hex(minutesBCD);
	pTime->hours = BCD_To_Hex(hoursBCD);
	pTime->period = periodOfDay;
}

void DS3231_Set_Time(uint8_t hour, uint8_t min)
{
	uint8_t minutesBCD;
	uint8_t prevHoursBCD;
	uint8_t hoursBCD;
	
	minutesBCD = Hex_To_BCD(min);
	hoursBCD = Hex_To_BCD(hour);
	
	I2C_Read_1Byte(I2C1, DS3231_ADDR, HOUR_REG_ADDR, &prevHoursBCD);
	/* 0xE0 preserves settings of the ds3231 hour register
	so that a write to the register doesn't clear the hour configurations.
	*/
	hoursBCD = ( hoursBCD | (prevHoursBCD & 0xE0) );
	
	I2C_Write(I2C1, DS3231_ADDR, MIN_REG_ADDR, minutesBCD);
	I2C_Write(I2C1, DS3231_ADDR, HOUR_REG_ADDR, hoursBCD);
}

void DS3231_Set_12_Hour_Format(uint8_t periodOfDay)
{
	/*
	12 hour mode is selected by setting bit 6 of 
	hour register (see datasheet) i.e. 1<<6.
	Setting bit 5 of hour register selects the
	period of day as PM i.e. 1<<5 otherwise AM
	is selected.
	*/
	uint8_t hoursBCD;
	
	I2C_Read_1Byte(I2C1, DS3231_ADDR, HOUR_REG_ADDR, &hoursBCD);
	if (periodOfDay == DS3231_PERIOD_PM)
	{
		hoursBCD |= ( (1<<6) | (1<<5) );
	}
	else
	{
		hoursBCD &= ~(1<<5);
		hoursBCD |= (1<<6);
	}
	I2C_Write(I2C1, DS3231_ADDR, HOUR_REG_ADDR, hoursBCD);
}

void DS3231_Set_24_Hour_Format(void)
{
	/*
	Bits 5 and 6 of the hour register must be cleared for
	24 hour format to be selected.
	*/
	uint8_t hoursBCD;
	
	I2C_Read_1Byte(I2C1, DS3231_ADDR, HOUR_REG_ADDR, &hoursBCD);
	hoursBCD &= ~( (1<<6) | (1<<5) );
	I2C_Write(I2C1, DS3231_ADDR, HOUR_REG_ADDR, hoursBCD);
}
