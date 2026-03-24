#ifndef DUTY_H
#define DUTY_H

#include <gb/gb.h>
#include "definitions.h"
#include "common.h"

void increaseDuty(void);
void decreaseDuty(void);
void updateSweepDuty(uint8_t duty);
void updateSquareDuty(uint8_t duty);
void updateWaveDuty(void);
void updateNoiseDividingRatio(int number);

void increaseMacroDuty(int number);
void decreaseMacroDuty(int number);
void bPressedHandler(void);
void changNoiseCounterStep(void);
void changeWaveType(void);

extern const uint8_t dutyValues[4];
extern const uint8_t dutyFaderPosition[4];
extern const uint8_t dutyFaderPositionNoise[8];

#endif