#ifndef _BEAT_CONTROLLER_H_
#define _BEAT_CONTROLLER_H_

#define MAX_BPM 300
#define MIN_BPM 40

int BeatController_getBPM(); 
void BeatController_setBPM(int newBpm); 
void BeatController_playRockBeat(int beatCount, wavedata_t *hiHatFile, wavedata_t *bassFile, wavedata_t *snareFile);
void BeatController_playCustomBeat(int beatCount, wavedata_t *bassFile, wavedata_t *hiHatFile);


#endif