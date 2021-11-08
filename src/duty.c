#include "duty.h"
// Duty page

void dutyKeypadController() {
  if (KEY_PRESSED(J_UP)) {
    increaseDuty();
    moveFader(current_channel);
    waitpadup();
  } else if (KEY_PRESSED(J_DOWN)) { 
    decreaseDuty();
    moveFader(current_channel);
    waitpadup();
  } else if (KEY_PRESSED(J_RIGHT)) {
    change_fader(J_RIGHT);
    waitpadup();
  } else if (KEY_PRESSED(J_LEFT)) {
    change_fader(J_LEFT);
    waitpadup();
  } else if (KEY_PRESSED(J_A)) {
    placeMacroMarker();
    waitpadup();  // Wait until the button is released
  } else if (KEY_PRESSED(J_B)) {
    bPressedHandler();
    waitpadup();
  }
  //wait_vbl_done();
  //UPDATE_KEYS();
}

// increase duty 1 step at the time, 4 steps
// https://gbdev.io/pandocs/Sound_Controller.html#ff11---nr11---channel-1-sound-lengthwave-pattern-duty-rw
void increaseDuty() { 
  switch(current_channel)
  {
    case 0: {
      if(duty_sweep == 3) {
        increaseMacroDuty(1);
        break;
      }
      duty_sweep = duty_sweep + 1;
      updateSweepDuty(dutyValues[duty_sweep]);
      duty_fader_group[current_channel].fader_position = duty_sweep;
      increaseMacroDuty(1);
      break;
    }
    case 1: {
      if(duty_square == 3) {
        increaseMacroDuty(1);
        break;
      }
      duty_square = duty_square + 1;
      updateSquareDuty(dutyValues[duty_square]);
      duty_fader_group[current_channel].fader_position = duty_square;
      increaseMacroDuty(1);
      break;
    }
    case 2: {
      if(duty_wave == 3) {
        increaseMacroDuty(1);
        break;
      }
      duty_wave = duty_wave + 1;
      updateWaveDuty();
      duty_fader_group[current_channel].fader_position = duty_wave;
      increaseMacroDuty(1);
      break;
    }
    case 3: {
      if(noiseStruct.dividing_ratio == 7) {
        increaseMacroDuty(1);
        break;
      }
      noiseStruct.dividing_ratio += 1;
      updateNoiseDividingRatio(noiseStruct.dividing_ratio);
      duty_fader_group[current_channel].fader_position = noiseStruct.dividing_ratio;
      increaseMacroDuty(1);
      break;
    }
  }
}

// ...and decrease
void decreaseDuty() { 
  switch(current_channel)
  {
    case 0: {
      if(duty_sweep != 0) {
        duty_sweep = duty_sweep - 1;  
        updateSweepDuty(dutyValues[duty_sweep]);
        duty_fader_group[current_channel].fader_position = duty_sweep;
      }
      decreaseMacroDuty(1);
      break;
    }
    case 1: {
      if(duty_square != 0) {
        duty_square = duty_square - 1;    
        updateSquareDuty(dutyValues[duty_square]);
        duty_fader_group[current_channel].fader_position = duty_square;
      }
      decreaseMacroDuty(1);
      break;
    }
    case 2: {
      if(duty_wave != 0) {
        duty_wave = duty_wave - 1;
        updateWaveDuty();
        duty_fader_group[current_channel].fader_position = duty_wave;
      }
      decreaseMacroDuty(1);
      break;
    }
    case 3: {
      if(noiseStruct.dividing_ratio != 0) {
        noiseStruct.dividing_ratio = noiseStruct.dividing_ratio - 1;
        updateNoiseDividingRatio(noiseStruct.dividing_ratio);
        duty_fader_group[current_channel].fader_position = noiseStruct.dividing_ratio;
      }
      decreaseMacroDuty(1);
      break;
    }
  }
}

// this updates sweep channels duty registers
void updateSweepDuty(UBYTE duty) { 
    NR11_REG = duty;
}
// this updates square channels duty registers
void updateSquareDuty(UBYTE duty) { 
    NR21_REG = duty;
}

// Wave channel has no duty register but has corresponding waves in table
void updateWaveDuty() {
    updateWaveVolume(wave_volume, duty_wave);
}

// noise "duty". ut the controll for dividing ratio here because reasons.
void updateNoiseDividingRatio(int number) {
  noiseStruct.dividing_ratio = number;
  updateNoiseFreq(noise_freq);
}

// Increase the macro enabled channels, decrease the inverted ones.
void increaseMacroDuty(int number) {
  if(current_channel != 0 && dutyMacroStatus.sweep != 0) {
    if (dutyMacroStatus.sweep == 1 ) { // regular macro marker
      if(duty_sweep + number > 3) { // if not highest add one
        duty_sweep = 3;
      } else {
        duty_sweep += number;
      }
    } else if (dutyMacroStatus.sweep == 2) { // inverted macro
      if (duty_sweep - number < 0) {
        duty_sweep = 0;
      } else {
        duty_sweep -= number;
      }
    }
    updateSweepDuty(dutyValues[duty_sweep]);
    duty_fader_group[0].fader_position = duty_sweep;
    moveFader(0);
  }
  if(current_channel != 1 && dutyMacroStatus.square != 0) {
    if (dutyMacroStatus.square == 1 ) { // regular macro marker
      if(duty_square + number > 3) { // if not highest add one
        duty_square = 3;
      } else {
        duty_square += number;
      }
    } else if (dutyMacroStatus.square == 2) { // inverted macro
      if (duty_square - number < 0) {
        duty_square = 0;
      } else {
        duty_square -= number;
      }
    }
    updateSquareDuty(dutyValues[duty_square]);
    duty_fader_group[1].fader_position = duty_square;
    moveFader(1);
  }
  if(current_channel != 2 && dutyMacroStatus.wave != 0) {
    if (dutyMacroStatus.wave == 1 ) { // regular macro marker
      if(duty_wave + number > 3) { // if not highest add one
        duty_wave = 3;
      } else {
        duty_wave += number;
      }
    } else if (dutyMacroStatus.wave == 2) { // inverted macro
      if (duty_wave - number < 0) {
        duty_wave = 0;
      } else {
        duty_wave -= number;
      }
    }
    updateWaveDuty();
    duty_fader_group[2].fader_position = duty_wave;
    moveFader(2);
  }
  if(current_channel != 3 && dutyMacroStatus.noise != 0) {
    if (dutyMacroStatus.noise == 1 ) { // regular macro marker
      if(noiseStruct.dividing_ratio + number > 7) { // if not highest add one
        noiseStruct.dividing_ratio = 7;
      } else {
        noiseStruct.dividing_ratio += number;
      }
    } else if (dutyMacroStatus.noise == 2) { // inverted macro
      if (noiseStruct.dividing_ratio - number < 0) {
        noiseStruct.dividing_ratio = 0;
      } else {
        noiseStruct.dividing_ratio -= number;
      }
    }
    updateNoiseDividingRatio(noiseStruct.dividing_ratio);
    duty_fader_group[3].fader_position = noiseStruct.dividing_ratio;
    moveFader(3);
  }
}

// Decrease the macro enabled channels, increase the inverted ones.
void decreaseMacroDuty(int number) {
  if(current_channel != 0 && dutyMacroStatus.sweep != 0) {
    if (dutyMacroStatus.sweep == 1 ) { // regular macro marker
      if (duty_sweep - number < 0) {
        duty_sweep = 0;
      } else {
        duty_sweep -= number;
      }
    } else if (dutyMacroStatus.sweep == 2) { // inverted macro
      if(duty_sweep + number > 3) { // if not highest add one
        duty_sweep = 3;
      } else {
        duty_sweep += number;
      }
    }
    updateSweepDuty(dutyValues[duty_sweep]);
    duty_fader_group[0].fader_position = duty_sweep;
    moveFader(0);
  }
  if(current_channel != 1 && dutyMacroStatus.square != 0) {
    if (dutyMacroStatus.square == 1 ) { // regular macro marker
      if (duty_square - number < 0) {
        duty_square = 0;
      } else {
        duty_square -= number;
      }
    } else if (dutyMacroStatus.square == 2) { // inverted macro
      if(duty_square + number > 3) { // if not highest add one
        duty_square = 3;
      } else {
        duty_square += number;
      }
    }
    updateSquareDuty(dutyValues[duty_square]);
    duty_fader_group[1].fader_position = duty_square;
    moveFader(1);
  }
  if(current_channel != 2 && dutyMacroStatus.wave != 0) {
    if (dutyMacroStatus.wave == 1 ) { // regular macro marker
      if (duty_wave - number < 0) {
        duty_wave = 0;
      } else {
        duty_wave -= number;
      }
    } else if (dutyMacroStatus.wave == 2) { // inverted macro
      if(duty_wave + number > 3) { // if not highest add one
        duty_wave = 3;
      } else {
        duty_wave += number;
      }
    }
    updateWaveDuty();
    duty_fader_group[2].fader_position = duty_wave;
    moveFader(2);
  }
  if(current_channel != 3 && dutyMacroStatus.noise != 0) {
    if (dutyMacroStatus.noise == 1 ) { // regular macro marker
      if (noiseStruct.dividing_ratio - number < 0) {
        noiseStruct.dividing_ratio = 0;
      } else {
        noiseStruct.dividing_ratio -= number;
      }
    } else if (dutyMacroStatus.noise == 2) { // inverted macro
      if(noiseStruct.dividing_ratio + number > 7) { // if not highest add one
        noiseStruct.dividing_ratio = 7;
      } else {
        noiseStruct.dividing_ratio += number;
      }
    }
    updateNoiseDividingRatio(noiseStruct.dividing_ratio);
    duty_fader_group[3].fader_position = noiseStruct.dividing_ratio;
    moveFader(3);
  }
}

void bPressedHandler() {
  if (current_channel == 3) { // noise
    changNoiseCounterStep();
  } else if (current_channel == 2) { // wave
    changeWaveType();
  }
}

// Change the counter step of Polynomial Counter in the noise channel
// 1 = 7bit and 0 = 15bits
void changNoiseCounterStep() {
  if (noiseStruct.counter_step == 1) {
    noiseStruct.counter_step = 0;
    NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
    NR44_REG = 0x80; // restart channel
    set_bkg_tile_xy(0x11, 0x0F, 0x35); // left flip area
    set_bkg_tile_xy(0x12, 0x0F, 0x38); // right flip area
  } else {
    noiseStruct.counter_step = 1;
    NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
    NR44_REG = 0x80;
    set_bkg_tile_xy(0x11, 0x0F, 0x36);
    set_bkg_tile_xy(0x12, 0x0F, 0x37);
  }
}

// icon flippety flip
void changeWaveType() {
  switch(wave_type)
  {
    case SQUARE: {
      wave_type = SAW;
      set_bkg_tile_xy(0x0D, 0x0F, 0x3A);
      break;
    }
    case SAW: {
      wave_type = RAMP;
      set_bkg_tile_xy(0x0D, 0x0F, 0x3D);
      break;
    }
    case RAMP: {
      wave_type = TRIANGLE;
      set_bkg_tile_xy(0x0D, 0x0F, 0x3B);
      break;
    }
    case TRIANGLE: {
      wave_type = SINE;
      set_bkg_tile_xy(0x0D, 0x0F, 0x3C);
      break;
    }
    case SINE: {
      wave_type = SQUARE;
      set_bkg_tile_xy(0x0D, 0x0F, 0x39);
      break;
    }
  }
  updateWaveDuty();
}