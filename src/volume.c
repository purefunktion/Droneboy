#include "volume.h"
// Volume page

// Keypad 
void volumeKeypadController() {
  if (KEY_PRESSED(J_A)) {
    // kanske selecta flerän en fader
    waitpadup();  // Wait until the button is released
  } else if (KEY_PRESSED(J_B)) {
    //mute?
    waitpadup();
  } else if (KEY_PRESSED(J_UP)) {
    increaseVolume();
    moveFader();
    waitpadup();
  } else if (KEY_PRESSED(J_DOWN)) {
    decreaseVolume();
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

// Increase volume 1 step at the time
void increaseVolume() {
  switch(current_channel)
  {
    case 0: {
      if (sweep_volume == 15) {
        break;
      }
      sweep_volume = sweep_volume + 1;
      updateSweepVolume(volumeValues[sweep_volume]);
      fader_group[current_channel].fader_position = sweep_volume;
      break;
    }
    case 1: {
      if (square_volume == 15) {
        break;
      }
      square_volume = square_volume + 1;
      updateSquareVolume(volumeValues[square_volume]);
      fader_group[current_channel].fader_position = square_volume;
      break;
    }
    case 2: {
      if (wave_volume == 15) {
        break;
      }
      //From highest to lowest the values are 0x20, 0x40, 0x60, 0x00.
      wave_volume = wave_volume + 1;
      updateWaveVolume(wave_volume, duty_wave*16);
      fader_group[current_channel].fader_position = wave_volume;
      break;
    }
    case 3: {
      if (noise_volume == 15) {
        break;
      }
      noise_volume = noise_volume + 1;
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[current_channel].fader_position = noise_volume;
      break;
    }
  }
}

// ..and decrease
void decreaseVolume() {
  switch(current_channel)
  {
    case 0: {
      if (sweep_volume == 0b0000) {
        break;
      }
      sweep_volume = sweep_volume - 1;
      updateSweepVolume(volumeValues[sweep_volume]);
      fader_group[current_channel].fader_position = sweep_volume;
      break;
    }
    case 1: {
      if (square_volume == 0) {
        break;
      }
      square_volume = square_volume - 1;
      updateSquareVolume(volumeValues[square_volume]);
      fader_group[current_channel].fader_position = square_volume;
      break;
    }
    case 2: {
      if (wave_volume == 0) {
        break;
      }
      wave_volume = wave_volume - 1;
      updateWaveVolume(wave_volume, duty_wave*16);
      fader_group[current_channel].fader_position = wave_volume;
      break;
    }
    case 3: {
      if (noise_volume == 0) {
        updateNoiseVolume(noise_volume);
        break;
      }
      noise_volume = noise_volume - 1;
      updateNoiseVolume(volumeValues[noise_volume]);
      fader_group[current_channel].fader_position = noise_volume;
      break;
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

// updae the noise
void updateNoiseVolume(UBYTE volume) {
  NR42_REG = volume;
  NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
  NR44_REG = 0x80;
}