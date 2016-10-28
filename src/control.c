#define GPIO_DIR "/sys/class/gpio"
#define JOY_UP 26
#define JOY_RIGHT 47
#define JOY_DOWN 46
#define JOY_LEFT 65
#define JOY_IN 27
#define PRESSED_VAL "0\n"

#include "control.h"
#include "udp.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

const int max_length = 1024;
_Bool polling = false;
struct timespec delay;
pthread_t controlThreadId;

void Control_init() {
	polling = true;
	delay.tv_sec = 0;
	delay.tv_nsec = 50000000;
	pthread_create(&controlThreadId, NULL, control_thread, NULL);
}

void Control_cleanup() {
	pthread_join(controlThreadId, NULL);
}

void* control_thread(void* args) {
	_Bool pressed = false;
	while (polling) {
		// TODO: Debounce
		if (!pressed) {
			if (checkPin(JOY_UP)) {
				printf("Vol up\n");
				pressed = true;
			} else if (checkPin(JOY_RIGHT)) {
				printf("Vol fast\n");
				pressed = true;
			} else if (checkPin(JOY_DOWN)) {
				printf("Vol dwn\n");
				pressed = true;
			} else if (checkPin(JOY_LEFT)) {
				printf("Vol slow\n");
				pressed = true;
			} else if (checkPin(JOY_IN)) {
				printf("cycle\n");
				pressed = true;
			}
			if (pressed) {
				nanosleep(&delay, NULL);
				pressed = false;
			}
		}
	}
	pthread_exit(0);
}

_Bool checkPin(int pin) {
	exportGpio(pin);
	char res[max_length];
	readFromFile("value", pin, res);
	_Bool comp = strcmp(PRESSED_VAL, res);
	unexportGpio(pin);
	return !comp;
}

void readFromFile(char *fileName, int pin, char* res) {
	char dir[max_length];
	snprintf(dir, sizeof(dir), GPIO_DIR "/gpio%d/%s", pin, fileName);

	FILE *file = fopen(dir, "r");
	if (file == NULL) {
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}

	char buff[max_length];
	fgets(buff, max_length, file);

	fclose(file);

	strcpy(res, buff);
}

// Set the pin to export
void exportGpio(int pin) {
	FILE *pinFile = fopen(GPIO_DIR "/export", "w");
	if (pinFile == NULL) {
		printf("ERROR: Unable to open export file. \n");
		exit(1);
	}

	fprintf(pinFile, "%d", pin);

	fclose(pinFile);
}

// Set the pin to unexport
void unexportGpio(int pin) {
	FILE *pinFile = fopen(GPIO_DIR "/unexport", "w");
	if (pinFile == NULL) {
		printf("ERROR: Unable to open unexport file. \n");
		exit(1);
	}

	fprintf(pinFile, "%d", pin);

	fclose(pinFile);
}
