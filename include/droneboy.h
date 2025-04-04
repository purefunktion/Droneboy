#ifndef DRONEBOY_H
#define DRONEBOY_H

void tim(void);
void intro(void);
void init(void);

// keypad functions
void dutyKeypadController(void);
void volumeKeypadController(void);
void frequencyKeypadController(void);
void volumeKeypadController(void);
void chordKeypadController(void);

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
void changeControlPage(int to_page);
void changeToDutyBackground(void);
void changeToVolumeBackground(void);
void changeToFrequencyBackground(void);
void changeToChordBackground(void);

// goto credit page
void goToCreditPage(void);
void leaveCreditPage(void);

// blank tile
const unsigned char blank[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// duty page
void setUpSwitches(void);

#endif