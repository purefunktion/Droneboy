#ifndef DRONEBOY_H
#define DRONEBOY_H

void tim();
void intro();
void init();

// keypad functions
void dutyKeypadController();
void volumeKeypadController();
void frequencyKeypadController();
void volumeKeypadController();
void chordKeypadController();

// placement of the fader marker on freq page
const UBYTE faderMarkerFreqx[2][4] = {{13, 93, 13, 93},{1,1,1,1}};
const UBYTE faderMarkerFreqy[2][4] = {{53, 53, 117, 117},{1,1,1,1}};

// frequency page, numbers
void setUpFrequencySprites();
// chord page setup sprites to visulize chords
void setupChordSprites();

// hide sprites you don't like
void hideSprites(int sprite_id, int num);

// flip through the pages
void changeControlPage(int to_page);
void changeToDutyBackground();
void changeToVolumeBackground();
void changeToFrequencyBackground();
void changeToChordBackground();

// goto credit page
void goToCreditPage();
void leaveCreditPage();

// blank tile
const unsigned char blank[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// duty page
void setUpSwitches();

#endif