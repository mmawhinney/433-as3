#ifndef I2C_H_
#define I2C_H_

int initI2cBus(void);

void i2c_Init(void);

void* i2c_thread(void* args);

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

void writeStringToGpio(char *fileName, int pin, char* res);

#endif
