#include <gb/gb.h>
#include "definitions.h"
#include "globals.h"

void increaseDuty();
void decreaseDuty();
void updateDuty(UBYTE duty);
void updateWaveDuty(int duty);

extern const UBYTE dutyValues[4];
extern const UBYTE dutyFaderPosition[4];