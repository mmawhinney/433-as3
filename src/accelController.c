#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <stdint.h>
#include "accelController.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x1c
#define CTRL_REG1_ADDRESS 0x2a
#define ACTIVE 0x01
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06



int i2cFileDesc;
pthread_t i2cThreadId;

// Creates and launch the thread needed to do i2c work
void i2c_Init() {
	initI2cBus();
	setActive();
	pthread_create(&i2cThreadId, NULL, i2c_thread, NULL);
}

void* i2c_thread(void* args){
	while(1) {
		readFromI2cReg(i2cFileDesc, OUT_X_MSB);
		struct timespec delay;
		delay.tv_sec = 0;
		delay.tv_nsec = 500000000;
		nanosleep(&delay, NULL);
	}
	
	pthread_exit(0);
}

void setActive() {
	writeI2cReg(i2cFileDesc, 0x2a, 0x01);
}

// Does all the work to enable capes and prepare the i2c bus
int initI2cBus(void) {
	// Open the bus
	i2cFileDesc = open(I2CDRV_LINUX_BUS1, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C: Unable to open bus for read/write (%s)\n",
		I2CDRV_LINUX_BUS1);
		printf("Error is: %d\n", i2cFileDesc);
		printf("try:   echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots\n");
		exit(1);
	}
	int result = ioctl(i2cFileDesc, I2C_SLAVE, I2C_DEVICE_ADDRESS);
	if (result < 0) {
		perror("I2C: Unable to set I2C device to slave address.");
		exit(1);
	}
	return i2cFileDesc;
}

// Write the value into register
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}
}

void readFromI2cReg(int i2cFileDesc, unsigned char regAddr) {
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if(res != sizeof(regAddr)) {
		perror("I2C: Unable to write to i2c register\n");
	}

	char value[7];
	res = read(i2cFileDesc, &value, 7);
	for(int i = 0; i < 7; i++) {
		printf("res[%d] = %d\n", i, value[i]);
	}
	int16_t x = (value[1] << 8) | (value[2]);
	int16_t y = (value[3] << 8) | (value[4]);
	int16_t z = (value[5] << 8) | (value[6]);
	printf("x = %d\n", x);
	printf("y = %d\n", y);
	printf("z = %d\n", z);
}

// // Read a value from file of given name and given GPIO pin and write to res
// void writeStringToGpio(char *fileName, int pin, char* res) {
// 	char dir[BUF];
// 	snprintf(dir, sizeof(dir), GPIO_DIR "/gpio%d/%s", pin, fileName);
// 	FILE *file = fopen(dir, "w");
// 	if (file == NULL) {
// 		printf("ERROR: Unable to open file (%s) for write\n", fileName);
// 		exit(-1);
// 	}
// 	fprintf(file, "%s", res);
// 	fclose(file);

// }
