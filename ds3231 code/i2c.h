#ifndef _I2C_H
#define _I2C_H

extern void I2C_Init(I2C_TypeDef* i2cPort, uint32_t i2cClock, uint32_t i2cCCR, uint32_t i2cTRISE);
extern void I2C_Write(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
extern void I2C_Read_1Byte(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t* data); 
extern void I2C_Read_2Bytes(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t* data); 
extern void I2C_Read_Multiple(I2C_TypeDef* i2cPort, uint8_t slaveAddr, uint8_t regAddr, uint8_t* data, uint32_t length); 

#endif //_I2C_H
