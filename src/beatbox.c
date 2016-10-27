#include <stdio.h>
#include <stdlib.h>
#include "audioMixer.h"

#define BASE_DRUM "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_HAT "wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "wave-files/100059__menegass__gui-drum-snare-soft.wav"

int main() {

	AudioMixer_init();
	// Load wave file we want to play:
	wavedata_t sampleFile;
	wavedata_t otherFile;
	AudioMixer_readWaveFileIntoMemory(BASE_DRUM, &sampleFile);
	AudioMixer_readWaveFileIntoMemory(HI_HAT, &otherFile);

	AudioMixer_cleanup();
}
