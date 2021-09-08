#include <gb/gb.h>
#include "definitions.h"
#include "tilesandbackgrounds.h"
#include "globals.h"

void printCurrentChannelNote();
void flipHeader();

void increaseCurrentFreq(int amount);
void decreaseCurrentFreq(int amount);
void printCurrentChannelFrequency();
 
void frequencyMode();
void noteMode();

void increaseCurrentNote(int amount);
void decreaseCurrentNote(int amount);

int uwti(UWORD num);