#define GPIO_DIR "/sys/class/gpio"
#define JOY_UP 26
#define JOY_RIGHT 47
#define JOY_DOWN 46
#define JOY_LEFT 65
#define JOY_IN 27
#define PRESSED_VAL "0\n"

#include "control.h"
#include "udp.h"
#include "audioMixer.h"
#include "beatController.h"
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
	delay.tv_nsec = 500000000;
	pthread_create(&controlThreadId, NULL, control_thread, NULL);
}

void Control_cleanup() {
	pthread_join(controlThreadId, NULL);
}

void* control_thread(void* args) {
	_Bool pressed = false;
	while (polling) {
		if (!pressed) {
			if (checkPin(JOY_UP)) {
				increaseVolume();
				pressed = true;
			} else if (checkPin(JOY_RIGHT)) {
				increaseTempo();
				pressed = true;
			} else if (checkPin(JOY_DOWN)) {
				decreaseVolume();
				pressed = true;
			} else if (checkPin(JOY_LEFT)) {
				decreaseTempo();
				pressed = true;
			} else if (checkPin(JOY_IN)) {
				cycleBeat();
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

void increaseVolume() {
	int currentVolume = AudioMixer_getVolume();
	int newVolume = currentVolume + 5;
	if (newVolume <= AUDIOMIXER_MAX_VOLUME) {
		AudioMixer_setVolume(newVolume);
	}
}

void decreaseVolume() {
	int currentVolume = AudioMixer_getVolume();
	int newVolume = currentVolume - 5;
	if (newVolume >= 0) {
		AudioMixer_setVolume(newVolume);
	}
}

int getVolume() {
	return AudioMixer_getVolume();
}

void setBeat(int beat) {
	BeatController_setBeat(beat);
}

int getBeat() {
	return BeatController_getBeat();
}

void cycleBeat() {
	BeatController_cycleCurrentBeat();
}

void increaseTempo() {
	int currentBpm = BeatController_getBPM();
	int newBpm = currentBpm + 5;
	if (newBpm <= MAX_BPM) {
		BeatController_setBPM(newBpm);
	}
}

void decreaseTempo() {
	int currentBpm = BeatController_getBPM();
	int newBpm = currentBpm - 5;
	if (newBpm >= MIN_BPM) {
		BeatController_setBPM(newBpm);
	}
}

int getTempo() {
	return BeatController_getBPM();
}
