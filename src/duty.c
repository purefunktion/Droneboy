#include "duty.h"
// Duty page

void dutyKeypadController() {
  if (KEY_PRESSED(J_UP)) {
    increaseDuty();
    moveFader();    
    waitpadup();
  } else if (KEY_PRESSED(J_DOWN)) { 
    decreaseDuty();
    moveFader();
    waitpadup();
  } else if (KEY_PRESSED(J_RIGHT)) {
    change_fader(J_RIGHT);
    waitpadup();
  } else if (KEY_PRESSED(J_LEFT)) {
    change_fader(J_LEFT);
    waitpadup();
  }
  wait_vbl_done();
  UPDATE_KEYS();
}

// increase duty 1 step at the time, 4 steps
// https://gbdev.io/pandocs/Sound_Controller.html#ff11---nr11---channel-1-sound-lengthwave-pattern-duty-rw
void increaseDuty() { 
  switch(current_channel)
  {
    case 0: {
      if(duty_sweep == 3) {
        break;
      }
      duty_sweep = duty_sweep + 1;
      updateDuty(dutyValues[duty_sweep]);
      duty_fader_group[current_channel].fader_position = duty_sweep;
      break;
    }
    case 1: {
      if(duty_square == 3) {
        break;
      }
      duty_square = duty_square + 1;
      updateDuty(dutyValues[duty_square]);
      duty_fader_group[current_channel].fader_position = duty_square;
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
        updateDuty(dutyValues[duty_sweep]);
        duty_fader_group[current_channel].fader_position = duty_sweep;
      } 
      break;
    }
    case 1: {
      if(duty_square != 0) {
        duty_square = duty_square - 1;    
        updateDuty(dutyValues[duty_square]);
        duty_fader_group[current_channel].fader_position = duty_square;
      }
      break;
    }
  }
}

// this updates sweep and square channels duty registers
void updateDuty(UBYTE duty) { 
  if (current_channel == 0) {
    NR11_REG = duty;
  } else if (current_channel == 1) {
    NR21_REG = duty;
  }
}