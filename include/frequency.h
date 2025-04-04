#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <gb/gb.h>
#include "definitions.h"
#include "tilesandbackgrounds.h"
#include "common.h"

void increaseCurrentFreq(int amount);
void decreaseCurrentFreq(int amount);
void printChannelFrequency(int channel);
void printChannelNote(int channel);

void frequencyMode(void);
void noteMode(void);

void increaseCurrentNote(int amount);
void decreaseCurrentNote(int amount);

void increaseMacroFreq(int number);
void decreaseMacroFreq(int number);
void increaseMacroNote(int number);
void decreaseMacroNote(int number);

// copy the note freq to the freq global
void copyNoteToFreq(void);

int uwti(UWORD num);

#endif