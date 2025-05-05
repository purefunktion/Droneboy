#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// credit Zalo for keypad logic
// https://github.com/Zal0
#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) (!(keys & (K)) && (previous_keys & (K)))

#define TIMER_FREQ 1024     // 256 timer ticks per second
#define SECONDS_PER_MIN 60

// load the wave register pattern data
void loadWave(void);

// fader stuff
void moveFader(int channel);
void change_fader(BYTE direction);
void updateFaderMarker(void);
void updateRecordMarker(void);

void updateSweepFreq(int retrigger);
void updateSquareFreq( int retrigger);
void updateWaveFreq(int retrigger);
void updateNoiseFreq(UBYTE new_freq);
void updateNoiseNoteFreq(UBYTE new_freq);

void clearCounterValues(UINT8 position, int channel);
void setCounterSprites(UINT8 position, int value);
void updateWaveVolume(int volume, int sample_index);

void placeMacroMarker(void);
void setAllVolumeMacroMarkers(void);
void setAllDutyMacroMarkers(void);
void setAllFreqMacroMarkers(void);

void flipHeader(void);

// frequency page, notes
void setNoteSprites(int position, int note_value);

// chord page, called from main when switching
void printChordParts(void);
void setOnOffSprites(void);
void playChordStep(void);
void playNextChord(void);
void printCurrentSeq(void);

// bpm 
void blinkBPM(void);
void printBPM(void);


#endif