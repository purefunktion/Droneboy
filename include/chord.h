// chord header file
#ifndef CHORD_H
#define CHORD_H

#include <gb/gb.h>
#include "definitions.h"
#include "globals.h"
#include "tilesandbackgrounds.h"
#include "common.h"

void changeRootNote(BYTE direction, int num);
void playCurrentChord(void);
void changeNotes(void);
void changeChordPart(BYTE direction);
void chordPartRouter(BYTE direction, int num);
void changeMinorMajor(BYTE direction);
void changeAugDimNorm(BYTE direction);
void setMinorMajorSprites(int major_minor);
void chordChangeMode(void);
void chordSteppaMode(void);
void stepChordSteppa(BYTE direction);
void saveCurrentStep(void);
void playCurrentStep(void);
void printCurrentStep(int step);
void setNoteSpritesBg(int x, int y, int number);
void printWithHash(int cp, int step);
void printWithoutHash(int cp, int step);
void printMinMajAugDim(int step);
void playCurrentSeqStep(void);
void chordStepRecordRouter(BYTE direction, int num);

#endif