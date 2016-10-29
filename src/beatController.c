#include <stdio.h>
#include <stdlib.h>
#include "audioMixer.h"

#define BASE_DRUM_HARD "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_HAT "wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "wave-files/100059__menegass__gui-drum-snare-soft.wav"

int bpm = 120;



int BeatController_getBPM() {
	return bpm;
}

void BeatController_setBPM(int newBpm) {
	bpm = newBpm;
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

