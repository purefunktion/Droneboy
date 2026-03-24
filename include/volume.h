#ifndef VOLUME_H
#define VOLUME_H

#include <gb/gb.h>
#include "definitions.h"
#include "tilesandbackgrounds.h"
#include "common.h"

void increaseVolume(int number);
void decreaseVolume(int number);

void updateSweepVolume(int volume);
void updateSquareVolume(int volume);
void updateNoiseVolume(uint8_t volume);
void updateWaveToBeLoaded(int volume, int sample_index);
void loadSquareWaveLow(int volume, int sample_index);
void loadSquareWaveHigh(int volume, int sample_index);
void loadSawWaveLow(int volume);
void loadSawWaveHigh(int volume);
void loadTriangleWaveLow(int volume);
void loadTriangleWaveHigh(int volume);
void loadSineWaveLow(int volume);
void loadSineWaveHigh(int volume);
void loadRampWaveLow(int volume);
void loadRampWaveHigh(int volume);

void increaseMacroVolume(int number);
void decreaseMacroVolume(int number);

extern const uint8_t wave_volumeValues[4];
extern const uint8_t volumeFaderPositionWave[4];

#endif