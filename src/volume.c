#include "volume.h"
// Volume page

const UBYTE wave_volumeValues[4] = {0x00, 0x60, 0x40, 0x20};
const UBYTE volumeFaderPositionWave[4] = {119, 98, 71, 41};

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
        updateNoiseVolume(15);
      } else {
        updateNoiseVolume(noise_volume + number);
      }
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
  if (volumeMacroStatus.sweep != 0 && current_channel != SWEEP) {
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
  if (volumeMacroStatus.square != 0 && current_channel != SQUARE) {
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
  if (volumeMacroStatus.wave != 0 && current_channel != WAVE) {
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
  if (volumeMacroStatus.noise != 0 && current_channel != NOISE) {
    if (volumeMacroStatus.noise == 1 && noise_volume != 15) {
      if (noise_volume + number > 15) {
        updateNoiseVolume(15);
      } else {
        updateNoiseVolume(noise_volume + number);
      }
    } else {
      if (noise_volume - number < 0) {
        updateNoiseVolume(0);
      } else {
        updateNoiseVolume(noise_volume - number);
      }
    }
    fader_group[3].fader_position = noise_volume;
    moveFader(3);
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
        updateNoiseVolume(0);
      } else {
        updateNoiseVolume(noise_volume - number);
      }
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
  if (volumeMacroStatus.sweep != 0 && current_channel != SWEEP) {
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
  if (volumeMacroStatus.square != 0 && current_channel != SQUARE) {
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
  if (volumeMacroStatus.wave != 0 && current_channel != WAVE) {
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
  if (volumeMacroStatus.noise != 0 && current_channel != NOISE) {
    if (volumeMacroStatus.noise == 1) {
      if (noise_volume - number < 0) {
        updateNoiseVolume(0);
      } else {
        updateNoiseVolume(noise_volume - number);
      }
    } else {
        if (noise_volume + number > 15) {
          updateNoiseVolume(15);
        } else {
          updateNoiseVolume(noise_volume + number);
        }
      fader_group[3].fader_position = noise_volume;
      moveFader(3);
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

// update the noise volume
void updateNoiseVolume(UBYTE volume) {
  //NR42_REG = volume;
  //NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
  //NR44_REG = 0x80;
  if (volume > noise_volume) {
    while (noise_volume != volume) {
      NR42_REG = 0x08;
      noise_volume++;
    }
  } else if (volume < noise_volume) {
    while (noise_volume != volume) {
    //afaikt this is the least clicky way to decrease volume
    // FF21 = NR42_REG
      __asm
        ld a, #0x08
        .rept 15    // https://shop-pdp.net/ashtml/asmcro.htm#SECT6
        ldh (#0xFF21),a
        .endm
      __endasm;
      noise_volume--;
    }
  } else {
    // do nada
  }
}

// The volume dictates the offest in the saw sample array 
// and the hight of the square amplitude
void updateWaveToBeLoaded(int volume, int sample_index) {
  switch(wave_type)
  {
    case SQUAREWAVE: {
      if (low_or_high_wave_freq == 0) { // 0 = low
        loadSquareWaveLow(volume, sample_index);
      } else {
        loadSquareWaveHigh(volume, sample_index);
      }
      break;
    }
    case SAW: {
      if (low_or_high_wave_freq == 0) {
        loadSawWaveLow(volume);
      } else {
        loadSawWaveHigh(volume);
      }
      break;
    }
    case RAMP: {
      if (low_or_high_wave_freq == 0) {
        loadRampWaveLow(volume);
      } else {
        loadRampWaveHigh(volume);
      }
      break;
    }
    case TRIANGLE: {
      if (low_or_high_wave_freq == 0) {
        loadTriangleWaveLow(volume);
      } else {
        loadTriangleWaveHigh(volume);
      }
      break;
    }
    case SINE: {
      if (low_or_high_wave_freq == 0) {
        loadSineWaveLow(volume);
      } else {
        loadSineWaveHigh(volume);
      }
      break;
    }
  }
}

// Routine for loading the square wave into wave memory area using
// double length wave forms that produce lower frequencys
void loadSquareWaveLow(int volume, int sample_index) {
  unsigned char *dst = &waveToBeLoaded[0];
  //unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &squareSamplesLow[sample_index*16]; // points to selected duty
  unsigned char length = 16;
  while (length--) {
    unsigned char tmp = *src;
    UBYTE elbyto = (UBYTE) ((volume & tmp) | ((volume & tmp)<<4));
    *dst++ = elbyto;
    //*dst2++ = elbyto;
    *src++;
  }
}

// Routine for loading the square wave into wave memory area
void loadSquareWaveHigh(int volume, int sample_index) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &squareSamplesHigh[sample_index*8]; // points to selected duty
  unsigned char length = 8;
  while (length--) {
    unsigned char tmp = *src;
    UBYTE elbyto = (UBYTE) ((volume & tmp) | ((volume & tmp)<<4));
    *dst++ = elbyto;
    *dst2++ = elbyto;
    *src++;
  }
}

// Routine for loading the LOW saw wave into wave memory area
void loadSawWaveLow(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  //unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sawSamplesLow[volume*16]; // Create pointer to the waveform
  unsigned char length = 16;
  while (length--) {
    *dst++ = *src++;
    //*dst2++ = *src++;
  }
}

// Routine for loading the LOW saw wave into wave memory area
void loadSawWaveHigh(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sawSamplesHigh[volume*8]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    *dst++ = *src;
    *dst2++ = *src++;
  }
}

// Routine for loading the LOW ramp wave into wave memory area
void loadRampWaveLow(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *src = &sawSamplesLow[((volume+1)*16)-1]; // Create pointer to the waveform
  unsigned char length = 16;
  while (length--) {
    unsigned char tmp = *src;
    UBYTE losbitos = ((tmp & 0x0F) << 4) | ((tmp & 0xF0) >> 4);
    *dst++ = losbitos;
    *src--;
  }
}

// High ramp loadery
void loadRampWaveHigh(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sawSamplesHigh[((volume+1)*8)-1]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    unsigned char tmp = *src;
    UBYTE losbitos = ((tmp & 0x0F) << 4) | ((tmp & 0xF0) >> 4);
    *dst++ = losbitos;
    *dst2++ = losbitos;
    *src--;
  }
}

// Triangle Low loader
void loadTriangleWaveLow(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *src = &triangleSamplesLow[volume*16];
  unsigned char length = 16;
  while (length--) {
    *dst++ = *src++;
  }
}

void loadTriangleWaveHigh(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &triangleSamplesHigh[volume*8];
  unsigned char length = 8;
  while (length--) {
    *dst++ = *src;
    *dst2++ = *src++;
  }
}

// Routine for loading the LOW sine wave into wave memory area
void loadSineWaveLow(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *src = &sineSamplesLow[volume*16]; // Create pointer to the waveform
  unsigned char length = 16;
  while (length--) {
    *dst++ = *src++;
  }
}

// sine high
void loadSineWaveHigh(int volume) {
  unsigned char *dst = &waveToBeLoaded[0];
  unsigned char *dst2 = &waveToBeLoaded[8];
  unsigned char *src = &sineSamplesHigh[volume*8]; // Create pointer to the waveform
  unsigned char length = 8;
  while (length--) {
    *dst++ = *src;
    *dst2++ = *src++;
  }
}
