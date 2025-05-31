#include <gb/gb.h>
#include "droneboy.h"
#include "tilesandbackgrounds.h"
#include "definitions.h"
#include "globals.h"
#include "midi.h"
#include "serial.h"

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
int frequency_mode = 0;
UINT8 chord_mode = 0;
UINT8 num_control_pages = 4;
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

// current wave type in wave channel
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
UINT8 beats_per_step = 0; // 0 == once every beat(quarter)
UINT8 beats_counter = 0; // keeps track of beats
int current_seq_chord = 0;

// state of the navigation 
int right_pressed = 0; // select + right goes right(duh)
int left_pressed = 0; // read above but left
int up_pressed = 0; // up nav
int down_pressed = 0; // down nav

// navigation state
State current_state = VOLUME_PAGE;

// BPM globals
uint16_t tim_cnt = 0;
uint16_t bpm = 120;
uint16_t bpm_in_cycles; // how many timer ticks per beat

// if 0 then reading from serial
BYTE system_idle = 0;

// 0 read lower sounding waves else higher
BYTE low_or_high_wave_freq = 0;
// flip between high or low waves in frequency button controller
BYTE low_high_wave_flip = 0;

// Main 
void main(void) {
  
  __critical {
      add_TIM(tim);
  }
  // Set TMA to divide clock by 0xF0U
  TMA_REG = 0xF0U;
  // Set clock to 16384 Hertz
  TAC_REG = 0x07U;
  tim_cnt = 0;
  enable_interrupts();
  set_interrupts(VBL_IFLAG | TIM_IFLAG | SIO_IFLAG);
  init();
  
  // Navigation events
  Event event = EVENT_NONE;
  // this makes serial tick
  rSC = SIOF_XFER_START | SIOF_CLOCK_EXT;
  // Main loop
  while(1) {
    // serial flag
    system_idle = 1;
    updateMidiBuffer();
    if (system_idle == 1) {
      // start chord sequencer from anywhere(only in tone/chord mode)
      if (KEY_PRESSED(J_START)) {
        startStopChordStep();
        waitpadup();
      }
      event = EVENT_NONE;
      // Change control page
      if (KEY_PRESSED(J_SELECT)) {
        if (right_pressed == 1) {
          if (!KEY_PRESSED(J_RIGHT)) {
            event = EVENT_RIGHT;
            right_pressed = 0;
          }
        } else {
          if (KEY_PRESSED(J_RIGHT)) {
            right_pressed = 1;
          }
        }
        if (left_pressed == 1) {
          if (!KEY_PRESSED(J_LEFT)) {
            event = EVENT_LEFT;
            left_pressed = 0;
          }
        } else {
          if (KEY_PRESSED(J_LEFT)) {
            left_pressed = 1;
          }
        }
        if (up_pressed == 1) {
          if (!KEY_PRESSED(J_UP)) {
            event = EVENT_UP;
            up_pressed = 0;
          }
        } else {
          if (KEY_PRESSED(J_UP)) {
            up_pressed = 1;
          }
        }
        if (down_pressed == 1) {
          if (!KEY_PRESSED(J_UP)) {
            event = EVENT_DOWN;
            down_pressed = 0;
          }
        } else {
          if (KEY_PRESSED(J_DOWN)) {
            down_pressed = 1;
          }
        }
      } else {
        right_pressed = 0;
        left_pressed = 0;
        up_pressed = 0;
        down_pressed = 0;
      }

      if (event != EVENT_NONE) {
        switch (current_state) {
          case VOLUME_PAGE:
            handleVolumePage(event);
            break;
          case DUTY_PAGE:
            handleDutyPage(event);
            break;
          case FREQ_PAGE:
            handleFreqPage(event);
            break;
          case CHORD_PAGE:
            handleChordPage(event);
            break;
          case CREDIT_PAGE:
            handleCreditPage(event);
            break;
          case BPM_PAGE:
            handleBpmPage(event);
            break;
        }
      }

      if (right_pressed != 1
          && left_pressed != 1
          && up_pressed != 1
          && down_pressed != 1) {
        switch(current_state)
        {
          case VOLUME_PAGE: {
            num_faders = 4;
            volumeKeypadController();
            break;
          }
          case DUTY_PAGE: {
            num_faders = 4;
            dutyKeypadController();
            break;
          }
          case FREQ_PAGE: {
            num_faders = 4;
            flipHeader();
            frequencyKeypadController();
            break;
          }
          case CHORD_PAGE: {
            num_faders = 1;
            chordKeypadController();
            break;
          }
          case BPM_PAGE: {
            num_faders = 1;
            bpmKeypadController();
            break;
          }
        }
      }
      wait_vbl_done();
      UPDATE_KEYS();
    }
  }
}

// timer interrupt
void tim(void) {
  tim_cnt++;
  if (tim_cnt >= bpm_in_cycles) {
    blinkBPM();
    if (play_chord_step == 1) {
      playChordStep();
    }
    tim_cnt = 0;
  }
}

// Handel navigation event on Volume page
void handleVolumePage(Event event) {
  switch (event) {
    case EVENT_RIGHT: {
      current_state = DUTY_PAGE;
      changeToDutyBackground();
      break;
    }
    case EVENT_LEFT: {
      current_state = CHORD_PAGE;
      changeToChordBackground();
      break;
    }
    case EVENT_DOWN: {
      current_state = CREDIT_PAGE;
      goToCreditPage();
      break;
    }
    default:
      break;
  }
}

// Handle navigation event on Duty page
void handleDutyPage(Event event) {
  switch (event) {
    case EVENT_RIGHT: {
      current_state = FREQ_PAGE;
      changeToFrequencyBackground();
      break;
    }
    case EVENT_LEFT: {
      current_state = VOLUME_PAGE;
      changeToVolumeBackground();
      break;
    }
    default:
      break;
  }
}

// Handle navigation event on Frequency page
void handleFreqPage(Event event) {
  switch (event) {
    case EVENT_RIGHT: {
      current_state = CHORD_PAGE;
      changeToChordBackground();
      break;
    }
    case EVENT_LEFT: {
      current_state = DUTY_PAGE;
      changeToDutyBackground();
      break;
    }
    default:
      break;
  }
}

// Handle navigation event on Chord page
void handleChordPage(Event event) {
  switch (event) {
    case EVENT_RIGHT: {
      current_state = VOLUME_PAGE;
      changeToVolumeBackground();
      break;
    }
    case EVENT_LEFT: {
      current_state = FREQ_PAGE;
      changeToFrequencyBackground();
      break;
    }
    case EVENT_UP: {
      current_state = BPM_PAGE;
      changeToBPMBackground();
      break;
    }
    default:
      break;
  }
}

// Handle navigation event on Credit page
void handleCreditPage(Event event) {
  switch (event) {
    case EVENT_UP: {
      current_state = VOLUME_PAGE;
      changeToVolumeBackground();
      break;
    }
    default:
      break;
  }
}

// Handle navigation event on BPM page
void handleBpmPage(Event event) {
  switch (event) {
    case EVENT_DOWN: {
      current_state = CHORD_PAGE;
      changeToChordBackground();
      break;
    }
    default:
      break;
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
  printLowHighFreqIcon();
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

/*
* Change to the BPM background.
*/
void changeToBPMBackground(void) {
  set_bkg_data(0,4, fadertile); // setup fader tiles
  set_bkg_tiles(0x00, 0x00, 20, 18, bpmbackground);
  // move the duty faders on screen

  for (int i = 0; i <= max_faders-1; i++)
  {
    move_sprite(i, 1, 168);
  }
  move_sprite(37, 2, 168);
  move_sprite(38, 2, 168);
  move_sprite(39, 0, 0); // hide rec marker from chord
  hideSprites(0, 36);
  printBPM(); // show current bpm
  printChordSteppaOnOff(); // show if chordsteppa is on/off
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
  if (current_state == VOLUME_PAGE) { // Volume
    move_sprite(37, fader_group[current_channel].x, 128);
    move_sprite(38, fader_group[current_channel].x, 136);
  } else if(current_state == DUTY_PAGE) { // Duty
    move_sprite(37, fader_group[current_channel].x, 120);
    move_sprite(38, fader_group[current_channel].x, 128); 
  } else if(current_state == FREQ_PAGE) { // Frequency
    if (frequency_mode == 0) { // Frequency number mode
      move_sprite(37, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]);
      move_sprite(38, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]+8);
    } else { // Note mode
      move_sprite(37, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]+10);
      move_sprite(38, faderMarkerFreqx[0][current_channel], faderMarkerFreqy[0][current_channel]+18);
    }
  } else if(current_state == CHORD_PAGE) { // chord
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
  if (current_state == VOLUME_PAGE) {
    fader_group[channel].y = volumeFaderPosition[fader_group[channel].fader_position];
    move_sprite(channel, fader_group[channel].x, fader_group[channel].y);
  } else if (current_state == DUTY_PAGE) {
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
* See below for C solution
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
  // see load_wave.s
  load_wave(&waveToBeLoaded[0]);
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
  // https://gbdev.io/pandocs/Audio_Registers.html
  NR52_REG = 0x80; // Bit 7 all sounds on
  NR50_REG = 0x77; // don't need Vin  
  NR51_REG = 0xFF; // output all channels to S01 and SO2

  // sweep channel 
  // https://gbdev.io/pandocs/Audio_Registers.html#sound-channel-1--pulse-with-period-sweep

  // Volume envelope
  NR12_REG = 0x08; //0x00 // 0=min volume, length ignored
  // Duty
  NR11_REG = 0x80; //0x80 // 50% square wave
  // Frequency
  updateSweepFreq(1);

  // square channel
  // https://gbdev.io/pandocs/Audio_Registers.html#sound-channel-2--pulse
  NR22_REG = 0x00;
  NR21_REG = 0x80;
  updateSquareFreq(1);

  // wave channel
  // https://gbdev.io/pandocs/Audio_Registers.html#sound-channel-3--wave-output
  loadWave();

  // noise channel
  // https://gbdev.io/pandocs/Audio_Registers.html#sound-channel-4--noise
  noiseStruct.dividing_ratio = 7; // 3 bits
  noiseStruct.counter_step = 1; // 1 = 7bits, 0 = 15 bits
  noiseStruct.clock_freq = noise_freq; // 4 bits
  NR41_REG = 0x80;
  NR42_REG = 0x08;
  updateNoiseFreq(noise_freq);
  NR44_REG = 0x80;
  display_off(); // @todo deleting this works on emus, not on hardware
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

  UINT8 root = 19;
  UINT8 major_scale_from_root[] = {0, 2, 4, 5, 7, 9, 11};
  UINT8 major_scale_majmin[] = {0, 1, 1, 0, 0, 1, 1};
  UINT8 major_scale_adn[] = {0, 0, 0, 0, 0, 0, 2};
  //chord steppa init
  for (int i = 0; i < 7; ++i) {
    if (i != 0) {
      csx += 2;
    }
    chordsteppa[i].root = root + major_scale_from_root[i];
    chordsteppa[i].majmin = major_scale_majmin[i];
    chordsteppa[i].adn = major_scale_adn[i];
    chordsteppa[i].y = 0x0B;
    chordsteppa[i].x = csx;
    chord_steppa_step[i].y = 136;
  }
  chordsteppa[7].root = root + 12;
  chordsteppa[7].majmin = 0;
  chordsteppa[7].adn = 0;
  chordsteppa[7].y = 0x0B;
  chordsteppa[7].x = csx + 2; // we are done with csx here
  chord_steppa_step[7].y = 136;

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
  SHOW_BKG;
  DISPLAY_ON;
  SPRITES_8x8;
  SHOW_SPRITES;

  // zombie volume mode init
  NR12_REG = 0x08;
  NR13_REG = (UBYTE)sweep_freq & 0xFF;
  NR14_REG = 0x80 | ((sweep_freq & 0x0700)>>8);
  NR22_REG = 0x08;
  NR23_REG = (UBYTE)square_freq & 0xFF;
  NR24_REG = 0x80 | ((square_freq & 0x0700)>>8);

  setBpm(120); // disco time
}
