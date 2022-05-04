#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// credit Zalo for keypad logic
// https://github.com/Zal0
#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) (!(keys & (K)) && (previous_keys & (K)))

// load the wave register pattern data
void loadWave();

// fader stuff
void moveFader(int channel);
void change_fader(BYTE direction);
void updateFaderMarker();
void updateRecordMarker();

void updateSweepFreq(int retrigger);
void updateSquareFreq( int retrigger);
void updateWaveFreq(int retrigger);
void updateNoiseFreq(UBYTE new_freq);
void updateNoiseNoteFreq(UBYTE new_freq);

void clearCounterValues(UINT8 position, int channel);
void setCounterSprites(UINT8 position, int value);
void updateWaveVolume(int volume, int sample_index);

void placeMacroMarker();
void setAllVolumeMacroMarkers();
void setAllDutyMacroMarkers();
void setAllFreqMacroMarkers();

void flipHeader();

// frequency page, notes
void setNoteSprites(int position, int note_value);

// chord page, called from main when switching
void printChordParts();
void setOnOffSprites();
void playChordStep();
void playNextChord();
void printCurrentSeq();

// bpm 
void blinkBPM();

#endif