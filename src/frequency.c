#include "frequency.h"
// Frequency page

void frequencyKeypadController() {
    if (KEY_TICKED(J_B)) {
        if(KEY_PRESSED(J_A)) {
            if (frequency_mode == 0) {
                frequency_mode = 1;
                flipHeader();
            } else {
                frequency_mode = 0;
                flipHeader();
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

void flipHeader() {

    DISPLAY_OFF;
    if (frequency_mode == 1) {
        set_bkg_tiles(0x00, 0x00, 9, 1, noteFreqText);
    } else {
        set_bkg_tiles(0x00, 0x00, 9, 1, freqFreqText);
    }
    SHOW_BKG;
    DISPLAY_ON;
}

// when flipping through notes
void noteMode() {
    if (KEY_TICKED(J_UP)) {
        if(KEY_PRESSED(J_A)) {
            increaseCurrentNote(12);
        } else if(KEY_PRESSED(J_B)) {
            increaseCurrentNote(3);
        } else {    
            increaseCurrentNote(1);
        }
        printCurrentChannelNote();  
    } else if (KEY_TICKED(J_DOWN)){
        if(KEY_PRESSED(J_A)) {
            decreaseCurrentNote(12);
        } else if (KEY_PRESSED(J_B)) {
            decreaseCurrentNote(3);
        }  else {
            decreaseCurrentNote(1);
        }
        printCurrentChannelNote();
    }
    else if (KEY_PRESSED(J_RIGHT)) {
        change_fader(J_RIGHT);
        waitpadup();
    } else if (KEY_PRESSED(J_LEFT)) {
        change_fader(J_LEFT);
        waitpadup();
    }
}

// when flipping through freqs
void frequencyMode() {
    if (KEY_TICKED(J_UP)) {
        if(KEY_PRESSED(J_A)) {
            increaseCurrentFreq(100);
        } else if(KEY_PRESSED(J_B)) {
            increaseCurrentFreq(10);
        } else {    
            increaseCurrentFreq(1);
        }
        printCurrentChannelFrequency(); 
    } else if (KEY_TICKED(J_DOWN)){
        if(KEY_PRESSED(J_A)) {
            decreaseCurrentFreq(100);
        } else if (KEY_PRESSED(J_B)) {
            decreaseCurrentFreq(10);
        }  else {
            decreaseCurrentFreq(1);
        }
        printCurrentChannelFrequency();
    } else if (KEY_PRESSED(J_RIGHT)) {
        change_fader(J_RIGHT);
        waitpadup();
    } else if (KEY_PRESSED(J_LEFT)) {
        change_fader(J_LEFT);
        waitpadup();
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
            updateSweepFreq(sweep_freq);
            break;
        }
        case 1: {
            if ((square_freq + amount) >= 2047) {
                square_freq = 2047;
            } else {
                square_freq = square_freq + amount;
            }
            updateSquareFreq(square_freq);
            break;
        }
        case 2: {
            if ((wave_freq + amount) >= 2047) {
                wave_freq = 2047;
            } else {
                wave_freq = wave_freq + amount;
            }
            updateWaveFreq(wave_freq);
            break;
        }
        case 3: {
            if ((noise_freq + amount) >= 15) {
                noise_freq = 15;
            } else {
                noise_freq = noise_freq + amount;
            }
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
            updateSweepFreq(sweep_freq);
            break;
        }
        case 1: {
            if ((uwti(square_freq) - amount) <= 0) {
                square_freq = 0;
            } else {
                square_freq = square_freq - amount;
            }
            updateSquareFreq(square_freq);
            break;
        }
        case 2: {
            if ((uwti(wave_freq) - amount) <= 0) {
                wave_freq = 0;
            } else {
                wave_freq = wave_freq - amount;
            }
            updateWaveFreq(wave_freq);
            break;
        }
        case 3: {
            if ((noise_freq - amount) <= 0) {
                noise_freq = 0;
            } else {
                noise_freq = noise_freq - amount;
            }
            updateNoiseFreq(noise_freq);
            break;
        }
    }
}

// update frequency 
void updateSweepFreq(UWORD new_freq) { 
    UBYTE freqlow, freqhigh;
    freqlow = (UBYTE)new_freq & 0xFF;
    freqhigh = (UBYTE)((new_freq & 0x0700)>>8);
    NR13_REG = freqlow;
    NR14_REG = 0x80 | freqhigh;
}

void updateSquareFreq(UWORD new_freq) {
    UBYTE freqlow, freqhigh;
    freqlow = (UBYTE)new_freq & 0xFF;
    freqhigh = (UBYTE)((new_freq & 0x0700)>>8);
    NR23_REG = freqlow;
    NR24_REG = 0x80 | freqhigh;
}

void updateWaveFreq(UWORD new_freq) {
    UBYTE freqlow, freqhigh;
    freqlow = (UBYTE)new_freq & 0xFF;
    freqhigh = (UBYTE)((new_freq & 0x0700)>>8);
    NR33_REG = freqlow; // Set lower byte of frequency.
    NR34_REG = 0x80 | freqhigh; // Set higher byte of frequency and start playback.
}

void updateNoiseFreq(UBYTE new_freq) {
    noiseStruct.clock_freq = new_freq;//: 4 bits;
    NR43_REG = noiseStruct.dividing_ratio | (noiseStruct.counter_step << 3) | (noiseStruct.clock_freq << 4);
}

// print function for frequency mode
void printCurrentChannelFrequency() {
    switch(current_channel)
    {
        case 0: {
            int value = sweep_freq;
            UINT8 position = 4;
            setCounterSprites(position, value);
            clearCounterValues(position);
            break;
        }
        case 1: {
            int value = square_freq;
            UINT8 position = 8;
            setCounterSprites(position, value);
            clearCounterValues(position);
            break;
        }
        case 2: {
            int value = wave_freq;
            UINT8 position = 12;
            setCounterSprites(position, value);
            clearCounterValues(position);
            break;
        }
        case 3: {
            int value = noise_freq;
            UINT8 position = 16;
            setCounterSprites(position, value);
            clearCounterValues(position);
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
* This sets a blank tile where there is no number. 
*/
void clearCounterValues(UINT8 position) {
    int val;
    if (current_channel == 0) {
        val = sweep_freq;
    } else if (current_channel == 1) {
        val = square_freq;
    } else if (current_channel == 2) {
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
            updateSweepFreq(frequencies[sweep_note]);
            break;
        }
        case 1: {
            if ((square_note + amount) >= 71) {
                square_note = 71;
            } else {
                square_note = square_note + amount;
            }
            updateSquareFreq(frequencies[square_note]);
            break;
        }
        case 2: {
            if ((wave_note + amount) >= 71) {
                wave_note = 71;
            } else {
                wave_note = wave_note + amount;
            }
            updateWaveFreq(frequencies[wave_note]);
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
            updateSweepFreq(frequencies[sweep_note]);
            break;
        }
        case 1: {
            if ((square_note - amount) <= 0) {
                square_note = 0;
            } else {
                square_note = square_note - amount;
            }
            updateSquareFreq(frequencies[square_note]);
            break;
        }
        case 2: {
            if ((wave_note - amount) <= 0) {
                wave_note = 0;
            } else {
                wave_note = wave_note - amount;
            }
            updateWaveFreq(frequencies[wave_note]);
            break;
        }
    }
}

/*
* Print function for notes
*/
void printCurrentChannelNote() {
    switch(current_channel)
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
    }
}