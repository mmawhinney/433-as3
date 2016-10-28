#include "I2C.h"

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

// Some code adapted from guide
#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"
#define GPIO_DIR "/sys/class/gpio"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

#define LEFT_GPIO 61
#define RIGHT_GPIO 44
#define BUF 512

int i2cFileDesc;
pthread_t i2cThreadId;

// Creates and launch the thread needed to do i2c work
void i2c_Init() {
	initI2cBus();
	pthread_create(&i2cThreadId, NULL, i2c_thread, NULL);
}

void* i2c_thread(void* args){

	pthread_exit(0);
}
// Does all the work to enable capes and prepare the i2c bus
int initI2cBus(void) {
	// Loads cape if not already loded for i2c
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

// Read a value from file of given name and given GPIO pin and write to res
void writeStringToGpio(char *fileName, int pin, char* res) {
	char dir[BUF];
	snprintf(dir, sizeof(dir), GPIO_DIR "/gpio%d/%s", pin, fileName);
	FILE *file = fopen(dir, "w");
	if (file == NULL) {
		printf("ERROR: Unable to open file (%s) for write\n", fileName);
		exit(-1);
	}
	fprintf(file, "%s", res);
	fclose(file);

}
