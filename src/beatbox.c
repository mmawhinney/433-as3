#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "audioMixer.h"
#include "control.h"

const unsigned int MS_TO_NS_FACTOR = 1000000;
const unsigned int NSEC_MAX = 1000000000;

_Bool playing = false;
static int currentBeat = 1;
static int bpm = 120;
struct timespec beat_delay;

#define BASE_DRUM_HARD "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_HAT "wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "wave-files/100059__menegass__gui-drum-snare-soft.wav"

void Beat_init();
void setMsDelay(int delayInMs);

int main() {
	AudioMixer_init();
	Beat_init();
	Control_init();
	int beatCount = 0;
	playing = true;
	while (playing) {
		switch (currentBeat) {
		case 0:
			break;
		case 1: {
			wavedata_t otherFile;
			AudioMixer_readWaveFileIntoMemory(HI_HAT, &otherFile);
			if (beatCount == 0 || beatCount == 4) {
				wavedata_t sampleFile;
				AudioMixer_readWaveFileIntoMemory(BASE_DRUM_HARD, &sampleFile);
			} else if (beatCount == 2 || beatCount == 6) {
				wavedata_t lastFile;
				AudioMixer_readWaveFileIntoMemory(SNARE, &lastFile);
			}
			break;
		}
		//TODO: make better
		case 2: {
			if (beatCount == 0 || beatCount == 1 || beatCount == 4
					|| beatCount == 5) {
				wavedata_t otherFile;
				AudioMixer_readWaveFileIntoMemory(BASE_DRUM_HARD, &otherFile);
			} else if (beatCount == 2 || beatCount == 6) {
				wavedata_t sampleFile;
				AudioMixer_readWaveFileIntoMemory(HI_HAT, &sampleFile);
			}
			break;
		}
		}
		beatCount++;
//		printf("s : %ld, ns: %ld\n", beat_delay.tv_sec, beat_delay.tv_nsec);
		nanosleep(&beat_delay, NULL);
		if (beatCount == 8) {
			beatCount = 0;
		}
	}
	AudioMixer_cleanup();
}

void Beat_init() {
	int delay = ((60 / (bpm / 2)) * 500);
	setMsDelay(delay);
	currentBeat = 2;
	playing = true;
}

void setMsDelay(int delayInMs) {
	long long temp = delayInMs * MS_TO_NS_FACTOR;
	beat_delay.tv_sec = temp / NSEC_MAX;
	beat_delay.tv_nsec = temp % NSEC_MAX;
}

