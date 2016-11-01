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
#include "audioMixer.h"
#include "beatController.h"

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
static _Bool running = false;
pthread_t i2cThreadId;

int16_t x;
int16_t y;
int16_t z;

// Creates and launch the thread needed to do i2c work
void i2c_Init() {
	initI2cBus();
	setActive();
	running = true;
	pthread_create(&i2cThreadId, NULL, i2c_thread, NULL);
}

void* i2c_thread(void* args) {
	while (running) {
		readFromI2cReg(i2cFileDesc, OUT_X_MSB);

		struct timespec delay;
		delay.tv_sec = 0;
		delay.tv_nsec = 10000000;
		nanosleep(&delay, NULL);
	}

	pthread_exit(0);
}

void i2c_cleanup() {
	running = false;
}

int16_t getXValue() {
	return x;
}

int16_t getYValue() {
	return y;
}

int16_t getZValue() {
	return z;
}

void setActive() {
	writeI2cReg(i2cFileDesc, 0x2a, 0x01);
}

// Does all the work to enable capes and prepare the i2c bus
int initI2cBus(void) {
	// Loads cape if not already loaded for i2c
	FILE *slot = fopen("/sys/devices/platform/bone_capemgr/slots", "w+");
	if (!slot) {
		printf("I2C: Unable to load I2C cape\n");
		printf(
				"try:   echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots\n");
	}
	fprintf(slot, "BB-I2C1");
	fclose(slot);

	// Open the bus
	i2cFileDesc = open(I2CDRV_LINUX_BUS1, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C: Unable to open bus for read/write (%s)\n",
		I2CDRV_LINUX_BUS1);
		printf("Error is: %d\n", i2cFileDesc);
		printf(
				"try:   echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots\n");
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
	if (res != sizeof(regAddr)) {
		perror("I2C: Unable to write to i2c register\n");
	}

	char value[7];
	res = read(i2cFileDesc, &value, 7);

	x = (value[1] << 8) | (value[2]);
	y = (value[3] << 8) | (value[4]);
	z = (value[5] << 8) | (value[6]);
}

