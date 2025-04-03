#ifndef DUTY_H
#define DUTY_H

#include <gb/gb.h>
#include "definitions.h"
#include "common.h"

void increaseDuty();
void decreaseDuty();
void updateSweepDuty(UBYTE duty);
void updateSquareDuty(UBYTE duty);
void updateWaveDuty();
void updateNoiseDividingRatio(int number);

void increaseMacroDuty(int number);
void decreaseMacroDuty(int number);
void bPressedHandler();
void changNoiseCounterStep();
void changeWaveType();

extern const UBYTE dutyValues[4];
extern const UBYTE dutyFaderPosition[4];
extern const UBYTE dutyFaderPositionNoise[8];

#endif