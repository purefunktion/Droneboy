// chord header file
#ifndef CHORD_H
#define CHORD_H

#include <gb/gb.h>
#include "definitions.h"
#include "globals.h"
#include "tilesandbackgrounds.h"
#include "common.h"

void changeRootNote(uint8_t direction, int num);
void playCurrentChord(void);
void changeNotes(void);
void changeChordPart(uint8_t direction);
void chordPartRouter(uint8_t direction, int num);
void changeMinorMajor(uint8_t direction);
void changeAugDimNorm(uint8_t direction);
void setMinorMajorSprites(int major_minor);
void chordChangeMode(void);
void chordSteppaMode(void);
void stepChordSteppa(int8_t direction);
void saveCurrentStep(void);
void playCurrentStep(void);
void printCurrentStep(int step);
void setNoteSpritesBg(int x, int y, int number);
void printWithHash(int cp, int step);
void printWithoutHash(int cp, int step);
void printMinMajAugDim(int step);
void playCurrentSeqStep(void);
void chordStepRecordRouter(uint8_t direction, int num);
void setAugDimNormSprites(void);
void setCurrentChordSteppaStep(int new_step);

#endif