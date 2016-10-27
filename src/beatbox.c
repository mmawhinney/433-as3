#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "audioMixer.h"

#define BASE_DRUM "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_HAT "wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "wave-files/100059__menegass__gui-drum-snare-soft.wav"

const unsigned int MS_TO_NS_FACTOR = 1000000;
const unsigned int NSEC_MAX = 1000000000;

void setMSDelay(int delayInMs, struct timespec delay);
int main() {
	AudioMixer_init();
	// Load wave file we want to play:
	// time for half beat
	/*If you were coding this, you might have a loop that continuously repeats. Each pass through the
	 loop corresponds to a ½ beat (which is an eighth note, and one row in the above table). The loop
	 first plays any needed sound(s) and then waits for the duration of half a beat time.
	 The amount of time to wait for half a beat is:
	 Time For Half Beat [sec] = 60 [sec/min] / BPM / 2 [half-beats per beat]
	 If you want the delay in milliseconds, multiply by 1,000.*/
	int bpm = 60;
	int beat_count = 0;
	int i = 0;
	struct timespec beat;
	beat.tv_sec = 1;
	beat.tv_nsec = 10000000;
	while (i < 30) {

		wavedata_t otherFile;
		AudioMixer_readWaveFileIntoMemory(HI_HAT, &otherFile);

		if (beat_count == 0 || beat_count == 4) {
			wavedata_t sampleFile;
			AudioMixer_readWaveFileIntoMemory(BASE_DRUM, &sampleFile);
		} else if (beat_count == 2 || beat_count == 6) {
			wavedata_t lastFile;
			AudioMixer_readWaveFileIntoMemory(SNARE, &lastFile);
		}
		int bpm_sleep = ((60 * bpm) / 2) * 1000;
		printf("%d, ", bpm_sleep);
//		setMSDelay(bpm_sleep, beat);
		beat_count++;
		nanosleep(&beat, NULL);
		if (beat_count == 8) {
			beat_count = 0;
		}
		i++;
	}
	AudioMixer_cleanup();
}

void setMSDelay(int delayInMs, struct timespec delay) {
	long temp = delayInMs * MS_TO_NS_FACTOR;
	delay.tv_sec = temp / NSEC_MAX;
	delay.tv_nsec = temp % NSEC_MAX;
}
