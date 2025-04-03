#include "frequency.h"
// Frequency page

void frequencyKeypadController() {
    if (KEY_TICKED(J_B)) {
        if(KEY_PRESSED(J_A)) {
            if (frequency_mode == 0) {
                frequency_mode = 1;
                flipHeader();
                domacro = 0;
                up_sweep_counter = 0;
            } else {
                frequency_mode = 0;
                flipHeader();
                domacro = 0;
                up_sweep_counter = 0;
            }
            updateFaderMarker();
            waitpadup();
        }
    }
    switch(frequency_mode)
    {
        case 0: {
                frequencyMode();
                break;
        }
        case 1: {
                noteMode();
                break;
        }
    }
}

// show/hide frequency/note
void flipHeader() {
    if (frequency_mode == 1) {
        set_bkg_tiles(0x00, 0x00, 9, 1, noteFreqText);
    } else {
        set_bkg_tiles(0x00, 0x00, 9, 1, freqFreqText);
    }
}

// when flipping through notes
void noteMode() {

    if (KEY_RELEASED(J_A) && domacro == 1) {
        placeMacroMarker();
        domacro = 0;
    }

    if (KEY_TICKED(J_UP)) {
        if(KEY_PRESSED(J_A)) {
            increaseCurrentNote(12);
            domacro = 0;
        } else if(KEY_PRESSED(J_B)) {
            increaseCurrentNote(3);
        } else {    
            increaseCurrentNote(1);
        }
        printChannelNote(current_channel);  
    } else if (KEY_TICKED(J_DOWN)){
        if(KEY_PRESSED(J_A)) {
            decreaseCurrentNote(12);
            domacro = 0;
        } else if (KEY_PRESSED(J_B)) {
            decreaseCurrentNote(3);
        }  else {
            decreaseCurrentNote(1);
        }
        printChannelNote(current_channel);
    } else if (KEY_PRESSED(J_RIGHT)) {
        change_fader(J_RIGHT);
        waitpadup();
    } else if (KEY_PRESSED(J_LEFT)) {
        change_fader(J_LEFT);
        waitpadup();
    } else if (KEY_TICKED(J_A)) {
        if(KEY_PRESSED(J_B)) { // copy over to freq
            domacro = 0;
            copyNoteToFreq();
            printChannelFrequency(current_channel);
        } else {
            domacro = 1;
        }
    }
}

// when flipping through freqs
void frequencyMode() {

    // set macro marker
    if (KEY_RELEASED(J_A) && domacro == 1) {
        placeMacroMarker();
        domacro = 0;
    }
    // sweep up done
    if (KEY_RELEASED(J_UP) && up_sweep_counter > 0) {
        up_sweep_counter = 0;
    }

    // sweep down done
    if (KEY_RELEASED(J_DOWN) && down_sweep_counter > 0) {
        down_sweep_counter = 0;
    }

    if (KEY_TICKED(J_UP)) {
        if(KEY_PRESSED(J_A)) {
            increaseCurrentFreq(100);
            domacro = 0;
        } else if(KEY_PRESSED(J_B)) {
            increaseCurrentFreq(10);
        } else {    
            increaseCurrentFreq(1);
        }
        printChannelFrequency(current_channel);
    } else if (KEY_TICKED(J_DOWN)){
        if(KEY_PRESSED(J_A)) {
            decreaseCurrentFreq(100);
            domacro = 0;
        } else if (KEY_PRESSED(J_B)) {
            decreaseCurrentFreq(10);
        }  else {
            decreaseCurrentFreq(1);
        }
        printChannelFrequency(current_channel);
    } else if (KEY_PRESSED(J_RIGHT)) {
        change_fader(J_RIGHT);
        waitpadup();
    } else if (KEY_PRESSED(J_LEFT)) {
        change_fader(J_LEFT);
        waitpadup();
    } else if (KEY_TICKED(J_A)) {
        domacro = 1;
    }

    // continuous up sweep
    if (KEY_PRESSED(J_UP)) {
        // counter to check if just a button press
        if (up_sweep_counter == 30) {
            increaseCurrentFreq(1);
            printChannelFrequency(current_channel);
        } else {
            up_sweep_counter++;
        }
    }

    // continuous down sweep
    if (KEY_PRESSED(J_DOWN)) {
        // counter to check if just a button press
        if (down_sweep_counter == 30) {
            decreaseCurrentFreq(1);
            printChannelFrequency(current_channel);
        } else {
            down_sweep_counter++;
        }
    }
}

// this will copy the frequency of the current note
// to the global freqency variable, for easier manipulation. 
void copyNoteToFreq() {
    switch(current_channel)
    {
        case 0: {
            sweep_freq = frequencies[sweep_note];
            break;
        }
        case 1: {
            square_freq = frequencies[square_note];
            break;
        }
        case 2: {
            wave_freq = frequencies[wave_note];
            break;
        }
       /* case 3: {
            noise_freq = noiseNotesFrequencies[noise_note];
            break;
        }*/
    }
}

// increase freq by amount(set in frequencyMode)
void increaseCurrentFreq(int amount) {
    switch(current_channel)
    {
        case 0: {
            if ((sweep_freq + amount) >= 2047) {
                sweep_freq = 2047;
            } else {
                sweep_freq = sweep_freq + amount;
            }
            increaseMacroFreq(amount);  
            updateSweepFreq(0);
            break;
        }
        case 1: {
            if ((square_freq + amount) >= 2047) {
                square_freq = 2047;
            } else {
                square_freq = square_freq + amount;
            }
            increaseMacroFreq(amount);
            updateSquareFreq( 0);
            break;
        }
        case 2: {
            if ((wave_freq + amount) >= 2047) {
                wave_freq = 2047;
            } else {
                wave_freq = wave_freq + amount;
            }
            increaseMacroFreq(amount);
            updateWaveFreq(0);
            break;
        }
        case 3: {
            if ((noise_freq + amount) >= 15) {
                noise_freq = 15;
            } else {
                noise_freq = noise_freq + amount;
            }
            increaseMacroFreq(amount);
            updateNoiseFreq(noise_freq);
            break;
        }
    }
}

/**
* Below zero check for unsigned word.
**/
int uwti(UWORD num) {
    int lol = (int) num;
    return lol; 
}

// decrese current frequency by amount(again set in frequencyMode)
void decreaseCurrentFreq(int amount) {
    switch(current_channel)
    {
        case 0: {
            if ((uwti(sweep_freq) - amount) <= 0) {
                sweep_freq = 0;
            } else {
                sweep_freq = sweep_freq - amount;
            }
            decreaseMacroFreq(amount);
            updateSweepFreq(0);
            break;
        }
        case 1: {
            if ((uwti(square_freq) - amount) <= 0) {
                square_freq = 0;
            } else {
                square_freq = square_freq - amount;
            }
            decreaseMacroFreq(amount);
            updateSquareFreq(0);
            break;
        }
        case 2: {
            if ((uwti(wave_freq) - amount) <= 0) {
                wave_freq = 0;
            } else {
                wave_freq = wave_freq - amount;
            }
            decreaseMacroFreq(amount);
            updateWaveFreq(0);
            break;
        }
        case 3: {
            if ((noise_freq - amount) <= 0) {
                noise_freq = 0;
            } else {
                noise_freq = noise_freq - amount;
            }
            decreaseMacroFreq(amount);
            updateNoiseFreq(noise_freq);
            break;
        }
    }
}

// update frequency 
// you dont have to retrigger to update frequency
void updateSweepFreq(int retrigger) { 
    UBYTE freqlow, freqhigh;
    if (frequency_mode == 0) {
        freqlow = (UBYTE)sweep_freq & 0xFF;
        freqhigh = (UBYTE)((sweep_freq & 0x0700)>>8);
    } else {
        freqlow = (UBYTE)frequencies[sweep_note] & 0xFF;
        freqhigh = (UBYTE)((frequencies[sweep_note] & 0x0700)>>8); 
    }
    NR13_REG = freqlow;
    if (retrigger == 1) {
        NR12_REG = volumeValues[sweep_volume]; // set the current volume
        NR14_REG = 0x80 | freqhigh; // retrigger 
    } else {
        NR14_REG = freqhigh;
    }
}

void updateSquareFreq(int retrigger) {
    UBYTE freqlow, freqhigh;
    if (frequency_mode == 0) {
        freqlow = (UBYTE)square_freq & 0xFF;
        freqhigh = (UBYTE)((square_freq & 0x0700)>>8);
    } else {
        freqlow = (UBYTE)frequencies[square_note] & 0xFF;
        freqhigh = (UBYTE)((frequencies[square_note] & 0x0700)>>8); 
    }
    NR23_REG = freqlow;
    if (retrigger == 1) {
        NR22_REG = volumeValues[square_volume];
        NR24_REG = 0x80 | freqhigh;
    } else {
        NR24_REG = freqhigh;
    }
}

void updateWaveFreq(int retrigger) {
    //NR30_REG = 0x00; // this must be done before retriggering ...
    UBYTE freqlow, freqhigh;
    if (frequency_mode == 0) {
        freqlow = (UBYTE)wave_freq & 0xFF;
        freqhigh = (UBYTE)((wave_freq & 0x0700)>>8);
    } else {
        freqlow = (UBYTE)frequencies[wave_note] & 0xFF;
        freqhigh = (UBYTE)((frequencies[wave_note] & 0x0700)>>8); 
    }
    //NR30_REG |= 0x80; // ... or the wave ram will overwrite itself 
    NR33_REG = freqlow; // Set lower byte of frequency.
    if (retrigger == 1) {
        NR34_REG = 0x80 | freqhigh;
    } else {
        NR34_REG = freqhigh; // Set higher byte of frequency and start playback.
    }
}

void updateNoiseFreq(UBYTE new_freq) {
    noiseStruct.clock_freq = new_freq;//: 4 bits;
    NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
}

void updateNoiseNoteFreq(UBYTE new_freq) {
    noiseStruct.dividing_ratio = (int)(new_freq & 0x07);
    noiseStruct.counter_step = (int)((new_freq & (0x1 << 3)) >> 3);
    noiseStruct.clock_freq = (int) ((new_freq & 0xF0) >> 4);
    noise_freq = noiseStruct.clock_freq;
    duty_fader_group[3].fader_position = noiseStruct.dividing_ratio;
    NR43_REG = new_freq;
}

// print function for frequency mode
void printChannelFrequency(int channel) {
    switch(channel)
    {
        case 0: {
            int value = sweep_freq;
            UINT8 position = 4;
            setCounterSprites(position, value);
            clearCounterValues(position, channel);
            break;
        }
        case 1: {
            int value = square_freq;
            UINT8 position = 8;
            setCounterSprites(position, value);
            clearCounterValues(position, channel);
            break;
        }
        case 2: {
            int value = wave_freq;
            UINT8 position = 12;
            setCounterSprites(position, value);
            clearCounterValues(position, channel);
            break;
        }
        case 3: {
            int value = noise_freq;
            UINT8 position = 16;
            setCounterSprites(position, value);
            clearCounterValues(position, channel);
            break;
        }
    }
}

/*
* This will setup the number tiles to display a value(frequency)
*/
void setCounterSprites(UINT8 position, int value) {
    if (value == 0) {
        set_sprite_tile(position, 4);
    } else {
        while (value > 0) {
            int digit = value % 10;
            set_sprite_tile(position, digit+4);
            value /= 10;
            position++;
        }
    }
}

/*
* Set the note tiles
*/
void setNoteSprites(int position, int note_value) {
    int i = 4;
    while(i)
    {
        switch(noteNames[note_value][i-1])
        {
            case 32: //space 
                set_sprite_tile(position, 21);
                break;
            case 35: //#
                set_sprite_tile(position, 37);
                break;
            case 51: //3 
                set_sprite_tile(position, 7);
                break;
            case 52: //4
                set_sprite_tile(position, 8);
                break;
            case 53: //5
                set_sprite_tile(position, 9);
                break;
            case 54: //6 
                set_sprite_tile(position, 10);
                break;
            case 55: //7 
                set_sprite_tile(position, 11);
                break;
            case 56: //8 
                set_sprite_tile(position, 12);
                break;
            case 65: //A
                set_sprite_tile(position, 30);
                break;
            case 66: //B 
                set_sprite_tile(position, 31);
                break;
            case 67: //C
                set_sprite_tile(position, 32);
                break;
            case 68: //D
                set_sprite_tile(position, 33);
                break;
            case 69: //E
                set_sprite_tile(position, 34);
                break;
            case 70: //F
                set_sprite_tile(position, 35);
                break;
            case 71: //G
                set_sprite_tile(position, 36);
                break;
        }
        i--;
        position++;
    }
}

/*
* Print function for notes
*/
void printChannelNote(int channel) {
  switch(channel)
  {
    case 0: {
        setNoteSprites(20, sweep_note);
        break;
    }
    case 1: {
        setNoteSprites(24, square_note);
        break;
    }
    case 2: {
        setNoteSprites(28, wave_note);
        break;
    }
    case 3: {
        setNoteSprites(32, noiseNoteNameIndex[noise_note]);
        break;
    }
  }
}

/*
* This sets a blank tile where there is no number. 
*/
void clearCounterValues(UINT8 position, int channel) {
    int val;
    if (channel == 0) {
        val = sweep_freq;
    } else if (channel == 1) {
        val = square_freq;
    } else if (channel == 2) {
        val = wave_freq;
    } else {
        val = noise_freq;
    }
    if (val < 1000) {
        set_sprite_tile(position+3, 21);
    }
    if (val < 100) {
        set_sprite_tile(position+2, 21);
    }
    if (val < 10) {
        set_sprite_tile(position+1, 21);
    }
}

/*
* Increase current channel note value frequency
*/
void increaseCurrentNote(int amount) {
    switch(current_channel)
    {
        case 0: {
            if ((sweep_note + amount) >= 71) {
                sweep_note = 71;
            } else {
                sweep_note = sweep_note + amount;
            }    
            increaseMacroNote(amount);   
            updateSweepFreq(0);
            break;
        }
        case 1: {
            if ((square_note + amount) >= 71) {
                square_note = 71;
            } else {
                square_note = square_note + amount;
            }
            increaseMacroNote(amount);
            updateSquareFreq(0);
            break;
        }
        case 2: {
            if ((wave_note + amount) >= 71) {
                wave_note = 71;
            } else {
                wave_note = wave_note + amount;
            }
            increaseMacroNote(amount);
            updateWaveFreq(0);
            break;
        }
        case 3: {
            if ((noise_note + amount) >= 5) {
                noise_note = 5;
            } else {
                noise_note = noise_note + amount;
            }
            increaseMacroNote(amount);
            updateNoiseNoteFreq(noiseNotesFrequencies[noise_note]);
            break;
        }
    }
}

/*
* Decrease current channel note value frequency
*/
void decreaseCurrentNote(int amount) {
    switch(current_channel)
    {
        case 0: {
            if ((sweep_note - amount) <= 0) {
                sweep_note = 0;
            } else {
                sweep_note = sweep_note - amount;
            }
            decreaseMacroNote(amount);
            updateSweepFreq(0);
            break;
        }
        case 1: {
            if ((square_note - amount) <= 0) {
                square_note = 0;
            } else {
                square_note = square_note - amount;
            }
            decreaseMacroNote(amount);
            updateSquareFreq(0);
            break;
        }
        case 2: {
            if ((wave_note - amount) <= 0) {
                wave_note = 0;
            } else {
                wave_note = wave_note - amount;
            }
            decreaseMacroNote(amount);
            updateWaveFreq(0);
            break;
        }
        case 3: {
            if ((noise_note - amount) <= 0) {
                noise_note = 0;
            } else {
                noise_note = noise_note - amount;
            }
            decreaseMacroNote(amount);
            updateNoiseNoteFreq(noiseNotesFrequencies[noise_note]);
            break;
        }
    }
}

// Increase the macro enabled channels freq
void increaseMacroNote(int number) {
  if(current_channel != 0 && freqMacroStatus.sweep != 0) {
    if (freqMacroStatus.sweep == 1 ) { // regular macro marker
      if ((sweep_note + number) >= 71) {
            sweep_note = 71;
        } else {
            sweep_note = sweep_note + number;
        }
    } else if (freqMacroStatus.sweep == 2) { // inverted macro
      if ((sweep_note - number) <= 0) {
            sweep_note = 0;
        } else {
            sweep_note = sweep_note - number;
        }
    }
    updateSweepFreq(0);
    printChannelNote(0);
  }
  if(current_channel != 1 && freqMacroStatus.square != 0) {
    if (freqMacroStatus.square == 1 ) { // regular macro marker
      if ((square_note + number) >= 71) {
            square_note = 71;
        } else {
            square_note = square_note + number;
        }
    } else if (freqMacroStatus.square == 2) { // inverted macro
      if ((square_note - number) <= 0) {
            square_note = 0;
        } else {
            square_note = square_note - number;
        }
    }
    updateSquareFreq(0);
    printChannelNote(1);
  }
  if(current_channel != 2 && freqMacroStatus.wave != 0) {
    if (freqMacroStatus.wave == 1 ) { // regular macro marker
      if ((wave_note + number) >= 71) {
            wave_note = 71;
        } else {
            wave_note = wave_note + number;
        } 
    } else if (freqMacroStatus.wave == 2) { // inverted macro
      if ((wave_note - number) <= 0) {
            wave_note = 0;
        } else {
            wave_note = wave_note - number;
        }
    }
    updateWaveFreq(0);
    printChannelNote(2);
  }
  if(current_channel != 3 && freqMacroStatus.noise != 0) {
    if (freqMacroStatus.noise == 1 ) { // regular macro marker
      if ((noise_note + number) >= 5) {
            noise_note = 5;
        } else {
            noise_note = noise_note + number;
        }
    } else if (freqMacroStatus.noise == 2) { // inverted macro
      if ((noise_note - number) <= 0) {
            noise_note = 0;
        } else {
            noise_note = noise_note - number;
        }
    }
    updateNoiseNoteFreq(noiseNotesFrequencies[noise_note]);
    printChannelNote(3);
  }
}

// Increase the macro enabled channels freq
void decreaseMacroNote(int number) {
  if(current_channel != 0 && freqMacroStatus.sweep != 0) {
    if (freqMacroStatus.sweep == 1 ) { // regular macro marker
      if ((sweep_note - number) <= 0) {
            sweep_note = 0;
        } else {
            sweep_note = sweep_note - number;
        }
    } else if (freqMacroStatus.sweep == 2) { // inverted macro
      if ((sweep_note + number) >= 71) {
            sweep_note = 71;
        } else {
            sweep_note = sweep_note + number;
        } 
    }
    updateSweepFreq(0);
    printChannelNote(0);
  }
  if(current_channel != 1 && freqMacroStatus.square != 0) {
    if (freqMacroStatus.square == 1 ) { // regular macro marker
       if ((square_note - number) <= 0) {
            square_note = 0;
        } else {
            square_note = square_note - number;
        }
    } else if (freqMacroStatus.square == 2) { // inverted macro
        if ((square_note + number) >= 71) {
            square_note = 71;
        } else {
            square_note = square_note + number;
        }
    }
    updateSquareFreq( 0);
    printChannelNote(1);
  }
  if(current_channel != 2 && freqMacroStatus.wave != 0) {
    if (freqMacroStatus.wave == 1 ) { // regular macro marker
      if ((wave_note - number) <= 0) {
            wave_note = 0;
        } else {
            wave_note = wave_note - number;
        }
    } else if (freqMacroStatus.wave == 2) { // inverted macro
      if ((wave_note + number) >= 71) {
            wave_note = 71;
        } else {
            wave_note = wave_note + number;
        }
    }
    updateWaveFreq(0);
    printChannelNote(2);
  }
  if(current_channel != 3 && freqMacroStatus.noise != 0) {
    if (freqMacroStatus.noise == 1 ) { // regular macro marker
      if ((noise_note - number) <= 0) {
            noise_note = 0;
        } else {
            noise_note = noise_note - number;
        }
    } else if (freqMacroStatus.noise == 2) { // inverted macro
      if ((noise_note + number) >= 5) {
            noise_note = 5;
        } else {
            noise_note = noise_note + number;
        }
    }
    updateNoiseNoteFreq(noiseNotesFrequencies[noise_note]);
    printChannelNote(3);
  }
}

// Increase the macro enabled channels freq
void increaseMacroFreq(int number) {
  if(current_channel != 0 && freqMacroStatus.sweep != 0) {
    if (freqMacroStatus.sweep == 1 ) { // regular macro marker
      if ((sweep_freq + number) >= 2047) {
            sweep_freq = 2047;
        } else {
            sweep_freq = sweep_freq + number;
        }
    } else if (freqMacroStatus.sweep == 2) { // inverted macro
      if ((uwti(sweep_freq) - number) <= 0) {
            sweep_freq = 0;
        } else {
            sweep_freq = sweep_freq - number;
        }
    }
    updateSweepFreq(0);
    printChannelFrequency(0);
  }
  if(current_channel != 1 && freqMacroStatus.square != 0) {
    if (freqMacroStatus.square == 1 ) { // regular macro marker
      if ((square_freq + number) >= 2047) {
            square_freq = 2047;
        } else {
            square_freq = square_freq + number;
        }
    } else if (freqMacroStatus.square == 2) { // inverted macro
      if ((uwti(square_freq) - number) <= 0) {
            square_freq = 0;
        } else {
            square_freq = square_freq - number;
        }
    }
    updateSquareFreq( 0);
    printChannelFrequency(1);
  }
  if(current_channel != 2 && freqMacroStatus.wave != 0) {
    if (freqMacroStatus.wave == 1 ) { // regular macro marker
      if ((wave_freq + number) >= 2047) {
            wave_freq = 2047;
        } else {
            wave_freq = wave_freq + number;
        }
    } else if (freqMacroStatus.wave == 2) { // inverted macro
      if ((uwti(wave_freq) - number) <= 0) {
            wave_freq = 0;
        } else {
            wave_freq = wave_freq - number;
        }
    }
    updateWaveFreq(0);
    printChannelFrequency(2);
  }
  if(current_channel != 3 && freqMacroStatus.noise != 0) {
    if (freqMacroStatus.noise == 1 ) { // regular macro marker
      if ((noise_freq + number) >= 15) {
            noise_freq = 15;
        } else {
            noise_freq = noise_freq + number;
        }
    } else if (freqMacroStatus.noise == 2) { // inverted macro
      if ((uwti(noise_freq) - number) <= 0) {
            noise_freq = 0;
        } else {
            noise_freq = noise_freq - number;
        }
    }
    updateNoiseFreq(noise_freq);
    printChannelFrequency(3);
  }
}

// Decrease the macro enabled channels freq
void decreaseMacroFreq(int number) {
  if(current_channel != 0 && freqMacroStatus.sweep != 0) {
    if (freqMacroStatus.sweep == 1 ) { // regular macro marker
      if ((uwti(sweep_freq) - number) <= 0) {
        sweep_freq = 0;
      } else {
        sweep_freq = sweep_freq - number;
      }
    } else if (freqMacroStatus.sweep == 2) { // inverted macro
      if ((sweep_freq + number) >= 2047) {
        sweep_freq = 2047;
      } else {
        sweep_freq = sweep_freq + number;
      }
    }
    updateSweepFreq(0);
    printChannelFrequency(0);
  }
  if(current_channel != 1 && freqMacroStatus.square != 0) {
    if (freqMacroStatus.square == 1 ) { // regular macro marker
      if ((uwti(square_freq) - number) <= 0) {
         square_freq = 0;
      } else {
        square_freq = square_freq - number;
      }
    } else if (freqMacroStatus.square == 2) { // inverted macro
      if ((square_freq + number) >= 2047) {
        square_freq = 2047;
      } else {
        square_freq = square_freq + number;
      }
    }
    updateSquareFreq(0);
    printChannelFrequency(1);
  }
  if(current_channel != 2 && freqMacroStatus.wave != 0) {
    if (freqMacroStatus.wave == 1 ) { // regular macro marker
      if ((uwti(wave_freq) - number) <= 0) {
        wave_freq = 0;
      } else {
        wave_freq = wave_freq - number;
      }
    } else if (freqMacroStatus.wave == 2) { // inverted macro
      if ((wave_freq + number) >= 2047) {
        wave_freq = 2047;
      } else {
        wave_freq = wave_freq + number;
      }
    }
    updateWaveFreq(0);
    printChannelFrequency(2);
  }
  if(current_channel != 3 && freqMacroStatus.noise != 0) {
    if (freqMacroStatus.noise == 1 ) { // regular macro marker
      if ((uwti(noise_freq) - number) <= 0) {
        noise_freq = 0;
      } else {
        noise_freq = noise_freq - number;
      }
    } else if (freqMacroStatus.noise == 2) { // inverted macro
      if ((noise_freq + number) >= 15) {
        noise_freq = 15;
      } else {
        noise_freq = noise_freq + number;
      }
    }
    updateNoiseFreq(noise_freq);
    printChannelFrequency(3);
  }
}
