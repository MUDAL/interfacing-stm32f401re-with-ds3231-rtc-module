#include "stm32f4xx.h"
#include "i2c.h"

void I2C_Init(I2C_TypeDef* i2cPort, uint32_t i2cClock, uint32_t i2cCCR, uint32_t i2cTRISE)
{
	//I2C software reset to eliminate BUSY flag glitch
	i2cPort->CR1 |= I2C_CR1_SWRST;
	i2cPort->CR1 &= ~I2C_CR1_SWRST;
	
	i2cPort->CR2 |= i2cClock;
	i2cPort->CCR = i2cCCR;
	i2cPort->TRISE = i2cTRISE;
	i2cPort->CR1 |= I2C_CR1_PE; //Enable I2C1 peripheral
}

void I2C_Write(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
	volatile uint32_t read_I2C_SR2;
	while (i2cPort->SR2 & I2C_SR2_BUSY); //wait for I2C busy bit to be cleared	
	 
	
	i2cPort->CR1 |= I2C_CR1_START; //Generate start condition
	while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB);//wait for start bit to be set
			
	i2cPort->DR = slaveAddr << 1; //slave address
	while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);//wait for ADDR bit to be set
	read_I2C_SR2 = i2cPort->SR2;
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);//wait for TXE bit to be set
	i2cPort->DR = regAddr;
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE); 
	i2cPort->DR = data;
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE || (i2cPort->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF); 
	i2cPort->CR1 |= I2C_CR1_STOP; 
		
}

void I2C_Read_1Byte(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t* data)
{
	volatile uint32_t read_I2C_SR2;
	while (i2cPort->SR2 & I2C_SR2_BUSY);//wait for I2C busy bit to be cleared 
	
	i2cPort->CR1 |= I2C_CR1_START; //Generate start condition
	while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB);//wait for start bit to be set
			
	i2cPort->DR = slaveAddr << 1; //slave address
	while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR); //wait for ADDR bit to be set
	read_I2C_SR2 = i2cPort->SR2;
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE); //wait for TXE bit to be set
	i2cPort->DR = regAddr;
		
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE); 
	i2cPort->CR1 |= I2C_CR1_START; 
	while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	i2cPort->DR = slaveAddr << 1 | 1;
	
	while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR); 
	i2cPort->CR1 &= ~I2C_CR1_ACK;//Send NACK
	read_I2C_SR2 = i2cPort->SR2;
		
	i2cPort->CR1 |= I2C_CR1_STOP; //Send STOP
	while((i2cPort->SR1 & I2C_SR1_RXNE) != I2C_SR1_RXNE); //Wait for RXNE bit to be set
	*data = i2cPort->DR;
		
}

void I2C_Read_2Bytes(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t* data)
{
	volatile uint32_t read_I2C_SR2;
	while (i2cPort->SR2 & I2C_SR2_BUSY);//wait for I2C busy bit to be cleared 
	
	i2cPort->CR1 |= I2C_CR1_START; //Generate start condition
	while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB);//wait for start bit to be set
			
	i2cPort->DR = slaveAddr << 1; //slave address
	while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR); //wait for ADDR bit to be set
	read_I2C_SR2 = i2cPort->SR2;
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);//wait for TXE bit to be set
	i2cPort->DR = regAddr;
		
	while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE); 
	i2cPort->CR1 |= I2C_CR1_START; 
	while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	i2cPort->DR = slaveAddr << 1 | 1;
	
	while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR); 
	i2cPort->CR1 &= ~I2C_CR1_ACK;//Send NACK
	i2cPort->CR1 |= I2C_CR1_POS; 
	read_I2C_SR2 = i2cPort->SR2;
		
	while((i2cPort->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF);//Wait for BTF bit to be set
	i2cPort->CR1 |= I2C_CR1_STOP; //Send STOP
	data[0] = i2cPort->DR;
	data[1] = i2cPort->DR;
	
}

void I2C_Read_Multiple(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t* data, uint32_t length)
{
	if (length < 3)
	{
		return;
	}
	
	else
	{
		volatile uint32_t read_I2C_SR2;
		while (i2cPort->SR2 & I2C_SR2_BUSY);//wait for I2C busy bit to be cleared
		
		i2cPort->CR1 |= I2C_CR1_START; //Generate start condition
		while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB); //wait for start bit to be set
				
		i2cPort->DR = slaveAddr << 1; //slave address
		while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR); //wait for ADDR bit to be set
		read_I2C_SR2 = i2cPort->SR2;
		while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE); //wait for TXE bit to be set
		i2cPort->DR = regAddr;
			
		while((i2cPort->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE); 
		i2cPort->CR1 |= I2C_CR1_START; 
		while((i2cPort->SR1 & I2C_SR1_SB) != I2C_SR1_SB); 
		i2cPort->DR = slaveAddr << 1 | 1;
		
		while((i2cPort->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR); 
		read_I2C_SR2 = i2cPort->SR2;
		i2cPort->CR1 |= I2C_CR1_ACK; //Send ACK
		
		//Read incoming data
		for (uint32_t i = 0; i < length - 3; i++)
		{
			while((i2cPort->SR1 & I2C_SR1_RXNE) != I2C_SR1_RXNE); //Wait for RXNE bit to be set
			data[i] = i2cPort->DR;
		}
		
		while((i2cPort->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF);//Wait for BTF bit to be set
		i2cPort->CR1 &= ~I2C_CR1_ACK;//Send NACK	
		data[length - 3] = I2C1->DR;
			
		while((i2cPort->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF);
		i2cPort->CR1 |= I2C_CR1_STOP; //Send STOP
		data[length - 2] = i2cPort->DR;
		data[length - 1] = i2cPort->DR;
	}
}



