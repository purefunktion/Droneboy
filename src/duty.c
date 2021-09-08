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
      updateWaveDuty(duty_wave);
      duty_fader_group[current_channel].fader_position = duty_wave;
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
        updateWaveDuty(duty_wave);
        duty_fader_group[current_channel].fader_position = duty_wave;
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
void updateWaveDuty(int duty) {
    updateWaveVolume(wave_volume, duty*16);
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
    updateWaveDuty(duty_wave);
    duty_fader_group[2].fader_position = duty_wave;
    moveFader(2);
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
    updateWaveDuty(duty_wave);
    duty_fader_group[2].fader_position = duty_wave;
    moveFader(2);
  }
}