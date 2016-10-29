#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "audioMixer.h"
#include "control.h"
#include "beatController.h"
#include "accelController.h"

const unsigned int MS_TO_NS_FACTOR = 1000000;
const unsigned int NSEC_MAX = 1000000000;

_Bool playing = false;
struct timespec beat_delay;

void Beat_init();
void setMsDelay(int delayInMs);
void calculateDelay();

int main() {
	AudioMixer_init();
	Beat_init();
	Control_init();
	i2c_Init();
	int beatCount = 0;
	playing = true;
	int currentBeat = 1;
	wavedata_t *hiHatFile = malloc(sizeof(wavedata_t));
	wavedata_t *bassFile = malloc(sizeof(wavedata_t));
	wavedata_t *snareFile = malloc(sizeof(wavedata_t));
	while (playing) {
		calculateDelay();
		currentBeat = BeatController_getCurrentBeat();
		switch (currentBeat) {
		case 0:
			break;
		case 1: {
			BeatController_playRockBeat(beatCount, hiHatFile, bassFile, snareFile);
			break;
		}
		//TODO: make better
		case 2: {
			BeatController_playCustomBeat(beatCount, bassFile, hiHatFile);
			break;
		}
		}
		beatCount++;
		// printf("s : %ld, ns: %ld\n", beat_delay.tv_sec, beat_delay.tv_nsec);
		nanosleep(&beat_delay, NULL);
		if (beatCount == 8) {
			beatCount = 0;
		}
	}
	AudioMixer_cleanup();
	free(hiHatFile);
	free(bassFile);
	free(snareFile);
}

void Beat_init() {
	calculateDelay();
	// currentBeat = 1;
	playing = true;
}

void setMsDelay(int delayInMs) {
	long long temp = delayInMs * MS_TO_NS_FACTOR;
	beat_delay.tv_sec = temp / NSEC_MAX;
	beat_delay.tv_nsec = temp % NSEC_MAX;
}

void calculateDelay() {
	int bpm = BeatController_getBPM();
	double delay = (60.0 / bpm / 2.0) * 1000;
	setMsDelay((int)delay);
}
