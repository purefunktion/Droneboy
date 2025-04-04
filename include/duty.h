#ifndef DUTY_H
#define DUTY_H

#include <gb/gb.h>
#include "definitions.h"
#include "common.h"

void increaseDuty(void);
void decreaseDuty(void);
void updateSweepDuty(UBYTE duty);
void updateSquareDuty(UBYTE duty);
void updateWaveDuty(void);
void updateNoiseDividingRatio(int number);

void increaseMacroDuty(int number);
void decreaseMacroDuty(int number);
void bPressedHandler(void);
void changNoiseCounterStep(void);
void changeWaveType(void);

extern const UBYTE dutyValues[4];
extern const UBYTE dutyFaderPosition[4];
extern const UBYTE dutyFaderPositionNoise[8];

#endif