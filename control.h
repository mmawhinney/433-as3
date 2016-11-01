#ifndef CONTROL_H
#define CONTROL_H

#include <string.h>
#include <stdbool.h>

void Control_init();
void* control_thread(void* args);
void readFromFile(char* fileName, int pin, char* res);
void exportGpio(int pin);
void unexportGpio(int pin);
_Bool checkPin(int pin);
void setBeat(int beat);
int getTempo();
int getBeat();
int getVolume();
void increaseVolume(void);
void decreaseVolume(void);
void increaseTempo(void);
void decreaseTempo(void);
void cycleBeat(void);

#endif
