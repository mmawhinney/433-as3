#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include "audioMixer.h"
#include "control.h"
#include "beatController.h"
#include "accelController.h"
#include "udp.h"

const unsigned int MS_TO_NS_FACTOR = 1000000;
const unsigned int NSEC_MAX = 1000000000;
const int PORT = 12345;

#define X_AXIS_THRESHOLD 5000
#define Y_AXIS_THRESHOLD 5000
#define Z_AXIS_THRESHOLD 23500

_Bool playing = false;
struct timespec beat_delay;

void setMsDelay(int delayInMs);
void calculateDelay();

int main() {
	AudioMixer_init();
	Control_init();
	i2c_Init();
	udp_startSocket(PORT);
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
		case 0: {
			int x = getXValue();
			int y = getYValue();
			int z = getZValue();
			if(x > X_AXIS_THRESHOLD) {
				BeatController_playHiHat(hiHatFile);
			} else if(y > Y_AXIS_THRESHOLD) {
				BeatController_playSnare(snareFile);
			} else if(z > Z_AXIS_THRESHOLD) {
				BeatController_playBass(bassFile);
			}
			beat_delay.tv_sec = 0;
			beat_delay.tv_nsec = 100000000;
			nanosleep(&beat_delay, NULL);
			break;
		}
		case 1: {
			BeatController_playRockBeat(beatCount, hiHatFile, bassFile, snareFile);
			nanosleep(&beat_delay, NULL);
			break;
		}
		case 2: {
			BeatController_playCustomBeat(beatCount, bassFile, hiHatFile);
			nanosleep(&beat_delay, NULL);
			break;
		}
		}
		beatCount++;
		if (beatCount == 8) {
			beatCount = 0;
		}
	}
	AudioMixer_cleanup();
	udp_stop();
	free(hiHatFile);
	free(bassFile);
	free(snareFile);
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
