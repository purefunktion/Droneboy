#include <gb/gb.h>
#include "definitions.h"
#include "common.h"

void increaseDuty();
void decreaseDuty();
void updateSweepDuty(UBYTE duty);
void updateSquareDuty(UBYTE duty);
void updateWaveDuty(int duty);

void increaseMacroDuty(int number);
void decreaseMacroDuty(int number);

extern const UBYTE dutyValues[4];
extern const UBYTE dutyFaderPosition[4];