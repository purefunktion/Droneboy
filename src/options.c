#include "options.h"
#include "midi.h"
#include "globals.h"

static uint8_t controllerState = 0;

// navigation on options page
void optionsKeypadController(void) {
  if (KEY_PRESSED(J_UP)) {
    if (controllerState == 0) {
      increaseBPM();
    } else if (controllerState == 1) {
      increaseMidiChannel();
    } else if (controllerState == 2) { // sweep pan
      if (sweep_pan < 2) {
        sweep_pan++;
      } else {
        sweep_pan = 0;
      }
      setPanning(SWEEP);
      printPanning();
    } else if (controllerState == 3) { // square pan
      if (square_pan < 2) {
        square_pan++;
      } else {
        square_pan = 0;
      }
      setPanning(SQUARE);
      printPanning();
    } else if (controllerState == 4) { // wave pan
      if (wave_pan < 2) {
        wave_pan++;
      } else {
        wave_pan = 0;
      }
      setPanning(WAVE);
      printPanning();
    } else if (controllerState == 5) { // noise pan
      if (noise_pan < 2) {
        noise_pan++;
      } else {
        noise_pan = 0;
      }
      setPanning(NOISE);
      printPanning();
    }
    waitpadup();
  } else if (KEY_PRESSED(J_DOWN)) {
    if (controllerState == 0) {
      decreaseBPM();
    } else if (controllerState == 1) {
      decreaseMidiChannel();
    } else if (controllerState == 2) {
      if (sweep_pan > 0) {
        sweep_pan--;
      } else {
        sweep_pan = 2;
      }
      setPanning(SWEEP);
      printPanning();
    } else if (controllerState == 3) {
      if (square_pan > 0) {
        square_pan--;
      } else {
        square_pan = 2;
      }
      setPanning(SQUARE);
      printPanning();
    } else if (controllerState == 4) {
      if (wave_pan > 0) {
        wave_pan--;
      } else {
        wave_pan = 2;
      }
      setPanning(WAVE);
      printPanning();
    } else if (controllerState == 5) {
      if (noise_pan > 0) {
        noise_pan--;
      } else {
        noise_pan = 2;
      }
      setPanning(NOISE);
      printPanning();
    }
    waitpadup();
  }
  // move marker among options
  if (KEY_PRESSED(J_RIGHT)) {
    if (controllerState >= 5) {
      controllerState = 0;
    } else {
      controllerState++;
    }
    printSelecta();
    waitpadup();
  } else if (KEY_PRESSED(J_LEFT)) {
    if (controllerState == 0) {
      controllerState = 5;
    } else {
      controllerState--;
    }
    printSelecta();
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
  clearCounterValuesBPM(10, 5, 0);
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

// Print function for bpm
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

/*
* This sets a blank tile where there is no number.
*/
void clearCounterValuesBPM(uint8_t x, uint8_t y, uint8_t which) {
    int val;
    if (which == 0) { //BPM
        val = bpm;
    } else { // or midi channel number
        val = midiChannel + 1; // midi is displayed as starting from 1
    }
    if (val < 100) { // bpm has max 3 digits
        set_bkg_tile_xy(x-2, y, 4);
    }
    if (val < 10) {
        set_bkg_tile_xy(x-1, y, 4);
    }
}

// indicate if chordsteppa is on or off
void printChordSteppaOnOff(void) {
  if (current_state == OPTIONS_PAGE) {
    if (play_chord_step == 0) {
      set_bkg_tile_xy(5, 3, 0x49); // stop tile
    } else {
      set_bkg_tile_xy(5, 3, 0x47); // play tile
    }
  }
}

// this is to indicate the BPM speed in top right corner
void blinkBPM(void) {
  if (current_state == OPTIONS_PAGE) {
    if(bpm_blink_state == 0) {
      set_bkg_tile_xy(0x13, 0x00, 0x46);
      bpm_blink_state = 1;
    } else {
      set_bkg_tile_xy(0x13, 0x00, 0x47);
      bpm_blink_state = 0;
    }
  }
}

// Show which of bpm or midi is affected by up down
void printSelecta(void) {
  set_bkg_tile_xy(17, 14, 0x04); // Blank
  set_bkg_tile_xy(12, 5, 0x04); // Blank
  set_bkg_tile_xy(12, 7, 0x04); // Blank
  set_bkg_tile_xy(7, 10, 0x04); // Blank
  set_bkg_tile_xy(17, 10, 0x04); // Blank
  set_bkg_tile_xy(7, 14, 0x04); // Blank


  if (controllerState == 0) { // BPM
    set_bkg_tile_xy(12, 5, 0x4A); // Stop sign
  } else if (controllerState == 1) { // Midi
    set_bkg_tile_xy(12, 7, 0x4A); // Stop sign
  } else if (controllerState == 2) { // SWEEP panning
    set_bkg_tile_xy(7, 10, 0x4A); // Stop sign
  } else if (controllerState == 3) { // SQUARE panning
    set_bkg_tile_xy(17, 10, 0x4A); // Stop sign
  } else if (controllerState == 4) { // WAVE panning
    set_bkg_tile_xy(7, 14, 0x4A); // Stop sign
  } else if (controllerState == 5) { // NOISE panning
    set_bkg_tile_xy(17, 14, 0x4A); // Stop sign
  }
}

// set panning on channel
void setPanning(uint8_t channel) {
  uint8_t reg51 = NR51_REG;
  switch (channel) {
    case SWEEP: {
      if (sweep_pan == 0) { // Stereo
        NR51_REG = (NR51_REG | (1 << 0)) | (1 << 4);
      } else if (sweep_pan == 1) { // Hard Right
        NR51_REG = (NR51_REG | (1 << 0)) & ~(1 << 4);
      } else if (sweep_pan == 2) { // Hard Left
        NR51_REG = (NR51_REG & ~(1 << 0)) | (1 << 4);
      }
      break;
    }
    case SQUARE: {
      if (square_pan == 0) { // Stereo
        NR51_REG = (NR51_REG | (1 << 1)) | (1 << 5);
      } else if (square_pan == 1) { // Hard Right
        NR51_REG = (NR51_REG | (1 << 1)) & ~(1 << 5);
      } else if (square_pan == 2) { // Hard Left
        NR51_REG = (NR51_REG & ~(1 << 1)) | (1 << 5);
      }
      break;
    }
    case WAVE: {
      if (wave_pan == 0) { // Stereo
        NR51_REG = (NR51_REG | (1 << 2)) | (1 << 6);
      } else if (wave_pan == 1) { // Hard Right
        NR51_REG = (NR51_REG | (1 << 2)) & ~(1 << 6);
      } else if (wave_pan == 2) { // Hard Left
        NR51_REG = (NR51_REG & ~(1 << 2)) | (1 << 6);
      }
      break;
    }
    case NOISE: {
      if (noise_pan == 0) { // Stereo
        NR51_REG = (NR51_REG | (1 << 3)) | (1 << 7);
      } else if (noise_pan == 1) { // Hard Right
        NR51_REG = (NR51_REG | (1 << 3)) & ~(1 << 7);
      } else if (noise_pan == 2) { // Hard Left
        NR51_REG = (NR51_REG & ~(1 << 3)) | (1 << 7);
      }
      break;
    }
    default:
      break;
  }
}

// Print out the panning letters
void printPanning(void) {
  if (sweep_pan == 0) { // Stereo
    set_bkg_tile_xy(4, 10, 0x07); // L
    set_bkg_tile_xy(5, 10, 0x0C); // R
  } else if (sweep_pan == 1) { // hard right
    set_bkg_tile_xy(4, 10, 0x04); // Blank
    set_bkg_tile_xy(5, 10, 0x0C); // R
  } else if (sweep_pan == 2) { // hard left
    set_bkg_tile_xy(4, 10, 0x07); // L
    set_bkg_tile_xy(5, 10, 0x04); // Blank
  }
  if (square_pan == 0) { // Stereo
    set_bkg_tile_xy(14, 10, 0x07); // L
    set_bkg_tile_xy(15, 10, 0x0C); // R
  } else if (square_pan == 1) { // hard right
    set_bkg_tile_xy(14, 10, 0x04); // Blank
    set_bkg_tile_xy(15, 10, 0x0C); // R
  } else if (square_pan == 2) { // hard left
    set_bkg_tile_xy(14, 10, 0x07); // L
    set_bkg_tile_xy(15, 10, 0x04); // Blank
  }
  if (wave_pan == 0) { // Stereo
    set_bkg_tile_xy(4, 14, 0x07); // L
    set_bkg_tile_xy(5, 14, 0x0C); // R
  } else if (wave_pan == 1) { // hard right
    set_bkg_tile_xy(4, 14, 0x04); // Blank
    set_bkg_tile_xy(5, 14, 0x0C); // R
  } else if (wave_pan == 2) { // hard left
    set_bkg_tile_xy(4, 14, 0x07); // L
    set_bkg_tile_xy(5, 14, 0x04); // Blank
  }
  if (noise_pan == 0) { // Stereo
    set_bkg_tile_xy(14, 14, 0x07); // L
    set_bkg_tile_xy(15, 14, 0x0C); // R
  } else if (noise_pan == 1) { // hard right
    set_bkg_tile_xy(14, 14, 0x04); // Blank
    set_bkg_tile_xy(15, 14, 0x0C); // R
  } else if (noise_pan == 2) { // hard left
    set_bkg_tile_xy(14, 14, 0x07); // L
    set_bkg_tile_xy(15, 14, 0x04); // Blank
  }
}
