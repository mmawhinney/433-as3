#ifndef _ACCEL_CONTROLLER_H_
#define _ACCEL_CONTROLLER_H_

void i2c_Init();

void* i2c_thread(void* args);

void setActive();

int initI2cBus(void);

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
void writeStringToGpio(char *fileName, int pin, char* res);
void readFromI2cReg(int i2cFileDesc, unsigned char regAddr);
int16_t getXValue();
int16_t getYValue();
int16_t getZValue();
void i2c_cleanup();


#endif
