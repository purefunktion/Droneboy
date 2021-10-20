#include "volume.h"
// Volume page

// Keypad 
void volumeKeypadController() {
  if (KEY_PRESSED(J_A)) {
    placeMacroMarker();
    waitpadup();  // Wait until the button is released
  } else if (KEY_TICKED(J_UP)) {
    if(KEY_PRESSED(J_B)) {
      increaseVolume(15);
      moveFader(current_channel);
    } else {
      increaseVolume(1);
      moveFader(current_channel);
    }
  } else if (KEY_PRESSED(J_DOWN)) {
    if(KEY_PRESSED(J_B)) {
      decreaseVolume(15);
      moveFader(current_channel);
    } else {
      decreaseVolume(1);
      moveFader(current_channel);
      waitpadup(); // hmm 
    }
  } else if (KEY_PRESSED(J_RIGHT)) {
    change_fader(J_RIGHT);
    waitpadup();
  } else if (KEY_PRESSED(J_LEFT)) {
    change_fader(J_LEFT);
    waitpadup();
  }
  //wait_vbl_done();
  //UPDATE_KEYS();
}

// Increase volume 1 step at the time
void increaseVolume(int number) {
  switch(current_channel)
  {
    case 0: {
      if ((sweep_volume + number) >= 15) {
        sweep_volume = 15;
      } else {
        sweep_volume += number;
      }
      updateSweepVolume(volumeValues[sweep_volume]);
      fader_group[current_channel].fader_position = sweep_volume;
      increaseMacroVolume(number);
      break;
    }
    case 1: {
      if ((square_volume + number) >= 15) {
        square_volume = 15;
      } else {
        square_volume += number;
      }
      updateSquareVolume(volumeValues[square_volume]);
      fader_group[current_channel].fader_position = square_volume;
      increaseMacroVolume(number);
      break;
    }
    case 2: {
      if ((wave_volume + number) >= 15) {
        wave_volume = 15;
      } else {
        wave_volume += number;
      }
      updateWaveVolume(wave_volume, duty_wave*16);
      fader_group[current_channel].fader_position = wave_volume;
      increaseMacroVolume(number);
      break;
    }
    case 3: {
      if ((noise_volume + number) >= 15) {
        noise_volume = 15;
      } else {
        noise_volume += number;
      }
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[current_channel].fader_position = noise_volume;
      increaseMacroVolume(number);
      break;
    }
  }
}


/**
* Increase the macro marked channels volume with number.
**/
void increaseMacroVolume(int number) {
  // not current fader used and macro marker set
  if(current_channel != 0 && volumeMacroStatus.sweep != 0) {
    if (volumeMacroStatus.sweep == 1 ) { // regular macro marker
      if (sweep_volume + number > 15) {
        sweep_volume = 15;
      } else {
        sweep_volume += number;
      }
    } else if (volumeMacroStatus.sweep == 2) { // inverted macro
      if (sweep_volume - number < 0) {
        sweep_volume = 0;
      } else {
        sweep_volume -= number;
      }
    }
    updateSweepVolume(volumeValues[sweep_volume]);
    fader_group[0].fader_position = sweep_volume;
    moveFader(0);
  }
  if(volumeMacroStatus.square != 0 && current_channel != 1) {
    if (volumeMacroStatus.square == 1 ) {
      if (square_volume + number > 15) {
        square_volume = 15;
      } else {
        square_volume += number;
      }
    } else {
      if (square_volume - number < 0) {
        square_volume = 0;
      } else {
        square_volume -= number;
      }
    }
    updateSquareVolume(volumeValues[square_volume]);
    fader_group[1].fader_position = square_volume;
    moveFader(1);
  }
  if(volumeMacroStatus.wave != 0 && current_channel != 2) {
    if (volumeMacroStatus.wave == 1 ) {
      if (wave_volume + number > 15) {
        wave_volume = 15;
      } else {
        wave_volume += number;
      }
    } else {
      if (wave_volume - number < 0) {
        wave_volume = 0;
      } else {
        wave_volume -= number;
      }
    }
    updateWaveVolume(wave_volume, duty_wave*16);
    fader_group[2].fader_position = wave_volume;
    moveFader(2);
  }
  //&& noise_volume != 15
  if(volumeMacroStatus.noise != 0 && current_channel != 3 ) {
    if (volumeMacroStatus.noise == 1 && noise_volume != 15) {
      if (noise_volume + number > 15) {
        noise_volume = 15;
      } else {
        noise_volume += number;
      }
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[3].fader_position = noise_volume;
      moveFader(3);
    } else if (volumeMacroStatus.noise == 2) {
      if (noise_volume - number < 0) {
        noise_volume = 0;
      } else {
        noise_volume -= number;
      }
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[3].fader_position = noise_volume;
      moveFader(3);
    }
  }
}

// ..and decrease
void decreaseVolume(int number) {
  switch(current_channel)
  {
    case 0: {
      if ((sweep_volume - number) <= 0) {
        sweep_volume = 0;
      } else {
        sweep_volume -= number;
      }
      updateSweepVolume(volumeValues[sweep_volume]);
      fader_group[current_channel].fader_position = sweep_volume;
      decreaseMacroVolume(number);
      break;
    }
    case 1: {
      if ((square_volume - number) <= 0) {
        square_volume = 0;
      } else {
        square_volume -= number;
      }
      updateSquareVolume(volumeValues[square_volume]);
      fader_group[current_channel].fader_position = square_volume;
      decreaseMacroVolume(number);
      break;
    }
    case 2: {
      if ((wave_volume - number) <= 0) {
        wave_volume = 0;
      } else {
        wave_volume -= number;
      }
      updateWaveVolume(wave_volume, duty_wave*16);
      fader_group[current_channel].fader_position = wave_volume;
      decreaseMacroVolume(number);
      break;
    }
    case 3: {
      if ((noise_volume - number) <= 0) {
        noise_volume = 0;
      } else {
        noise_volume -= number;
      }
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[current_channel].fader_position = noise_volume;
      decreaseMacroVolume(number);
      break;
    }
  }
}

/**
* Increase the macro marked channels volume with number.
**/
void decreaseMacroVolume(int number) {

  if(volumeMacroStatus.sweep != 0 && current_channel != 0) {
    if (volumeMacroStatus.sweep == 1 ) { // regular macro marker
      if (sweep_volume - number < 0) {
        sweep_volume = 0;
      } else {
        sweep_volume -= number;
      }
    } else {
      if (sweep_volume + number > 15) {
        sweep_volume = 15;
      } else {
        sweep_volume += number;
      }
    }
    updateSweepVolume(volumeValues[sweep_volume]);
    fader_group[0].fader_position = sweep_volume;
    moveFader(0);
  }
  if(volumeMacroStatus.square != 0 && current_channel != 1) {
    if (volumeMacroStatus.square == 1 ) {
      if (square_volume - number < 0) {
        square_volume = 0;
      } else {
        square_volume -= number;
      }
    } else {
      if (square_volume + number > 15) {
        square_volume = 15;
      } else {
        square_volume += number;
      }
    }
    updateSquareVolume(volumeValues[square_volume]);
    fader_group[1].fader_position = square_volume;
    moveFader(1);
  }
  if(volumeMacroStatus.wave != 0 && current_channel != 2) {
    if (volumeMacroStatus.wave == 1 ) {
      if (wave_volume - number < 0) {
        wave_volume = 0;
      } else {
        wave_volume -= number;
      }
    } else {
      if (wave_volume + number > 15) {
        wave_volume = 15;
      } else {
        wave_volume += number;
      }
    }
    updateWaveVolume(wave_volume, duty_wave*16);
    fader_group[2].fader_position = wave_volume;
    moveFader(2);
  }
  if(volumeMacroStatus.noise != 0 && current_channel != 3) {
    if (volumeMacroStatus.noise == 1) {
      if (noise_volume - number < 0) {
        noise_volume = 0;
      } else {
        noise_volume -= number;
      }
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[3].fader_position = noise_volume;
      moveFader(3);
    } else {
      if (noise_volume != 15) {
        if (noise_volume + number > 15) {
          noise_volume = 15;
        } else {
          noise_volume += number;
        }
        updateNoiseVolume(volumeValues[noise_volume]);
        fader_group[3].fader_position = noise_volume;
        moveFader(3);
      }
    }
  }
}

// Volume update funtions
void updateSweepVolume(int volume) {
  UBYTE freqhigh; 
  UWORD freq;
  NR12_REG = volume;
  // checks if in freq or note mode
  freq = (frequency_mode == 0) ? sweep_freq : frequencies[sweep_note];
  freqhigh = (UBYTE)((freq & 0x0700)>>8);
  NR14_REG = 0x80 | freqhigh;
}

void updateSquareVolume(int volume) { 
  UBYTE freqhigh;
  UWORD freq;
  NR22_REG = volume;
  freq = (frequency_mode == 0) ? square_freq : frequencies[square_note];
  freqhigh = (UBYTE)((freq & 0x0700)>>8);
  NR24_REG = 0x80 | freqhigh; //restart the channel annars inte funkis
}

// this function lowers the amplitude the 4 bit samples in the wave pattern RAM
void updateWaveVolume(int volume, int sample_index) {
    UWORD freq;
    UBYTE freqlow, freqhigh;
    if (volume == 0) {
      NR32_REG = 0x00;//turn off volume
      return;
    } else  {
      NR32_REG = 0x20; // highest volume
    }
    
    NR51_REG = 0b10111011;
    NR30_REG = 0x00;
    unsigned char *dst = (unsigned char *)(0xFF30u);
    unsigned char *src = &samples[sample_index];

    for(int i = 0; i < 16; i++) {
      unsigned char tmp = *src;
      *dst++ = (UBYTE) ((volume & tmp) | ((volume & tmp)<<4));
      *src++;
    }
    freq = (frequency_mode == 0) ? wave_freq : frequencies[wave_note];
    freqlow = (UBYTE)freq & 0xFF;
    freqhigh = (UBYTE)((freq & 0x0700)>>8);
    NR30_REG |= 0x80; // Enable wave channel.
    NR51_REG = 0b11111111;
    NR33_REG = freqlow; // Set lower byte of frequency.
    NR34_REG = 0x80 | freqhigh;// 0xC0 // Set higher byte of frequency and start playback.
}

// update the noise
void updateNoiseVolume(UBYTE volume) {
  NR42_REG = volume;
  //NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
  NR44_REG = 0x80;
}