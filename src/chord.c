#include "chord.h"
//#include <gb/bgb_emu.h> 
//chord view

UINT8 chord_on = 0; // this will play the chord
int chord_root_note = 24; // root note taken from frequencies table
int major_minor = 0; // 0 major, 1 minor
int aug_dim_norm = 0; // 0 norm, 1 augmented, 2 diminished
BYTE bpm_blink_state = 0;

// This controlls which mode is used
void chordKeypadController(void) {
  if (KEY_TICKED(J_B)) {
    if(KEY_PRESSED(J_A)) {
      if (chord_mode == 0) {
        chord_mode = 1;
        doPlayCurrentChord = 0;
        doSetCurrentStep = 0;
        keys = 0;
      } else {
        chord_mode = 0;       
        doPlayCurrentChord = 0;
        doSetCurrentStep = 0;
        keys = 0; // reset key step so b won't fire immediately
      }
      updateFaderMarker();
      waitpadup();
    }
  }
  switch(chord_mode)
  {
    case 0: {
      chordChangeMode();
      break;
    }
    case 1: {
      chordSteppaMode();
      break;
    }
  }
}

// chord change keypad controller
void chordChangeMode(void) {  
  if (KEY_RELEASED(J_A) && doPlayCurrentChord == 1) {
    playCurrentChord();  
    doPlayCurrentChord = 0;
  }
  if (KEY_RELEASED(J_B) && doSetCurrentStep == 1) {
    saveCurrentStep();
    printCurrentStep(current_record_steppa_step);
    doSetCurrentStep = 0;
  }
  if (KEY_TICKED(J_UP)) {
    if(KEY_PRESSED(J_A)) {
      chordPartRouter(J_UP, 12);
      doPlayCurrentChord = 0; // not turning chord on, just 12 semi tone jump
    } else if(KEY_PRESSED(J_B)) {
      chordPartRouter(J_UP, 3);
      doSetCurrentStep = 0;
    } else {    
      chordPartRouter(J_UP, 1);
    }
  } else if (KEY_TICKED(J_DOWN)) { 
    if(KEY_PRESSED(J_A)) {
      chordPartRouter(J_DOWN, 12);
      doPlayCurrentChord = 0; 
    } else if(KEY_PRESSED(J_B)) {
      chordPartRouter(J_DOWN, 3);
      doSetCurrentStep = 0;
    } else {    
      chordPartRouter(J_DOWN, 1);
    }
  } else if (KEY_TICKED(J_RIGHT)) { // move record marker in stepper
    if(KEY_PRESSED(J_A)) {
      chordStepRecordRouter(J_RIGHT, 1);
      doPlayCurrentChord = 0; 
    } else {    
      changeChordPart(J_RIGHT);
      waitpadup(); // eller...
    }
  } else if (KEY_TICKED(J_LEFT)) { 
    if(KEY_PRESSED(J_A)) {
      chordStepRecordRouter(J_LEFT, 1);
      doPlayCurrentChord = 0; 
    } else {    
      changeChordPart(J_LEFT);
      waitpadup();
    }
  } else if (KEY_TICKED(J_B)) {
    doSetCurrentStep = 1;
  } else if (KEY_TICKED(J_A)) {
    doPlayCurrentChord = 1;
  }
}

// this is to indicate the BPM speed in top right corner
void blinkBPM(void) {
  if (active_control_page == 3) {
    if(bpm_blink_state == 0) {
      set_bkg_tile_xy(0x13, 0x00, 0x46);
      bpm_blink_state = 1;
    } else {
      set_bkg_tile_xy(0x13, 0x00, 0x47);
      bpm_blink_state = 0;
    }
  }
}

// play chord step while sequencer is running, called by timer function
void playChordStep(void) {
  // reached number of beats pre step?
  if ( beats_counter == beats_per_step-1) {
    playNextChord();
    beats_counter = 0;
  } else {
    beats_counter++;
  }
}

// play the nex chord in the sequencer
void playNextChord(void) {
  if (current_seq_chord == 7) {
      current_seq_chord = 0;
    } else {
      current_seq_chord += 1;
    }
    playCurrentSeqStep();
}

// play the current step while sequencer is running
void playCurrentSeqStep(void) {
  int w = 0;
  int sq = 0;
  sq = chordsteppa[current_seq_chord].root + ((chordsteppa[current_seq_chord].majmin == 0) ? 4 : 3); 
  if (chordsteppa[current_seq_chord].adn == 1) {
    w = chordsteppa[current_seq_chord].root + 8;
  } else if (chordsteppa[current_seq_chord].adn == 2) { // deminisehed
    w = chordsteppa[current_seq_chord].root + 6;
  } else { // fallback 0 norm, this is for E8 chords basically 
    w = chordsteppa[current_seq_chord].root + 7;
  }
  sweep_note = chordsteppa[current_seq_chord].root;
  square_note = sq;
  wave_note = w;
  updateSweepFreq(1);
  updateSquareFreq(1);
  updateWaveFreq(1);
}

// chord step keydap controller
void chordSteppaMode(void) {
  if (KEY_PRESSED(J_RIGHT)) { 
    stepChordSteppa(J_RIGHT);
    waitpadup();
  } else if (KEY_PRESSED(J_LEFT)) { 
    stepChordSteppa(J_LEFT);
    waitpadup();
  } else if (KEY_PRESSED(J_B)) {
    playCurrentStep();
    waitpadup();
  }
}

// step in da steppa
void stepChordSteppa(BYTE direction) {
  if (direction == J_RIGHT) {
    if (current_chord_steppa_step == 7) {
      current_chord_steppa_step = 0;
    } else {
      current_chord_steppa_step += 1;
    }
  } else { //left
    if (current_chord_steppa_step == 0) {
      current_chord_steppa_step = 7;
    } else {
      current_chord_steppa_step -= 1;
    }
  }
  updateFaderMarker();
}

// saves the current chord in the current step
void saveCurrentStep(void) {
  chordsteppa[current_record_steppa_step].root = chord_root_note;
  chordsteppa[current_record_steppa_step].majmin = major_minor;
  chordsteppa[current_record_steppa_step].adn = aug_dim_norm;
}

// plays current step
void playCurrentStep(void) {
  frequency_mode = 1; // we are now floatin in note mode baby
  int w = 0;
  int sq = 0;
  sq = chordsteppa[current_chord_steppa_step].root + ((chordsteppa[current_chord_steppa_step].majmin == 0) ? 4 : 3); 
  if (chordsteppa[current_chord_steppa_step].adn == 1) {
    w = chordsteppa[current_chord_steppa_step].root + 8;
  } else if (chordsteppa[current_chord_steppa_step].adn == 2) { // deminisehed
    w = chordsteppa[current_chord_steppa_step].root + 6;
  } else { // fallback 0 norm, this is for E8 chords basically 
    w = chordsteppa[current_chord_steppa_step].root + 7;
  }
  sweep_note = chordsteppa[current_chord_steppa_step].root;
  square_note = sq;
  wave_note = w;
  updateSweepFreq(1);
  updateSquareFreq(1);
  updateWaveFreq(1);
}

// decide direction of record marker on stepper part
void chordStepRecordRouter(BYTE direction, int num) {
  if (direction == J_RIGHT) {
    if (current_record_steppa_step == 7) {
      current_record_steppa_step = 0;
    } else {
      current_record_steppa_step += num;
    }
  } else { //left
    if (current_record_steppa_step == 0) {
      current_record_steppa_step = 7;
    } else {
      current_record_steppa_step -= num;
    }
  }
  updateRecordMarker();
}

// this will move the record marker on the stepper part
void updateRecordMarker(void) {
  move_sprite(39, chord_steppa_step[current_record_steppa_step].x, 152);
}

// send args to correct function according to current chord part
void chordPartRouter(BYTE direction, int num) {
  switch(current_chord_step)
  {
    case 0: { // change root note
      changeRootNote(direction, num);
      break;
    }
    case 1: { // change minor or major flag
      changeMinorMajor(direction);
      break;
    }
    case 2: { // augmented, diminished or "normal"
      changeAugDimNorm(direction);
      break;
    }
  };
}

// Change the root note of the chord A-G 12 semitones
// play if chord mode is on
// redraw the sprites
void changeRootNote(BYTE direction, int num) {
  if (direction == J_UP && (chord_root_note + num <= 64)) {
    chord_root_note += num;
  } else if(direction == J_DOWN && (chord_root_note - num >= 0)) {
    chord_root_note -= num;
  }
  if (chord_on) {
    changeNotes();
  }
  printChordParts();
}

// Minor major switch
void changeMinorMajor(BYTE direction) {
  if (direction == J_UP ) {
    major_minor = (major_minor == 0) ? 1 : 0;
  } else if(direction == J_DOWN ) {
    major_minor = (major_minor == 0) ? 1 : 0;
  }
  if (chord_on) {
    changeNotes();
  }
  printChordParts();
}

// this will affect the 5th of a triad chord
// augmented, diminished and normal mode.
void changeAugDimNorm(BYTE direction) {
  if (direction == J_UP ) {
    if (aug_dim_norm == 2) {
      aug_dim_norm = 0;
    } else {
      aug_dim_norm++;
    }
  } else if(direction == J_DOWN ) {
    if (aug_dim_norm == 0) {
      aug_dim_norm = 2;
    } else {
      aug_dim_norm--;
    }
  }
  if (chord_on) {
    changeNotes();
  }
  printChordParts();
}

// this turns chord playing on or off
// when off changes won't take effect immediatly
void playCurrentChord(void) {
  chord_on = (chord_on) ? 0 : 1; // flip true/false on/off
  if (chord_on) {
    frequency_mode = 1; // set to note mode
    changeNotes();
  }
  setOnOffSprites();
}

// changes the channels frequencies according to choosen chord
void changeNotes(void) {
  sweep_note = chord_root_note;
  square_note = chord_root_note + ((major_minor == 0) ? 4 : 3); // number of semitones for a major or minor chord
  if (aug_dim_norm == 1 && chord_root_note < 64) { // 1 aug and less than highest root note for aug
    wave_note = chord_root_note + 8;
  } else if (aug_dim_norm == 2) { // deminisehed
    wave_note = chord_root_note + 6;
  } else { // fallback 0 norm, this is for E8 chords basically 
    wave_note = chord_root_note + 7;
  }
  
  updateSweepFreq(1);
  updateSquareFreq(1);
  updateWaveFreq(1);
} 

// This will step through the diffrent parts of the chord that is changeable
// like root note, octave, major minor etc
void changeChordPart(BYTE direction) {
  if (direction == J_RIGHT) {
    if (current_chord_step == 3 - 1) {
      current_chord_step = 0;
    } else {
      current_chord_step += 1;
    }
  } else { //left
    if (current_chord_step == 0) {
      current_chord_step = 3 - 1;
    } else {
      current_chord_step -= 1;
    }
  }
  updateFaderMarker();
}

// print the minor major indicator to screen
void setMinorMajorSprites( int major_minor) {
  UINT8 spritenum = (major_minor == 0) ? 0x04 : 0x3E;
  set_bkg_tile_xy(0x06, 0x05, spritenum);
}

// print the on/off indicator to screen
void setOnOffSprites(void) {
  UINT8 spritenum = ( chord_on == 0) ? 0x42 : 0x41;
  set_bkg_tile_xy(0x02, 0x02, spritenum);
}

// place the aug dim norm sprites
void setAugDimNormprites(void) {
  UINT8 spritenum = 0x04; // norm
  if (aug_dim_norm == 1) { // aug
    spritenum = 0x3F;  
  } else if (aug_dim_norm == 2) { // dim
    spritenum = 0x40;
  }
  set_bkg_tile_xy(0x07, 0x05, spritenum);
}

//print the chord parts to screen
void printChordParts(void) {
  setNoteSprites(20, chord_root_note);
  setMinorMajorSprites(major_minor);
  setAugDimNormprites();
}

// prints current step to the steppa
void printCurrentStep(int step) {
  int cp = 0;
  int compare = noteNames[chordsteppa[step].root][0] == 32;
  //BGB_printf("Compare: %d", compare);
  if (compare) {
    printWithoutHash(cp, step);
  } else {
    printWithHash(cp, step);
  }
  printMinMajAugDim(step);
}

// prints minor major augmented or dimished part of chord
void printMinMajAugDim(int step) {
  int num = 1;
  if (chordsteppa[step].majmin) { // 0 major, 1 minor
    if (chordsteppa[step].adn == 0) { // print only minor
      num = 1;
    } else if (chordsteppa[step].adn == 1) { // print minor and aug +
      num = 4;
    } else { // print minor and dim o
      num = 5;
    }
  } else {
    if (chordsteppa[step].adn == 0) { // print only major
      num = 32; // space
    } else if (chordsteppa[step].adn == 1) { // print aug +
      num = 2;
    } else { // print dim o
      num = 3;
    }
  }
  setNoteSpritesBg(chordsteppa[step].x, 
                     chordsteppa[step].y + 3, 
                     num);
}

// when root note has a hash
void printWithHash(int cp, int step) {
  for (int i = 0; i < 4; i++) {
      if (i == 0) { 
        // print
      }
      if (i == 1) {
        // print
      }
      if (i == 2) { // ignore
        cp++;
        continue;   
      }
      if (i == 3) { // octave 
         setNoteSpritesBg(chordsteppa[step].x, 
                   chordsteppa[step].y + (i - 1), 
                   noteNames[chordsteppa[step].root][cp]);
        cp++;
        continue;
      }
      setNoteSpritesBg(chordsteppa[step].x, 
                   chordsteppa[step].y + i, 
                   noteNames[chordsteppa[step].root][cp]);
      cp++;
    }
}

// when root note dosen't have a hash sign
void printWithoutHash(int cp, int step) {
  for (int i = 0; i < 4; i++) {
      if (i == 0) { // skip first as this is blank
        cp++;
        continue;
      }
      if (i == 1) {
        // print
      }
      if (i == 2) { // print blank
        setNoteSpritesBg(chordsteppa[step].x, 
                   chordsteppa[step].y + (i-1), 
                   noteNames[chordsteppa[step].root][0]);
        cp++;
        continue;   
      }
      if (i == 3) { // octave
         // print
      }
      setNoteSpritesBg(chordsteppa[step].x, 
                   chordsteppa[step].y + (i-1), 
                   noteNames[chordsteppa[step].root][cp]);
      cp++;
    }
}

//this will print the sequence
void printCurrentSeq(void) {
  for (int i = 0; i < 8; ++i)
  {
      printCurrentStep(i);
  }
}


/*
* Set the note tiles
*/
void setNoteSpritesBg(int x, int y, int number) {
  switch(number)
  {
    case 1: // Minor
        set_bkg_tile_xy(x, y, 0x3E);
        break;
    case 2: // Aug
        set_bkg_tile_xy(x, y, 0x3F);
        break;
    case 3: // Dim
        set_bkg_tile_xy(x, y, 0x40);
        break;
    case 4: // Minor and aug
        set_bkg_tile_xy(x, y, 0x44);
        break;
    case 5: // Minor and dim
        set_bkg_tile_xy(x, y, 0x45);
        break;
    case 32: //space 
        set_bkg_tile_xy(x, y, 0x04);
        break;
    case 35: //#
        set_bkg_tile_xy(x, y, 0x43);
        break;
    case 51: //3 
        set_bkg_tile_xy(x, y, 0x18);
        break;
    case 52: //4
        set_bkg_tile_xy(x, y, 0x19);
        break;
    case 53: //5
        set_bkg_tile_xy(x, y, 0x1A);
        break;
    case 54: //6 
        set_bkg_tile_xy(x, y, 0x1B);
        break;
    case 55: //7 
        set_bkg_tile_xy(x, y, 0x1C);
        break;
    case 56: //8 
        set_bkg_tile_xy(x, y, 0x1D);
        break;
    case 65: //A
        set_bkg_tile_xy(x, y, 0x29);
        break;
    case 66: //B 
        set_bkg_tile_xy(x, y, 0x2A);
        break;
    case 67: //C
        set_bkg_tile_xy(x, y, 0x0F);
        break;
    case 68: //D
        set_bkg_tile_xy(x, y, 0x11);
        break;
    case 69: //E
        set_bkg_tile_xy(x, y, 0x0A);
        break;
    case 70: //F
        set_bkg_tile_xy(x, y, 0x0B);
        break;
    case 71: //G
        set_bkg_tile_xy(x, y, 0x2B);
        break;
  }
}