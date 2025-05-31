#include <gb/gb.h>
#include "volume.h"
#include "duty.h"
#include "frequency.h"
#include "bpm.h"
#include "chord.h"
#include "midi.h"
#include "serial.h"
//#include <stdio.h> //to debug vals

uint8_t statusByte;
uint8_t addressByte;
uint8_t valueByte;
uint8_t capturedAddress;

UWORD coarse_sweep_value = 0;
UWORD coarse_square_value = 0;
UWORD coarse_wave_value = 0;

int old_sweep_midi_volume = 0;
int old_square_midi_volume = 0;
int old_wave_midi_volume = 0;
int old_noise_midi_volume = 0;

void updateMidiBuffer(void) {
  if (serialBufferReadPosition == serialBufferPosition) {
    return;
  }

  serialBufferReadPosition++; // unsigned overflow from 255 -> 0 is automatic.

  uint8_t byte = serialBuffer[serialBufferReadPosition];

  // STATUS BYTE
  if (byte & MIDI_STATUS_BIT) {
    if ((byte & MIDI_STATUS_SYSTEM) == MIDI_STATUS_SYSTEM) {
      return;
    }
    statusByte = byte;
    capturedAddress = 0;
    system_idle = 0;
    return;
  }

  // 2ND BYTE (note/CC control)
  if (!capturedAddress) {
    capturedAddress = 1;
    addressByte = byte;
    system_idle = 0;
    return;
  }

  // 3RD BYTE (velocity/value)
  capturedAddress = 0;
  valueByte = byte;
  system_idle = 0;

  switch ((statusByte >> 4) & 0x0F) {
  case MIDI_STATUS_CC:
    eventMidiCC();
    break;
  case MIDI_STATUS_NOTE_ON:
    eventMidiNoteOn();
    break;
  case MIDI_STATUS_NOTE_OFF:
    eventMidiNoteOff();
    break;
  /*case MIDI_STATUS_PC:
    asmEventMidiPC();
    break;*/
  }
}

// midi channel could be skipped
void eventMidiCC(void) {
  switch ((statusByte) & 0x0F) {
    case 0x00:
      eventMidiCCChannel();
      break;
  }
}

// cc number switch
void eventMidiCCChannel(void) {
  switch (addressByte) {
    case 0x01:
      sweepMidiVolume();
      break;
    case 0x02:
      squareMidiVolume();
      break;
    case 0x03:
      waveMidiVolume();
      break;
    case 0x04:
      noiseMidiVolume();
      break;
    case 0x05:
      sweepMidiDuty();
      break;
    case 0x06:
      squareMidiDuty();
      break;
    case 0x07:
      waveMidiDuty();
      break;
    case 0x08:
      noiseMidiDuty();
      break;
    case 0x09:
      sweepMidiFreqCoarse();
      break;
    case 0x0A:
      squareMidiFreqCoarse();
      break;
    case 0x0B:
      waveMidiFreqCoarse();
      break;
    case 0x0C:
      noiseMidiFreq();
      break;
    case 0x0D:
      sweepMidiFine();
      break;
    case 0x0E:
      squareMidiFine();
      break;
    case 0x0F:
      waveMidiFine();
      break;
    case 0x13:
      changeWaveTypeMidi();
      break;
    case 0x14:
      noiseMidiCounterStep();
      break;
    case 0x15:
      muteSweepMidi();
      break;
    case 0x16:
      muteSquareMidi();
      break;
    case 0x17:
      muteWaveMidi();
      break;
    case 0x18:
      muteNoiseMidi();
      break;
    case 0x1B:
      toggleHighLowWaveTypeMidi();
      break;
    case 0x1D:  // 29
      rewindToStartSeqMidi();
      break;
    case 0x20: // 32
      seqMidiStart();
      break;
    case 0x21:  // 33
      toggleChordOnOffMidi();
      break;
  }
}

// volume numbers
void sweepMidiVolume(void) {
  if (valueByte >= 15) {
    valueByte = 15;
  }
  //printf("sweepmidivolume");
  updateSweepVolume(valueByte);
}

void squareMidiVolume(void) {
  if (valueByte >= 15) {
    valueByte = 15;
  }
  updateSquareVolume(valueByte);
}

void waveMidiVolume(void) {
  if (valueByte >= 15) {
    valueByte = 15;
  }
  wave_volume = (int)valueByte;
  updateWaveVolume(valueByte, duty_wave);
}

void noiseMidiVolume(void) {
  if (valueByte >= 15) {
    valueByte = 15;
  }
  updateNoiseVolume(valueByte);
}

//////////// Duty
void sweepMidiDuty(void) {
  if (valueByte >= 3) {
    duty_sweep = 3;
  } else {
    duty_sweep = valueByte;
  }
  updateSweepDuty(dutyValues[duty_sweep]);
}

void squareMidiDuty(void) {
  if (valueByte >= 3) {
    duty_square = 3;
  } else {
    duty_square = valueByte;
  }
  updateSquareDuty(dutyValues[duty_square]);
}

void waveMidiDuty(void) {
  if (valueByte >= 3) {
    duty_wave = 3;
  } else {
    duty_wave = valueByte;
  }
  updateWaveDuty();
}

void noiseMidiDuty(void) {
  if (valueByte >= 6) {
    valueByte = 6;
  }
  updateNoiseDividingRatio(valueByte);
}

///// FFFRRREEEQQQ

// coarse will map the CC 7 bit value 0-127 over 16 steps
// gets jagged in higher frequencies
void sweepMidiFreqCoarse(void) {
  if (valueByte*16 >= 2047) {
    sweep_freq = 2047;
  } else {    
      sweep_freq = (UWORD)valueByte * 16; // have to do this for 16 mul otherwise it will wrap around 
      coarse_sweep_value = sweep_freq;
      // printf("%d\n", sweep_freq);
  }
  updateSweepFreq(0);
}

// Fine tuning of the sweep channels frequency
// will be based on latest coarse value. 64 and above add from coarse
// under 64 subtract, this will give a smoother 1 step per CC value change
void sweepMidiFine(void) {
   if (valueByte >= 64) {
      /*så typ
      coarse sweep value = 1000
      utifrån coarse sweep value dra ifrån under 64 och lägg till över
      coarse sweep val sätts bara när coarseSweep == 0
      så den är konstant och sätter värdet på sweep_freq utifrån om det skall läggas till
      eller dras ifrån*/
      if ((coarse_sweep_value + (valueByte - 64)) >= 2047) {
        sweep_freq = 2047;
      } else {
        sweep_freq = coarse_sweep_value + (valueByte - 64);
      }
      //printf("%d\n", sweep_freq);
      //sweep_freq += valueByte - 64;
    } else {      
      sweep_freq = coarse_sweep_value - (64 - valueByte);
    }
    updateSweepFreq(0);
}

void squareMidiFreqCoarse(void) {
  if (valueByte*16 >= 2047) {
    square_freq = 2047;
  } else {
    square_freq = (UWORD)valueByte * 16;
    coarse_square_value = square_freq;
  }
  updateSquareFreq(0);
}

void squareMidiFine(void) {
  if (valueByte >= 64) {
    if ((coarse_square_value + (valueByte - 64)) >= 2047) {
      square_freq = 2047;
    } else {
      square_freq = coarse_square_value + (valueByte - 64);
    }
    //printf("%d\n", square_freq);
  } else {      
    square_freq = coarse_square_value - (64 - valueByte);
  }
  updateSquareFreq(0);
}

void waveMidiFreqCoarse(void) {
   if (valueByte*16 >= 2047) {
    wave_freq = 2047;
  } else {
    wave_freq = (UWORD)valueByte * 16;
    coarse_wave_value = wave_freq;
  }
  updateWaveFreq(0);
}

void waveMidiFine(void) {
  if (valueByte >= 64) {
    if ((coarse_wave_value + (valueByte - 64)) >= 2047) {
      wave_freq = 2047;
    } else {
      wave_freq = coarse_wave_value + (valueByte - 64);
    }
    //printf("%d\n", square_freq);
  } else {      
    wave_freq = coarse_wave_value - (64 - valueByte);
  }
  updateWaveFreq(0);
}

void noiseMidiFreq(void) {
  if (valueByte >= 15) {
    noise_freq = 15;
  } else {
    noise_freq = valueByte;
  }
  updateNoiseFreq(noise_freq);
}

// choose wave type
void changeWaveTypeMidi(void) {
  if (valueByte > 0) {
    changeWaveType();
  }
}

// choose if high or low wave types
void toggleHighLowWaveTypeMidi(void) {
  if (valueByte > 0) {
    low_or_high_wave_freq = 1;
    updateWaveToBeLoaded(wave_volume, duty_wave);
    loadWave();
  } else {
    low_or_high_wave_freq = 0;
    updateWaveToBeLoaded(wave_volume, duty_wave);
    loadWave();
  }
}

void noiseMidiCounterStep(void) {
  if (valueByte != 0) {
    noiseStruct.counter_step = 1; // this is flipped in the function... @todo refactor
    changNoiseCounterStep();
  } else {
    noiseStruct.counter_step = 0;
    changNoiseCounterStep();
  }
}

// Mute channels

void muteSweepMidi(void) {
  if (valueByte != 0) {
    old_sweep_midi_volume = sweep_volume;
    updateSweepVolume(0);
  } else {
    // volume globals are set in updateXVolume functions
    updateSweepVolume(old_sweep_midi_volume);
  }
}

void muteSquareMidi(void) {
  if (valueByte != 0) {
    old_square_midi_volume = square_volume;
    updateSquareVolume(0);
  } else {
    updateSquareVolume(old_square_midi_volume);
  }
}

void muteWaveMidi(void) {
  if (valueByte != 0) {
    old_wave_midi_volume = wave_volume;
    wave_volume = 0; // @todo this global is not handled like the others
    updateWaveVolume(wave_volume, duty_wave);
  } else {
    wave_volume = old_wave_midi_volume;
    updateWaveVolume(wave_volume, duty_wave);
  }
}

void muteNoiseMidi(void) {
  if (valueByte != 0) {
    old_noise_midi_volume = noise_volume;
    updateNoiseVolume(0);
  } else {
    updateNoiseVolume(old_noise_midi_volume);
  }
}

// sequencer stuff

void seqMidiStart(void) {
  if (valueByte != 0) {
    play_chord_step = 1;
    printChordSteppaOnOff();
  } else {
    play_chord_step = 0;
    printChordSteppaOnOff();
  }
}

// jump to first step of seqencer
void rewindToStartSeqMidi(void) {
  current_seq_chord = 0;
}

// General

// turn chord mode on off
void toggleChordOnOffMidi(void) {
  if (valueByte != 0) { // chord on
    chord_on = 1;
    frequency_mode = 1;
    low_or_high_wave_freq = 1;
    updateWaveVolume(wave_volume, duty_wave);
    changeNotes();
  } else { // chord off
    chord_on = 0;
    frequency_mode = 0;
    // @todo save state of low_or_high_wave_freq and restore
  }
}

// Note on off
// this plays the chords in the chord slots
// using white keys from middle c
void eventMidiNoteOn(void) {
  switch (addressByte) {
    case 0x3C: { // C4 is first slot
      setCurrentChordSteppaStep(0);
      playCurrentStep();
      break;
    }
    case 0x3E: { // next white key D is second
      setCurrentChordSteppaStep(1);
      playCurrentStep();
      break;
    }
    case 0x40: { // and so on
      setCurrentChordSteppaStep(2);
      playCurrentStep();
      break;
    }
    case 0x41: {
      setCurrentChordSteppaStep(3);
      playCurrentStep();
      break;
    }
    case 0x43: {
      setCurrentChordSteppaStep(4);
      playCurrentStep();
      break;
    }
    case 0x45: {
      setCurrentChordSteppaStep(5);
      playCurrentStep();
      break;
    }
    case 0x47: {
      setCurrentChordSteppaStep(6);
      playCurrentStep();
      break;
    }
    case 0x48: {
      setCurrentChordSteppaStep(7);
      playCurrentStep();
      break;
    }
  }
}

void eventMidiNoteOff(void) {
  // in the future this might come in handy
}