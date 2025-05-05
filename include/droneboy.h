#ifndef DRONEBOY_H
#define DRONEBOY_H

#include "globals.h"

void tim(void);
void intro(void);
void init(void);

void handleVolumePage(Event event);
void handleDutyPage(Event event);
void handleFreqPage(Event event);
void handleChordPage(Event event);
void handleCreditPage(Event event);
void handleBpmPage(Event event);

// keypad functions
void dutyKeypadController(void);
void volumeKeypadController(void);
void frequencyKeypadController(void);
void volumeKeypadController(void);
void chordKeypadController(void);
void bpmKeypadController(void);

// placement of the fader marker on freq page
const UBYTE faderMarkerFreqx[2][4] = {{13, 93, 13, 93},{1,1,1,1}};
const UBYTE faderMarkerFreqy[2][4] = {{53, 53, 117, 117},{1,1,1,1}};

// frequency page, numbers
void setUpFrequencySprites(void);
// helper function for above
void freqSpritesSetupHelper(int position, int freq_value, int note_value);
// Helper fun to move sprites
void freqSetupMoveHelper(int index, int x, int y);

// chord page setup sprites to visulize chords
void setupChordSprites(void);

// hide sprites you don't like
void hideSprites(int sprite_id, int num);

// flip through the pages
void changeToDutyBackground(void);
void changeToVolumeBackground(void);
void changeToFrequencyBackground(void);
void changeToChordBackground(void);
void changeToBPMBackground(void);

// goto credit page
void goToCreditPage(void);

// blank tile
const unsigned char blank[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// duty page
void setUpSwitches(void);

extern void load_wave(unsigned char *src);
void startStopChordStep(void);

#endif