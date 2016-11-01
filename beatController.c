#include <stdio.h>
#include <stdlib.h>
#include "audioMixer.h"

#define BASE_DRUM_HARD "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_HAT "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
#define NUM_BEATS 3

int bpm = 120;
int currentBeat = 1;


int BeatController_getBPM() {
	return bpm;
}

void BeatController_setBPM(int newBpm) {
	bpm = newBpm;
}

int BeatController_getCurrentBeat() {
	return currentBeat;
}

void BeatController_setBeat(int beat){
	currentBeat = beat;
}

int BeatController_getBeat() {
	return currentBeat;
}

void BeatController_cycleCurrentBeat() {
	int newBeat = currentBeat + 1;
	if(newBeat > NUM_BEATS-1) {
		newBeat = 0;
	}
	currentBeat = newBeat;
}

void BeatController_playRockBeat(int beatCount, wavedata_t *hiHatFile, wavedata_t *bassFile, wavedata_t *snareFile) {
	AudioMixer_readWaveFileIntoMemory(HI_HAT, hiHatFile);
	if (beatCount == 0 || beatCount == 4) {
		AudioMixer_readWaveFileIntoMemory(BASE_DRUM_HARD, bassFile);
	} else if (beatCount == 2 || beatCount == 6) {
		AudioMixer_readWaveFileIntoMemory(SNARE, snareFile);
	}
}

void BeatController_playCustomBeat(int beatCount, wavedata_t *bassFile, wavedata_t *hiHatFile) {
	if (beatCount == 0 || beatCount == 1 || beatCount == 4
					|| beatCount == 5) {
		AudioMixer_readWaveFileIntoMemory(BASE_DRUM_HARD, bassFile);
	} else if (beatCount == 2 || beatCount == 6) {
		AudioMixer_readWaveFileIntoMemory(HI_HAT, hiHatFile);
	}
}

void BeatController_playHiHat(wavedata_t *hiHatFile) {
	AudioMixer_readWaveFileIntoMemory(HI_HAT, hiHatFile);
}

void BeatController_playBass(wavedata_t *bassFile) {
	AudioMixer_readWaveFileIntoMemory(BASE_DRUM_HARD, bassFile);
}

void BeatController_playSnare(wavedata_t *snareFile) {
	AudioMixer_readWaveFileIntoMemory(SNARE, snareFile);
}
