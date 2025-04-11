#include "volume.h"
// Volume page

// Keypad 
void volumeKeypadController(void) {
  // sweep up done
  if (KEY_RELEASED(J_UP) && up_volume_counter > 0) {
      up_volume_counter = 0;
      volume_slide_counter = 0;
  }

  // volume down done
  if (KEY_RELEASED(J_DOWN) && down_volume_counter > 0) {
      down_volume_counter = 0;
      volume_slide_counter = 0;
  }

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
  } else if (KEY_TICKED(J_DOWN)) {
    if(KEY_PRESSED(J_B)) {
      decreaseVolume(15);
      moveFader(current_channel);
    } else {
      decreaseVolume(1);
      moveFader(current_channel);
    }
  } else if (KEY_PRESSED(J_RIGHT)) {
    change_fader(J_RIGHT);
    waitpadup();
  } else if (KEY_PRESSED(J_LEFT)) {
    change_fader(J_LEFT);
    waitpadup();
  }
  // continuous up volle
  if (KEY_PRESSED(J_UP)) {
      // counter to check if just a button press
      if (up_volume_counter == 30) {
          if (volume_slide_counter == 4) { // give wave and noise some breathing room...
            increaseVolume(1);
            moveFader(current_channel);
            volume_slide_counter = 0;
          } else {
            volume_slide_counter++;
          }
      } else {
          up_volume_counter++;
      }
  }

  // continuous down volle
  if (KEY_PRESSED(J_DOWN)) {
      // counter to check if just a button press
      if (down_volume_counter == 30) {
        if (volume_slide_counter == 4) {
          decreaseVolume(1);
          moveFader(current_channel);
          volume_slide_counter = 0;
        } else {
          volume_slide_counter++;
        }
      } else {
          down_volume_counter++;
      }
  }
}

// Increase volume 1 step at the time
void increaseVolume(int number) {
  switch(current_channel)
  {
    case SWEEP: {
      if ((sweep_volume + number) >= 15) {
        updateSweepVolume(15);
      } else {
        updateSweepVolume(sweep_volume + number);
      }
      fader_group[current_channel].fader_position = sweep_volume;
      increaseMacroVolume(number);
      break;
    }
    case SQUARE: {
      if ((square_volume + number) >= 15) {
        updateSquareVolume(15);
      } else {
        updateSquareVolume(square_volume + number);
      }
      fader_group[current_channel].fader_position = square_volume;
      increaseMacroVolume(number);
      break;
    }
    case WAVE: {
      if ((wave_volume + number) >= 15) {
        wave_volume = 15;
      } else {
        wave_volume += number;
      }
      updateWaveVolume(wave_volume, duty_wave);
      fader_group[current_channel].fader_position = wave_volume;
      increaseMacroVolume(number);
      break;
    }
    case NOISE: {
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
        updateSweepVolume(15);
      } else {
        updateSweepVolume(sweep_volume + number);
      }
    } else if (volumeMacroStatus.sweep == 2) { // inverted macro
      if (sweep_volume - number < 0) {
        updateSweepVolume(0);
      } else {
        updateSweepVolume(sweep_volume - number);
      }
    }
    fader_group[0].fader_position = sweep_volume;
    moveFader(0);
  }
  if(volumeMacroStatus.square != 0 && current_channel != 1) {
    if (volumeMacroStatus.square == 1 ) {
      if (square_volume + number > 15) {
        updateSquareVolume(15);
      } else {
        updateSquareVolume(square_volume + number);
      }
    } else {
      if (square_volume - number < 0) {
        updateSquareVolume(0);
      } else {
        updateSquareVolume(square_volume - number);
      }
    }
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
    updateWaveVolume(wave_volume, duty_wave);
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
    case SWEEP: {
      if ((sweep_volume - number) <= 0) {
        updateSweepVolume(0);
      } else {
        updateSweepVolume(sweep_volume - number);
      }      
      fader_group[current_channel].fader_position = sweep_volume;
      decreaseMacroVolume(number);
      break;
    }
    case SQUARE: {
      if ((square_volume - number) <= 0) {
        updateSquareVolume(0);
      } else {
        updateSquareVolume(square_volume - number);
      }
      fader_group[current_channel].fader_position = square_volume;
      decreaseMacroVolume(number);
      break;
    }
    case WAVE: {
      if ((wave_volume - number) <= 0) {
        wave_volume = 0;
      } else {
        wave_volume -= number;
      }
      updateWaveVolume(wave_volume, duty_wave);
      fader_group[current_channel].fader_position = wave_volume;
      decreaseMacroVolume(number);
      break;
    }
    case NOISE: {
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
        updateSweepVolume(0);
      } else {
        updateSweepVolume(sweep_volume - number);
      }
    } else {
      if (sweep_volume + number > 15) {
        updateSweepVolume(15);
      } else {
        updateSweepVolume(sweep_volume + number);
      }
    }
    fader_group[0].fader_position = sweep_volume;
    moveFader(0);
  }
  if(volumeMacroStatus.square != 0 && current_channel != 1) {
    if (volumeMacroStatus.square == 1 ) {
      if (square_volume - number < 0) {
        updateSquareVolume(0);
      } else {
        updateSquareVolume(square_volume - number);
      }
    } else {
      if (square_volume + number > 15) {
        updateSquareVolume(15);
      } else {
        updateSquareVolume(square_volume + number);
      }
    }
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
    updateWaveVolume(wave_volume, duty_wave);
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
  // zombie mode volume https://gbdev.gg8.se/wiki/articles/Gameboy_sound_hardware#Obscure_Behavior
  if (volume > sweep_volume) {
    while (sweep_volume != volume) {
      NR12_REG = 0x08;
      sweep_volume++;
    }
  } else if (volume < sweep_volume) {
    while (sweep_volume != volume) {
    //afaikt this is the least clicky way to decrease volume
      __asm 
        ld a, #0x08
        .rept 15    // https://shop-pdp.net/ashtml/asmcro.htm#SECT6
        ldh (#0xFF12),a
        .endm
      __endasm;
      sweep_volume--;
    }
  } else {
    // do nada
  }
}

void updateSquareVolume(int volume) {
  if (volume > square_volume) {
    while (square_volume != volume) {
      NR22_REG = 0x08;
      square_volume++;
    }
  } else if (volume < square_volume) {
    while (square_volume != volume) {
    //afaikt this is the least clicky way to decrease volume
    // FF17 = NR22_REG  
      __asm 
        ld a, #0x08
        .rept 15    // https://shop-pdp.net/ashtml/asmcro.htm#SECT6
        ldh (#0xFF17),a
        .endm
      __endasm;
      square_volume--;
    }
  } else {
    // do nada
  }
  // UBYTE freqhigh;
  // UWORD freq;
  // NR22_REG = volume;
  // freq = (frequency_mode == 0) ? square_freq : frequencies[square_note];
  // freqhigh = (UBYTE)((freq & 0x0700)>>8);
  // NR24_REG = 0x80 | freqhigh; //restart the channel annars inte funkis
}

// this function lowers the amplitude the 4 bit samples in the wave pattern RAM
void updateWaveVolume(int volume, int sample_index) {

    if (volume == 0) {
      NR32_REG = 0x00;//turn off volume
      return;
    } else  {
      NR32_REG = 0x20; // highest volume
    }
    updateWaveToBeLoaded(volume, sample_index);
    loadWave();
}

// The volume dictates the offest in the saw sample array 
// and the hight of the square amplitude
void updateWaveToBeLoaded(int volume, int sample_index) {
  switch(wave_type)
  {
    case SQUAREWAVE: {
      loadSquareWave(volume, sample_index);
      break;
    }
    case SAW: {
      loadSawWave(volume);
      break;
    }
    case RAMP: {
      loadRampWave(volume);
      break;
    }
    case TRIANGLE: {
      loadTriangleWave(volume);
      break;
    }
    case SINE: {
      loadSineWave(volume);
      break;
    }
  }
}

// Routine for loading the square wave into wave memory area
void loadSquareWave(int volume, int sample_index) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &squareSamples[sample_index*8]; // points to selected duty
  unsigned char length = 8;
  while (length--) {
    unsigned char tmp = *src;
    UBYTE elbyto = (UBYTE) ((volume & tmp) | ((volume & tmp)<<4));
    *dst++ = elbyto;
    *dst2++ = elbyto;
    *src++;
  }
}

// Routine for loading the saw wave into wave memory area
void loadSawWave(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sawSamples[volume*8]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    *dst++ = *src;
    *dst2++ = *src++;
  }
}

// Routine for loading the saw wave into wave memory area
void loadTriangleWave(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &triangleSamples[volume*8]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    *dst++ = *src;
    *dst2++ = *src++;
  }
}

// Routine for loading the sine wave into wave memory area
void loadSineWave(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sineSamples[volume*8]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    *dst++ = *src;
    *dst2++ = *src++;
  }
}

// Routine for loading the ramp wave into wave memory area
void loadRampWave(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sawSamples[((volume+1)*8)-1]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    unsigned char tmp = *src;
    UBYTE losbitos = ((tmp & 0x0F) << 4) | ((tmp & 0xF0) >> 4);
    *dst++ = losbitos;
    *dst2++ = losbitos;
    *src--;
  }
}

// update the noise
void updateNoiseVolume(UBYTE volume) {
  NR42_REG = volume;
  //NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
  NR44_REG = 0x80;
}