#include "bpm.h"

// navigation on bpm page
void bpmKeypadController(void) {
  if (KEY_PRESSED(J_UP)) {
    increaseBPM();
    waitpadup();
  } else if (KEY_PRESSED(J_DOWN)) { 
    decreaseBPM();
    waitpadup();
  }
}

// increase BPM by 1, 240 bpm limit
void increaseBPM(void) {
  if (bpm != 240) {
    setBpm(bpm+1);   
  }
  printBPM();
}

// decrease BPM by 1, 1 bpm limit
void decreaseBPM(void) {
  if (bpm != 1) {
    setBpm(bpm-1);
  }
  printBPM();
}

// set the BPM
void setBpm(uint16_t new_bpm) {
  bpm = new_bpm;
  bpm_in_cycles = bpm_cycles_lut[bpm - 1];
}

// start the chord sequencer
void startStopChordStep(void) {
  play_chord_step = (play_chord_step == 1) ? 0 : 1;
  printChordSteppaOnOff();
}

// print function for frequency mode
void printBPM(void) {
  int val = bpm;
  int x_pos = 10;
  wait_vbl_done();
  while (val > 0) {
    int digit = val % 10;
    set_bkg_tile_xy(x_pos, 5, digit + 21); // 21 is offset from where digits are in mem
    val /= 10;
    x_pos--;
  }
}

// indicate if chordsteppa is on or off
void printChordSteppaOnOff(void) {
  if (current_state == BPM_PAGE) {
    if (play_chord_step == 0) {
      set_bkg_tile_xy(5, 3, 0x49); // stop tile
    } else {
      set_bkg_tile_xy(5, 3, 0x47); // play tile
    }
  }
}

// this is to indicate the BPM speed in top right corner
void blinkBPM(void) {
  if (current_state == BPM_PAGE) {
    if(bpm_blink_state == 0) {
      set_bkg_tile_xy(0x13, 0x00, 0x46);
      bpm_blink_state = 1;
    } else {
      set_bkg_tile_xy(0x13, 0x00, 0x47);
      bpm_blink_state = 0;
    }
  }
}