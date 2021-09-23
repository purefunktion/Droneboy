#include <gb/gb.h>
#include "definitions.h"
#include "tilesandbackgrounds.h"
#include "common.h"

void increaseVolume();
void decreaseVolume();

void updateSweepVolume(int volume);
void updateSquareVolume(int volume);
void updateNoiseVolume(UBYTE volume);

void increaseMacroVolume(int number);
void decreaseMacroVolume(int number);

const UBYTE wave_volumeValues[4] = {0x00, 0x60, 0x40, 0x20};
const UBYTE volumeValues[16] = {0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240};
const UBYTE volumeFaderPositionWave[4] = {119, 98, 71, 41};
