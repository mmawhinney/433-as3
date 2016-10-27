#include <stdio.h>
#include <stdlib.h>
#include "audioMixer.h"

#define SOURCE_FILE "100060__menegass__gui-drum-splash-hard.wav"

int main() {

	AudioMixer_init();
	// Load wave file we want to play:
	wavedata_t sampleFile;
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE, &sampleFile);

	AudioMixer_cleanup();
}
