#ifndef DRONEBOY_H
#define DRONEBOY_H

void intro();
void init();

// wave tables, not really used
typedef enum {
  SQUARE,
  TRIANGLE,
  TEST
} instrument;

// load the wave register pattern data
void loadWave(int wave);

// keypad functions
void dutyKeypadController();
void volumeKeypadController();
void frequencyKeypadController();
void volumeKeypadController();

// placement of the fader marker on freq page
const UBYTE faderMarkerFreqx[2][4] = {{13, 93, 13, 93},{1,1,1,1}};
const UBYTE faderMarkerFreqy[2][4] = {{53, 53, 117, 117},{1,1,1,1}};

// frequency page, numbers
void setUpFrequencySprites();
// frequency page, notes
void setNoteSprites(int position, int note_value);

// hide sprites you don't like
void hideSprites(int sprite_id, int num);

// flip through the pages
void changeControlPage(int to_page);
void changeToDutyBackground();
void changeToVolumeBackground();
void changeToFrequencyBackground();

// goto credit page
void goToCreditPage();
void leaveCreditPage();

// blank tile
const unsigned char blank[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// duty page
void setCounterStepSwitch();

#endif