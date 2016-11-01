#ifndef _BEAT_CONTROLLER_H_
#define _BEAT_CONTROLLER_H_

#define MAX_BPM 300
#define MIN_BPM 40

int BeatController_getBPM(); 
void BeatController_setBPM(int newBpm); 
void BeatController_playRockBeat(int beatCount, wavedata_t *hiHatFile, wavedata_t *bassFile, wavedata_t *snareFile);
void BeatController_playCustomBeat(int beatCount, wavedata_t *bassFile, wavedata_t *hiHatFile);
int BeatController_getCurrentBeat(void);
void BeatController_setBeat(int beat);
int BeatController_getBeat();
void BeatController_cycleCurrentBeat(void);
void BeatController_playHiHat(wavedata_t *hiHatFile);
void BeatController_playBass(wavedata_t *bassFile);
void BeatController_playSnare(wavedata_t *snareFile);

#endif
