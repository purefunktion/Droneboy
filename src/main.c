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
int square_volume = 0;
int wave_volume = 0;
int noise_volume = 0;

int current_channel = 0;
int frequency_mode = 0;
UINT8 num_control_pages = 3;
int active_control_page = 0;
struct NoiseyStruct noiseStruct;

struct fader fader_group[4];
struct fader duty_fader_group[3];

int duty_sweep = 2;
int duty_square = 2;
int duty_wave = 2;
const UBYTE dutyValues[4] = {0x00, 0x40, 0x80, 0xC0};
const UBYTE dutyFaderPosition[4] = {111, 89, 65, 41};

// Note names to display
const char noteNames[72][5] = {
  " C 3", "C# 3", " D 3", "D# 3", " E 3", " F 3", "F# 3", " G 3", "G# 3", " A 3", "A# 3", " B 3",
  " C 4", "C# 4", " D 4", "D# 4", " E 4", " F 4", "F# 4", " G 4", "G# 4", " A 4", "A# 4", " B 4",
  " C 5", "C# 5", " D 5", "D# 5", " E 5", " F 5", "F# 5", " G 5", "G# 5", " A 5", "A# 5", " B 5",
  " C 6", "C# 6", " D 6", "D# 6", " E 6", " F 6", "F# 6", " G 6", "G# 6", " A 6", "A# 6", " B 6",
  " C 7", "C# 7", " D 7", "D# 7", " E 7", " F 7", "F# 7", " G 7", "G# 7", " A 7", "A# 7", " B 7",
  " C 8", "C# 8", " D 8", "D# 8", " E 8", " F 8", "F# 8", " G 8", "G# 8", " A 8", "A# 8", " B 8"
};
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
const UBYTE samples[] = {
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, //fiddy fiddy
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  // triangle wave, set duty_wave to 4 to try
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10

};
// y coordinates for the volume fader(16 steps)
const UBYTE volumeFaderPosition[16] = {119, 114, 109, 104, 98, 93, 89, 85, 80, 76, 71, 67, 58, 54, 49, 41};

// Main 
void main() {

  intro(); // comment this out if enoying
  init();
  
  // Main loop
  while(1) {
    // Change control page
    if (KEY_PRESSED(J_SELECT)) {
      if (active_control_page == num_control_pages-1) {
        active_control_page = 0;
      } else {
        active_control_page = active_control_page + 1;
      }
      changeControlPage(active_control_page);
      waitpadup();
    }
    // Switch depending on which control page we are on
    switch(active_control_page)
    {
      case 0: { // volume
        num_faders = 4;
        volumeKeypadController();
        break;
      }
      case 1: { // duty
        num_faders = 3;
        dutyKeypadController();
        break;
      }
      case 2: { // freq/note
        num_faders = 4;
        frequencyKeypadController();
        break;
      }
    }
    wait_vbl_done();
    UPDATE_KEYS();
  }
}

/*
* Flip between pages in the app
*/
void changeControlPage(int to_page) {
  if (to_page == 0) {
    changeToVolumeBackground();
  } else if (to_page == 1) {
    changeToDutyBackground();
  } else {
    changeToFrequencyBackground();
  }
}

/*
* Changes to the frequency background
*/
void changeToFrequencyBackground() {
  wait_vbl_done();
  DISPLAY_OFF;
  set_bkg_data(0,4, fadertile); // setup fader tiles
  set_bkg_tiles(0x00, 0x00, 20, 18, frequencybackground); // the bakground
  SHOW_BKG;
  DISPLAY_ON;
  setUpFrequencySprites();
  // this hides the sprites from duty page
  for (int i = 0; i <= max_faders-1; i++) 
  {
    move_sprite(i, 1, 168);
  }
  current_channel = 0;
  updateFaderMarker();
}

/*
* This could use some work but is left as is for clarity(hopefully)
*/
void setUpFrequencySprites() {
  // First the sweep channel
  current_channel = 0;
  int value = sweep_freq;
  int position = 4;
  setCounterSprites(position, value); // setup the frequency tiles
  clearCounterValues(position); // clear if needed
  setNoteSprites(position+16, sweep_note); // set the note tiles

  // square
  current_channel = 1;
  value = square_freq;
  position = 8;
  setCounterSprites(position, value);
  clearCounterValues(position);
  setNoteSprites(position+16, square_note);

  //wave
  current_channel = 2;
  value = wave_freq;
  position = 12;
  setCounterSprites(position, value);
  clearCounterValues(position);
  setNoteSprites(position+16, wave_note);

  //noise
  current_channel = 3;
  value = noise_freq;
  position = 16;
  setCounterSprites(position, value);
  clearCounterValues(position);
  setNoteSprites(position+16, noise_note);

  current_channel = 0;

  // This section positions the tiles used to display
  // notes or frequencies.
  // sweep
  int sweep_x = 51; // coordinates
  int sweep_y = 57;
  // numbers
  move_sprite(4, sweep_x, sweep_y); // 1
  move_sprite(5, sweep_x-8, sweep_y); // 10
  move_sprite(6, sweep_x-16, sweep_y); // 100
  move_sprite(7, sweep_x-24, sweep_y); // 1000
  // notes
  sweep_y = sweep_y + 10; // 10 pixels under the freq tiles
  move_sprite(20, sweep_x, sweep_y); 
  move_sprite(21, sweep_x-8, sweep_y); // decrese by 8 to move left
  move_sprite(22, sweep_x-16, sweep_y);
  move_sprite(23, sweep_x-24, sweep_y);

  // square
  int square_x = 130;
  int square_y = 57;
  move_sprite(8,  square_x, square_y); 
  move_sprite(9,  square_x-8, square_y); 
  move_sprite(10, square_x-16, square_y); 
  move_sprite(11, square_x-24, square_y); 
  //notes
  square_y = square_y + 10;
  move_sprite(24, square_x, square_y); 
  move_sprite(25, square_x-8, square_y);
  move_sprite(26, square_x-16, square_y);
  move_sprite(27, square_x-24, square_y);

  // wave
  int wave_x = 51;
  int wave_y = 121;
  //numbers
  move_sprite(12, wave_x, wave_y); 
  move_sprite(13, wave_x-8, wave_y); 
  move_sprite(14, wave_x-16, wave_y); 
  move_sprite(15, wave_x-24, wave_y); 
  //notes
  wave_y = wave_y + 10;
  move_sprite(28, wave_x, wave_y); 
  move_sprite(29, wave_x-8, wave_y);
  move_sprite(30, wave_x-16, wave_y);
  move_sprite(31, wave_x-24, wave_y);
  
  // noise
  int noise_x = 130;
  int noise_y = 121;
  //numbers
  move_sprite(16, noise_x, noise_y);
  move_sprite(17, noise_x-8, noise_y); 
  move_sprite(18, noise_x-16, noise_y); 
  move_sprite(19, noise_x-24, noise_y); 
  //noise notes exluded
}

/*
* This changes sprites/background to the duty page
* Duty is only available for sweep and square
*/
void changeToDutyBackground() {
  wait_vbl_done();
  DISPLAY_OFF;
  set_bkg_data(0,4, fadertile);
  set_bkg_tiles(0x00, 0x00, 20, 18, dutyfaderbackground);
  SHOW_BKG;
  DISPLAY_ON;
  // move the duty faders on screen
  move_sprite(0, duty_fader_group[0].x, duty_fader_group[0].y);
  move_sprite(1, duty_fader_group[1].x, duty_fader_group[1].y);
  move_sprite(2, duty_fader_group[2].x, duty_fader_group[2].y);
  // move noise off screen
  for (int i = 3; i <= max_faders-1; i++) {
    move_sprite(i, 1, 168);
  }
  current_channel = 0; // set to sweep
  updateFaderMarker();
}

/*
* Change to the volume background.
*/
void changeToVolumeBackground() {
  wait_vbl_done();
  DISPLAY_OFF;
  set_bkg_data(0,4, fadertile);
  set_bkg_tiles(0x00, 0x00, 20, 18, volumefaderbackground);
  SHOW_BKG;
  DISPLAY_ON;
  hideSprites(4, 36); // hide tiles from frequency page
  for (int i = 0; i <= max_faders-1; i++) {
    move_sprite(i, fader_group[i].x, fader_group[i].y);
  }
  current_channel = 0;
  updateFaderMarker();
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
void updateFaderMarker() {
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
  }
}

/*
* This will move the fader up/down(y axis), values from volumeFaderPosition
*/
void moveFader() {
  if (active_control_page == 0) {
    fader_group[current_channel].y = volumeFaderPosition[fader_group[current_channel].fader_position];
    move_sprite(current_channel, fader_group[current_channel].x, fader_group[current_channel].y);
  } else if (active_control_page == 1 ) {
    duty_fader_group[current_channel].y = dutyFaderPosition[duty_fader_group[current_channel].fader_position];
    move_sprite(current_channel, duty_fader_group[current_channel].x, duty_fader_group[current_channel].y);
  }
}

/*
* This will load a sample in to the wave channels wave pattern ram
* More or less copypasta from
* https://gbdev.gg8.se/forums/viewtopic.php?id=758
* and Nitro2k01 "antispike fix"(for future use, not realy shure this is correct)
* https://blog.gg8.se/wordpress/2013/02/11/gameboy-project-week-6-can-i-have-an-a-men/
*/
void loadWave(int wave) {
  UBYTE freqlow, freqhigh;
  NR51_REG = 0b10111011;
  NR30_REG = 0x00; 
  unsigned char *dst = (unsigned char *)(0xFF30u); // Create pointer to the wave RAM.
  unsigned char *src = &samples[wave]; // Create pointer to the waveform.
  unsigned char length = 16; // Number of bytes to copy.
  while (length--) {
    *dst++ = *src++;
  }
  freqlow = (UBYTE)wave_freq & 0xFF; // lower byte of frquency
  freqhigh = (UBYTE)((wave_freq & 0x0700)>>8); // higher bits
  NR30_REG |= 0x80; // Enable wave channel.
  NR51_REG = 0b11111111;
  NR33_REG = freqlow; // Set lower byte of frequency.
  NR34_REG = 0x80 | freqhigh; // 0xC0 // Set higher byte of frequency and start playback.
}

/*
* Setup when starting the app
*/
void init() {
  // IMPORTANT! NR52_REG must always be set first
  // otherwise there will be no sound.
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-control-registers
  NR52_REG = 0x80; // Bit 7 all sounds on
  NR50_REG = 0x77; // don't need Vin  
  NR51_REG = 0xFF; // output all channels to S01 and SO2

  // sweep channel 
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-1---tone--sweep
  // Volume envelope
  NR12_REG = 0x00; //0x00 // 0=min volume, length ignored
  // Duty
  NR11_REG = 0x80; //0x80 // 50% square wave
  // Frequency
  updateSweepFreq(sweep_freq);

  // square channel
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-2---tone
  NR22_REG = 0x00;
  NR21_REG = 0x80;
  updateSquareFreq(square_freq);

  // wave channel
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-3---wave-output
  loadWave(duty_wave*16);

  // noise channel
  // https://gbdev.io/pandocs/Sound_Controller.html#sound-channel-4---noise
  noiseStruct.dividing_ratio = 7;//: 3;
  noiseStruct.counter_step = 1;//: 1;
  noiseStruct.clock_freq = noise_freq;//: 4 bits;
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
  set_bkg_tiles(0,0,20,18, volumefaderbackground);

  SHOW_BKG;
  DISPLAY_ON;
  SPRITES_8x8;
  set_sprite_data(0, 4, fadertile);
  for (int i = 0; i < 4; ++i){
    set_sprite_tile(i, 0);
  }

  set_sprite_data(4, 20, frequencytiles);

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

  // Volume faders
  for (int i = 0; i < 4; ++i) {
    fader_group[i].y = 119; // they start at the bottom
    fader_group[i].fader_position = 0; // volume starts at zero
  }
  // from left to right 
  fader_group[0].x = 32; // sweep 
  fader_group[1].x = 72; // square
  fader_group[2].x = 112; // wave
  fader_group[3].x = 152; // noise

  // Duty faders
  // sweep
  duty_fader_group[0].x = 32; 
  duty_fader_group[0].y = 65;
  duty_fader_group[0].fader_position = 2;
  // square
  duty_fader_group[1].x = 72;
  duty_fader_group[1].y = 65;
  duty_fader_group[1].fader_position = 2;
  //wave
  duty_fader_group[2].x = 112;
  duty_fader_group[2].y = 65;
  duty_fader_group[2].fader_position = 2;

  // x first higher value ->
  // y second higher value down
  for (int i = 0; i < 4; ++i) {
    move_sprite(i, fader_group[i].x, fader_group[i].y);
  }

  updateFaderMarker();
  SHOW_SPRITES;
}