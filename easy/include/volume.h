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
void updateNoiseVolume(UBYTE volume);
void updateWaveToBeLoaded(int volume, int sample_index);
void loadSquareWave(int volume, int sample_index);
void loadSawWave(int volume);
void loadTriangleWave(int volume);
void loadSineWave(int volume);
void loadRampWave(int volume);

void increaseMacroVolume(int number);
void decreaseMacroVolume(int number);

const UBYTE wave_volumeValues[4] = {0x00, 0x60, 0x40, 0x20};
const UBYTE volumeFaderPositionWave[4] = {119, 98, 71, 41};

#endif