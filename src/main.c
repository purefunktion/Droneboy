#include <gb/gb.h>
#include "droneboy.h"
#include "tilesandbackgrounds.h"
#include "definitions.h"
#include "globals.h"

// Declaration of global variables
BYTE keys = 0;
UBYTE previous_keys;

int max_faders = 4;
int num_faders = 4;

int sweep_note = 36; // Tone channel 1 is called sweep
int square_note = 40; // Tone channel 2 is called square
int wave_note = 43; // Channel 3 Wave Output is called wave
int noise_note = 0; // And noise channel 4 
UWORD sweep_freq = 262;  
UWORD square_freq = 850;
UWORD wave_freq = 1002;
UBYTE noise_freq = 0;
int sweep_volume = 0;
int sweep_up_down_flag = 1; //going up
int square_volume = 0;
int wave_volume = 0;
int noise_volume = 0;

int current_channel = 0;
int credit_page = 0;
int frequency_mode = 0;
UINT8 chord_mode = 0;
UINT8 num_control_pages = 4;
int active_control_page = 0;
struct NoiseyStruct noiseStruct;
int current_chord_step = 0;
int current_chord_steppa_step = 0;
int current_record_steppa_step = 0;

// continuous sweep of freq and volume counters
int up_sweep_counter = 0;
int down_sweep_counter = 0;
int up_volume_counter = 0;
int down_volume_counter = 0;
int volume_slide_counter = 0;

// this are marker positions
struct fader fader_group[4];
struct fader duty_fader_group[4];
struct fader chord_part_step[4];
struct fader chord_steppa_step[8];

// duty 
int duty_sweep = 2;
int duty_square = 2;
int duty_wave = 2;

enum WAVES wave_type = SQUAREWAVE;

const UBYTE dutyValues[4] = {0x00, 0x40, 0x80, 0xC0};
const UBYTE dutyFaderPosition[4] = {111, 89, 65, 41};
const UBYTE dutyFaderPositionNoise[8] = {111, 100, 89, 80, 73, 65, 53, 41};

// Macro markers
struct MacroStatus volumeMacroStatus;
struct MacroStatus dutyMacroStatus;
struct MacroStatus freqMacroStatus;
int domacro = 0;

// chord page A-button state
int doPlayCurrentChord = 0;
// chord page B-button state
int doSetCurrentStep = 0;

// the chord steppa init
struct ChordStep chordsteppa[8];
// play the chord step sequencer, 0=off,1=on
BYTE play_chord_step = 0;
// number of beats per step in sequencer
UINT8 beats_per_step = 1;
UINT8 beats_counter = 0;
int current_seq_chord = 0;

// BPM globals
unsigned int tim_cnt = 0;
int bpm_in_cycles = 2048;
int bpm = 120;


// Note names to display
const char noteNames[72][5] = {
  " C 3", "C# 3", " D 3", "D# 3", " E 3", " F 3", "F# 3", " G 3", "G# 3", " A 3", "A# 3", " B 3",
  " C 4", "C# 4", " D 4", "D# 4", " E 4", " F 4", "F# 4", " G 4", "G# 4", " A 4", "A# 4", " B 4",
  " C 5", "C# 5", " D 5", "D# 5", " E 5", " F 5", "F# 5", " G 5", "G# 5", " A 5", "A# 5", " B 5",
  " C 6", "C# 6", " D 6", "D# 6", " E 6", " F 6", "F# 6", " G 6", "G# 6", " A 6", "A# 6", " B 6",
  " C 7", "C# 7", " D 7", "D# 7", " E 7", " F 7", "F# 7", " G 7", "G# 7", " A 7", "A# 7", " B 7",
  " C 8", "C# 8", " D 8", "D# 8", " E 8", " F 8", "F# 8", " G 8", "G# 8", " A 8", "A# 8", " B 8"
};
// the UBYTE for polynomial counter when doing notes on noise channel
const UBYTE noiseNotesFrequencies[] = {
   0X2F, 0x1F, 0x2D, 0x0F, 0x1D, 0x0D
 };
//  0x0B, 0x0A
// noise note name, index in the noteName array above
const int noiseNoteNameIndex[] = {
   2, 14, 20, 26, 32, 44
};
// 53, 60
// GB frequencies matching notes in noteNames
// taken from http://www.devrs.com/gb/files/sndtab.html
const UWORD frequencies[] = {
  44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
  1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
  1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
  1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
  1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
  1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015
};
// Wave table. Different square duties to match other square channels 12.5%, 25%, 50% and 75%.
const UBYTE squareSamples[] = {
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,  //fiddy fiddy
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
};

// half total wave ram length. this is so it will fit more easily
// with the frequency of the other square channels.
// the period is 16(nibbles hence 8 bytes in a row) but wave ram is 32 
const UBYTE sawSamples[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // silence not used
  0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00,
  0x33, 0x32, 0x22, 0x22, 0x11, 0x11, 0x10, 0x00,
  0x44, 0x33, 0x33, 0x22, 0x22, 0x11, 0x11, 0x00,
  0x55, 0x44, 0x43, 0x33, 0x22, 0x21, 0x11, 0x00,
  0x66, 0x55, 0x44, 0x43, 0x32, 0x22, 0x11, 0x00,
  0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
  0x87, 0x76, 0x65, 0x54, 0x43, 0x32, 0x21, 0x10,
  0x98, 0x87, 0x76, 0x55, 0x44, 0x32, 0x21, 0x10,
  0xA9, 0x98, 0x77, 0x65, 0x54, 0x33, 0x21, 0x10,
  0xBA, 0xA9, 0x87, 0x76, 0x54, 0x43, 0x21, 0x10,
  0xCB, 0xAA, 0x98, 0x76, 0x65, 0x43, 0x22, 0x10,
  0xDC, 0xBA, 0xA9, 0x87, 0x65, 0x43, 0x32, 0x10,
  0xED, 0xCB, 0xA9, 0x87, 0x76, 0x54, 0x32, 0x10,
  0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, // full volume
};

//samples for triangle wave
const UBYTE triangleSamples[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x01,0x11,0x11,0x11,0x11,0x00,0x00,
  0x01,0x11,0x12,0x22,0x22,0x11,0x11,0x00,
  0x01,0x12,0x22,0x33,0x32,0x22,0x11,0x00,
  0x11,0x22,0x33,0x44,0x43,0x32,0x21,0x10,
  0x11,0x23,0x34,0x45,0x44,0x33,0x21,0x10,
  0x12,0x23,0x45,0x56,0x55,0x43,0x22,0x10,
  0x12,0x34,0x45,0x67,0x65,0x44,0x32,0x10,
  0x12,0x34,0x56,0x78,0x76,0x54,0x32,0x10,
  0x12,0x35,0x67,0x89,0x87,0x65,0x32,0x10,
  0x13,0x45,0x68,0x9A,0x98,0x65,0x43,0x10,
  0x13,0x46,0x78,0xAB,0xA8,0x76,0x43,0x10,
  0x23,0x56,0x89,0xBC,0xB9,0x86,0x53,0x20,
  0x23,0x57,0x8A,0xBD,0xBA,0x87,0x53,0x20,
  0x24,0x57,0x9B,0xCE,0xCB,0x97,0x54,0x20,
  0x24,0x68,0x9B,0xDF,0xDB,0x98,0x64,0x20,
};

// sine wave LUT
const UBYTE sineSamples[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,
  0x11,0x22,0x22,0x21,0x11,0x00,0x00,0x01,
  0x22,0x33,0x33,0x32,0x21,0x00,0x00,0x01,
  0x23,0x34,0x44,0x33,0x21,0x10,0x00,0x11,
  0x33,0x45,0x55,0x43,0x32,0x10,0x00,0x12,
  0x34,0x56,0x66,0x54,0x32,0x10,0x00,0x12,
  0x45,0x67,0x77,0x65,0x42,0x10,0x00,0x12,
  0x46,0x78,0x88,0x76,0x42,0x10,0x00,0x12,
  0x56,0x89,0x99,0x86,0x53,0x10,0x00,0x13,
  0x57,0x9A,0xAA,0x97,0x53,0x10,0x00,0x13,
  0x68,0x9B,0xBB,0x98,0x63,0x20,0x00,0x23,
  0x68,0xAC,0xCC,0xA8,0x64,0x20,0x00,0x24,
  0x79,0xBD,0xDD,0xB9,0x74,0x20,0x00,0x24,
  0x7A,0xCD,0xED,0xCA,0x74,0x21,0x01,0x24,
  0x8A,0xDE,0xFE,0xDA,0x85,0x21,0x01,0x25,
};

// intial wave
UBYTE waveToBeLoaded[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,};

// volume values for the "zombie" mode. https://gbdev.gg8.se/wiki/articles/Gameboy_sound_hardware#Obscure_Behavior
const UBYTE volumeValues[16] = {0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x78, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8};
// y coordinates for the volume fader(16 steps)
const UBYTE volumeFaderPosition[16] = {119, 114, 109, 104, 98, 93, 89, 85, 80, 76, 71, 67, 58, 54, 49, 41};

// state of the navigation 
int right_pressed = 0; // select + right goes right(duh)
int left_pressed = 0; // read above but left

// Main 
void main(void) {
  
  CRITICAL {
      tim_cnt = 0;
      add_TIM(tim);
  }

   // Set TMA to divide clock by 0x100
  TMA_REG = 0xFFU;
  // Set clock to 4096 Hertz 
  TAC_REG = 0x04U;
  // Handle VBL and TIM interrupts
  set_interrupts(VBL_IFLAG | TIM_IFLAG);
  init();
  
  // Main loop
  while(1) {
    // Change control page
    if (KEY_PRESSED(J_START)) {
      credit_page = (credit_page == 1) ? 0 : 1; // credit page flipper
      if (credit_page == 1) {
        goToCreditPage();
      } else {
        leaveCreditPage();
      }
      waitpadup();
    }
    if (KEY_PRESSED(J_SELECT)) { // so if select is currently pressed
      if (right_pressed == 1) { // right has been pressed
        if (!KEY_PRESSED(J_RIGHT)) {
          if (active_control_page == num_control_pages - 1) { // minus one because array index
            active_control_page = 0;
          } else {
            active_control_page = active_control_page + 1;
          }
          changeControlPage(active_control_page);
          right_pressed = 0; // reset select+right state
        }
      } else {
        if (KEY_PRESSED(J_RIGHT)) {
          right_pressed = 1;
        }
      }
      if (left_pressed == 1) {
        if (!KEY_PRESSED(J_LEFT)) {
          if (active_control_page == 0) {
            active_control_page = num_control_pages - 1;
          } else {
            active_control_page = active_control_page - 1;
          }
          changeControlPage(active_control_page);
          left_pressed = 0; // reset select+left state
        }
      } else {
        if (KEY_PRESSED(J_LEFT)) {
          left_pressed = 1;
        }
      }
    } else {
      right_pressed = 0;
      left_pressed = 0;
    }
    // i not pressing select use navigation on current controle page
    if (credit_page != 1 && right_pressed != 1 && left_pressed != 1) {
      switch(active_control_page)
      {
        case 0: { // volume
          num_faders = 4;
          volumeKeypadController();
          break;
        }
        case 1: { // duty
          num_faders = 4;
          dutyKeypadController();
          break;
        }
        case 2: { // freq/note
          num_faders = 4;
          flipHeader();
          frequencyKeypadController();
          break;
        }
        case 3: { // chord
          num_faders = 1;
          chordKeypadController();
          break;
        }
      }
    }
    // Switch depending on which control page we are on
    wait_vbl_done();
    UPDATE_KEYS();
  }
}

// timer interrupt
void tim(void) {
  tim_cnt++;  
  if (tim_cnt == bpm_in_cycles) {
    blinkBPM();
    if (play_chord_step == 1) {
      playChordStep();
    }
    tim_cnt = 0;
  }
}

void setBpm(void) {
  bpm_in_cycles = 409600/((bpm*100)/60);
}

/*
* Flip between pages in the app
*/
void changeControlPage(int to_page) {
  switch(to_page) {
    case 0: { // volume
      changeToVolumeBackground();
      break;
    }
    case 1: {
      changeToDutyBackground();
      break;
    }
    case 2: {
      changeToFrequencyBackground();
      break;
    }
    case 3: {
      changeToChordBackground();
      break;
    }
  }
}

/*
* Change to the volume background.
*/
void changeToVolumeBackground(void) {
  set_bkg_data(0,4, fadertile); // all functions are "VRAM safe" albeit slow
  set_bkg_tiles(0x00, 0x00, 20, 18, volumefaderbackground);
  hideSprites(4, 36); // hide tiles from frequency page
  for (int i = 0; i <= max_faders-1; i++) {
    move_sprite(i, fader_group[i].x, fader_group[i].y);
  }
  // hide record marker from chord page
  move_sprite(39, 0, 0);
  updateFaderMarker();
  setAllVolumeMacroMarkers();
  domacro = 0;
}

/*
* This changes sprites/background to the duty page
* Duty is only available for sweep and square
*/
void changeToDutyBackground(void) {
  set_bkg_data(0,4, fadertile);
  set_bkg_tiles(0x00, 0x00, 20, 18, dutyfaderbackground);
  hideSprites(0, 39);
  // move the duty faders on screen
  for (int i = 0; i <= max_faders-1; i++) {
    if (i == 3) {
      duty_fader_group[i].y = dutyFaderPositionNoise[duty_fader_group[i].fader_position];
    }
    move_sprite(i, duty_fader_group[i].x, duty_fader_group[i].y);
  }
  updateFaderMarker();
  setAllDutyMacroMarkers();
  setUpSwitches();
}

/*
* Changes to the frequency background
*/
void changeToFrequencyBackground(void) {
  set_bkg_data(0,4, fadertile); // setup fader tiles
  set_bkg_tiles(0x00, 0x00, 20, 18, frequencybackground); // the bakground
  setUpFrequencySprites();
  // this hides the sprites from duty page
  for (int i = 0; i <= max_faders-1; i++) 
  {
    move_sprite(i, 1, 168);
  }
  move_sprite(39, 0, 0); // hide rec marker from chord
  updateFaderMarker();
  setAllFreqMacroMarkers();
}

/*
* Change to the chord background.
*/
void changeToChordBackground(void) {
  set_bkg_data(0,4, fadertile);
  set_bkg_tiles(0x00, 0x00, 20, 18, chordbackground);
  hideSprites(0, 39); // hide tiles from frequency page
  setupChordSprites();
  printCurrentSeq();
  updateFaderMarker();
  updateRecordMarker(); // record sprite 
  doPlayCurrentChord = 0;
  doSetCurrentStep = 0;
}

// credit page flipper
void goToCreditPage(void) {
  set_bkg_data(0,4, fadertile); // setup fader tiles
  set_bkg_tiles(0x00, 0x00, 20, 18, creditPageBackground);
  // move the duty faders on screen

  for (int i = 0; i <= max_faders-1; i++)
  {
    move_sprite(i, 1, 168);
  }
  move_sprite(37, 2, 168);
  move_sprite(38, 2, 168);
  hideSprites(0, 36);
}

// leave credit page
void leaveCreditPage(void) {
  changeControlPage(active_control_page);
}

// helper function for the setup of sprites
void freqSpritesSetupHelper(int position, int freq_value, int note_value) {
  setCounterSprites(position, freq_value); // setup the frequency tiles
  clearCounterValues(position, current_channel); // clear if needed
  setNoteSprites(position+16, note_value); // set the note tiles
}

// elper func to move sprites when going to frequency page
void freqSetupMoveHelper(int index, int x, int y) {
  int offset = 0;
  for (int i = index; i < index+4; ++i)
  {
    move_sprite(i, x-offset, y);
    offset += 8; // 1, 10, 100, 1000
  }
}

/*
* This is now cleaned up a bit
*/
void setUpFrequencySprites(void) {
  int temp_channel = current_channel;
  current_channel = SWEEP;
  freqSpritesSetupHelper(4, sweep_freq, sweep_note);

  current_channel = SQUARE;
  freqSpritesSetupHelper(8, square_freq, square_note);

  current_channel = WAVE;
  freqSpritesSetupHelper(12, wave_freq, wave_note);

  current_channel = NOISE;
  freqSpritesSetupHelper(16, noise_freq, noiseNoteNameIndex[noise_note]);
  current_channel = temp_channel;

  // This section positions the tiles used to display
  // notes or frequencies.
  // sweep
  freqSetupMoveHelper(4, 51, 57);
  freqSetupMoveHelper(20, 51, 67);

  // square
  freqSetupMoveHelper(8, 130, 57);
  freqSetupMoveHelper(24, 130, 67);

  // wave
  freqSetupMoveHelper(12, 51, 121);
  freqSetupMoveHelper(28, 51, 131);
  
  // noise
  freqSetupMoveHelper(16, 130, 121);
  freqSetupMoveHelper(32, 130, 131);
}

// background icons
void setUpSwitches(void) {
  if (noiseStruct.counter_step == 1) {
    set_bkg_tile_xy(0x11, 0x0F, 0x36);
    set_bkg_tile_xy(0x12, 0x0F, 0x37);
  } else {
    set_bkg_tile_xy(0x11, 0x0F, 0x35); // left flip area
    set_bkg_tile_xy(0x12, 0x0F, 0x38); // right flip area
  }
  // wave form icons
  switch(wave_type)
  {
    case SQUAREWAVE: {
      set_bkg_tile_xy(0x0D, 0x0F, 0x39);
      break;
    }
    case SAW: {
      set_bkg_tile_xy(0x0D, 0x0F, 0x3A);
      break;
    }
    case RAMP: {
      set_bkg_tile_xy(0x0D, 0x0F, 0x3D);
      break;
    }
    case TRIANGLE: {
      set_bkg_tile_xy(0x0D, 0x0F, 0x3B);
      break;
    }
    case SINE: {
      set_bkg_tile_xy(0x0D, 0x0F, 0x3C);
      break;
    }
  }
}

// set up the chord tiles to use
void setupChordSprites(void) {
  int x = 48; // coordinates
  int y = 56;
  //y = y + 10; // 10 pixels under the freq tiles
  move_sprite(20, x, y);
  move_sprite(21, x-8, y); // decrese by 8 to move left
  move_sprite(22, x-16, y);
  move_sprite(23, x-24, y);

  // record sprite for chord stepper
  move_sprite(39,16,152);

  // as these sprites are borrowed from frequency
  // we update them here once 
  printChordParts();
  // and when changing set the correct on/off indicator
  setOnOffSprites();
}

/*
* Moves sprites off screen in the range from sprite id up til num
*/
void hideSprites(int sprite_id, int num) {
  for (int i = sprite_id; i <= num-1; i++) {
    move_sprite(i, 1, 169+i);
  }
}

/**
* Flip between faders(channels) left/right 
*/
void change_fader(BYTE direction) {
  if (direction == J_RIGHT) {
    if (current_channel == num_faders - 1) {
      current_channel = 0;
    } else {
      current_channel += 1;
    }
  } else { //left
    if (current_channel == 0) {
      current_channel = num_faders - 1;
    } else {
      current_channel -= 1;
    }
  }
  updateFaderMarker();
}

/*
* This will move the fader marker to show the active channel.
*/
void updateFaderMarker(void) {
  if (active_control_page == 0) { // Volume
    move_sprite(37, fader_group[current_channel].x, 128);
    move_sprite(38, fader_group[current_channel].x, 136);
  } else if(active_control_page == 1) { // Duty
    move_sprite(37, fader_group[current_channel].x, 120);
    move_sprite(38, fader_group[current_channel].x, 128); 
  } else if(active_control_page == 2) { // Frequency
    if (frequency_mode == 0) { // Frequency number mode
      move_sprite(37, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]);
      move_sprite(38, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]+8);
    } else { // Note mode
      move_sprite(37, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]+10);
      move_sprite(38, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]+18);
    }
  } else if(active_control_page == 3) { // chord
    if (chord_mode == 0) { // Chord mode, steppa och chord change
      move_sprite(37, chord_part_step[current_chord_step].x, chord_part_step[current_chord_step].y);
      move_sprite(38, chord_part_step[current_chord_step].x, chord_part_step[current_chord_step].y + 8);
    } else { // chord steppa
      move_sprite(37, chord_steppa_step[current_chord_steppa_step].x, chord_steppa_step[current_chord_steppa_step].y);
      move_sprite(38, chord_steppa_step[current_chord_steppa_step].x, chord_steppa_step[current_chord_steppa_step].y + 8);
    }
  }
}

/*
* This will move the fader up/down(y axis), values from volumeFaderPosition
*/
void moveFader(int channel) {
  if (active_control_page == 0) {
    fader_group[channel].y = volumeFaderPosition[fader_group[channel].fader_position];
    move_sprite(channel, fader_group[channel].x, fader_group[channel].y);
  } else if (active_control_page == 1 ) {
    if (channel == 3) {
      duty_fader_group[channel].y = dutyFaderPositionNoise[duty_fader_group[channel].fader_position];
      move_sprite(channel, duty_fader_group[channel].x, duty_fader_group[channel].y);
    } else {
      duty_fader_group[channel].y = dutyFaderPosition[duty_fader_group[channel].fader_position];
      move_sprite(channel, duty_fader_group[channel].x, duty_fader_group[channel].y);
    }
  }
}

/*
* This will load a sample in to the wave channels wave pattern ram
* More or less copypasta from
* https://gbdev.gg8.se/forums/viewtopic.php?id=758
* and Nitro2k01 "antispike fix"
* https://blog.gg8.se/wordpress/2013/02/11/gameboy-project-week-6-can-i-have-an-a-men/
* Changed to use a the memory location instead of Look Up Table (LUT). 
*/
void loadWave(void) {
  UBYTE freqlow, freqhigh;
  // diffrent freq depending on note or freq mode
  if (frequency_mode == 0) {
    freqlow = (UBYTE)wave_freq & 0xFF; // lower byte of frquency
    freqhigh = (UBYTE)((wave_freq & 0x0700)>>8); // higher bits
  } else {
    freqlow = (UBYTE) frequencies[wave_note] & 0xFF;
    freqhigh = (UBYTE) ((frequencies[wave_note] & 0x0700)>>8);
  }
  NR51_REG = 0b10111011; // antispike
  // This next line must be done or wave ram will act weird see:
  // https://gbdev.gg8.se/wiki/articles/Gameboy_sound_hardware#Obscure_Behavior
  NR30_REG = 0x00; 
  unsigned char *dst = (unsigned char *)(0xFF30u); // Create pointer to the wave RAM.
  unsigned char *src = &waveToBeLoaded[0]; // Create pointer to the waveform.
  unsigned char length = 16; // Number of bytes to copy.
  while (length--) {
    *dst++ = *src++;
  }

  NR30_REG |= 0x80; // Enable wave channel.
  NR51_REG = 0b11111111; // antispike
  NR33_REG = freqlow; // Set lower byte of frequency.
  NR34_REG = 0x80 | freqhigh; // 0xC0 // Set higher byte of frequency and start playback.
}

/*
* Setup when starting the app.
*/
void init(void) {
  int csx = 0x01;
  // IMPORTANT! NR52_REG must always be set first
  // otherwise there will be no sound.
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-control-registers
  NR52_REG = 0x80; // Bit 7 all sounds on
  NR50_REG = 0x77; // don't need Vin  
  NR51_REG = 0xFF; // output all channels to S01 and SO2

  // sweep channel 
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-1---tone--sweep

  // Volume envelope
  NR12_REG = 0x08; //0x00 // 0=min volume, length ignored
  // Duty
  NR11_REG = 0x80; //0x80 // 50% square wave
  // Frequency
  updateSweepFreq(1);

  // square channel
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-2---tone
  NR22_REG = 0x00;
  NR21_REG = 0x80;
  updateSquareFreq(1);

  // wave channel
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-3---wave-output
  loadWave();

  // noise channel
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-4---noise
  noiseStruct.dividing_ratio = 7; // 3 bits
  noiseStruct.counter_step = 1; // 1 = 7bits, 0 = 15 bits
  noiseStruct.clock_freq = noise_freq; // 4 bits
  NR42_REG = 0x00;
  NR41_REG = 0;
  updateNoiseFreq(noise_freq);
  NR44_REG = 0x80;
  
  // setup the background data
  set_bkg_data(0,4, fadertile); 
  set_bkg_data(4,1, blank);
  set_bkg_data(5,16, pageheadertext);
  set_bkg_data(21,10, frequencytiles);
  set_bkg_data(31,8, icons);
  set_bkg_data(39,2, macroMarker);
  set_bkg_data(41,12, creditPageText);
  set_bkg_data(53, 4, noiseCounterStepFlip);
  set_bkg_data(57,17, waveforms);
  set_bkg_tiles(0,0,20,18, volumefaderbackground);

  SHOW_BKG;
  DISPLAY_ON;
  SPRITES_8x8;
  set_sprite_data(0, 4, fadertile);
  for (int i = 0; i < 4; ++i){
    set_sprite_tile(i, 0);
  }

  set_sprite_data(4, 20, frequencytiles);
  //set_sprite_data(1, 14, )
  //frequency numbers
  for (int i = 4; i < 24; i++) {
    set_sprite_tile(i, i-4);
  }
  // icons used on frequency page
  set_sprite_data(22, 8, icons);
  // frequency notes
  set_sprite_data(30, 16, notetext);
  for (int i = 20; i < 36; i++) {
    set_sprite_tile(i, 30);
  }
  // fader marker
  set_sprite_tile(37, 24);
  set_sprite_tile(38, 25);

  set_sprite_tile(39, 0x0E);

  // Volume faders
  for (int i = 0; i < 4; ++i) {
    fader_group[i].y = 119; // they start at the bottom
    fader_group[i].fader_position = 0; // volume starts at zero
    chord_part_step[i].y = 69;
    chord_part_step[i].fader_position = 0;
  }
  // from left to right 
  fader_group[SWEEP].x = 32;
  fader_group[SQUARE].x = 72;
  fader_group[WAVE].x = 112;
  fader_group[NOISE].x = 152;

  // chord step x positions, chord page
  chord_part_step[SWEEP].x = 48;
  chord_part_step[SQUARE].x = 56;
  chord_part_step[WAVE].x = 64;
  chord_part_step[NOISE].x = 72;

  // Duty faders
  // sweep
  duty_fader_group[SWEEP].x = 32;
  duty_fader_group[SWEEP].y = 65;
  duty_fader_group[SWEEP].fader_position = 2;
  // square
  duty_fader_group[SQUARE].x = 72;
  duty_fader_group[SQUARE].y = 65;
  duty_fader_group[SQUARE].fader_position = 2;
  //wave
  duty_fader_group[WAVE].x = 112;
  duty_fader_group[WAVE].y = 65;
  duty_fader_group[WAVE].fader_position = 2;
  //noise
  duty_fader_group[NOISE].x = 152;
  duty_fader_group[NOISE].y = 41;
  duty_fader_group[NOISE].fader_position = 7;

  // Macro markers
  volumeMacroStatus.sweep = 0;
  volumeMacroStatus.square = 0;
  volumeMacroStatus.wave = 0;
  volumeMacroStatus.noise = 0;
  dutyMacroStatus.sweep = 0;
  dutyMacroStatus.square = 0;
  dutyMacroStatus.wave = 0;
  dutyMacroStatus.noise = 0;
  freqMacroStatus.sweep = 0;
  freqMacroStatus.square = 0;
  freqMacroStatus.wave = 0;
  freqMacroStatus.noise = 0;
  // x first higher value ->
  // y second higher value down
  for (int i = 0; i < 4; ++i) {
    move_sprite(i, fader_group[i].x, fader_group[i].y);
  }

  //chord steppa init
  for (int i = 0; i < 8; ++i) {
    if (i != 0) {
      csx += 2;
    }
    chordsteppa[i].root = 24;
    chordsteppa[i].majmin = 0;
    chordsteppa[i].adn = 0;
    chordsteppa[i].y = 0x0B;
    chordsteppa[i].x = csx;
    chord_steppa_step[i].y = 136;
  }

  // chord step x positions, chord page
  chord_steppa_step[0].x = 16;
  chord_steppa_step[1].x = 32;
  chord_steppa_step[2].x = 48;
  chord_steppa_step[3].x = 64;
  chord_steppa_step[4].x = 80;
  chord_steppa_step[5].x = 96;
  chord_steppa_step[6].x = 112;
  chord_steppa_step[7].x = 128;

  updateFaderMarker();
  SHOW_SPRITES;

  // zombie volume mode init
  NR12_REG = 0x08;
  NR13_REG = (UBYTE)sweep_freq & 0xFF;
  NR14_REG = 0x80 | ((sweep_freq & 0x0700)>>8);
  NR22_REG = 0x08;
  NR23_REG = (UBYTE)square_freq & 0xFF;
  NR24_REG = 0x80 | ((square_freq & 0x0700)>>8);
}
