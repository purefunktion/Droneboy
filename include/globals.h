#ifndef GLOBALS_H
#define GLOBALS_H

//Globals
enum CHANNELS {SWEEP, SQUARE, WAVE, NOISE};

// Zalo key pad logic
// https://github.com/Zal0
extern BYTE keys;
extern UBYTE previous_keys;

// frequencies
extern UWORD sweep_freq;
extern UWORD square_freq;
extern UWORD wave_freq;
extern UBYTE noise_freq;

// volumes
extern int sweep_volume;
extern int sweep_up_down_flag; // 1 up zero down
extern int square_volume;
extern int wave_volume;
extern int noise_volume;

// notes
extern int sweep_note;  
extern int square_note;
extern int wave_note;
extern int noise_note;

// this indicates which channel is being controlled
extern int current_channel;
// and this for the chord part being controlled, on chord page
extern int current_chord_step;
// chord stepper current step
extern int current_chord_steppa_step;
// the record marker on stepper on chord page
extern int current_record_steppa_step;

// duty
extern int duty_sweep;
extern int duty_square;
extern int duty_wave; // sample index nums for square wave in wave channel

//wave type in wave cahnnel
enum WAVES {SQUAREWAVE, SAW, RAMP, TRIANGLE, SINE};
extern enum WAVES wave_type;

// this is the marker for fader and other stuff
struct fader {
  UINT8 x;
  UINT8 y;
  UINT8 fader_position;
};

extern struct fader fader_group[4];
extern struct fader duty_fader_group[4];
extern struct fader chord_part_step[4];
extern struct fader chord_steppa_step[8];

// which mode on freq page, note or freq
extern int frequency_mode;

struct NoiseyStruct {
  //NR43 0xFF22
  int dividing_ratio  ;//: 3;
  int counter_step ;//: 1;
  int clock_freq ;//: 4 bits;
} ;

extern struct NoiseyStruct noiseStruct;

// navigation state machine enums
typedef enum {
  VOLUME_PAGE,
  DUTY_PAGE,
  FREQ_PAGE,
  CHORD_PAGE,
  CREDIT_PAGE,
  BPM_PAGE
} State;

typedef enum {
  EVENT_NONE,
  EVENT_UP,
  EVENT_DOWN,
  EVENT_LEFT,
  EVENT_RIGHT
} Event;

extern UINT8 num_control_pages;
extern Event current_state; // what page we are on
extern int credit_page;

extern const char noteNames[72][5];
extern const UWORD frequencies[72];
extern const UBYTE squareSamples[32];
extern const UBYTE sawSamples[128];
extern const UBYTE triangleSamples[128];
extern const UBYTE sineSamples[128];
extern const UBYTE noiseNotesFrequencies[6]; // this is the UBYTE for FF22 - NR43 - Polynomial Counter
extern const int noiseNoteNameIndex[6]; // this is the index in noteNames
extern const UBYTE volumeFaderPosition[16];
extern UBYTE waveToBeLoaded[16]; // this is the arae where waves are manipulated

// Macro marker
struct MacroStatus {
  int sweep;
  int square;
  int wave;
  int noise;
};

extern struct MacroStatus volumeMacroStatus;
extern struct MacroStatus dutyMacroStatus;
extern struct MacroStatus freqMacroStatus;
// to place the macro marker
extern int domacro;
// this is to keep track of v_blanks before doing a continuous sweep on freq
extern int up_sweep_counter;
extern int down_sweep_counter;

// track v blanks to continuous 
extern int up_volume_counter;
extern int down_volume_counter;
extern int volume_slide_counter;

// A-button state, because A is used as semi tone hop as well, on chord page
extern int doPlayCurrentChord;
// B-button state on chord page
extern int doSetCurrentStep;

// Chord step struct
struct ChordStep {
  UINT8 root; // chord root note
  UINT8 majmin; // major or minor
  UINT8 adn; // norm, augmented, diminished
  int x; // background sprite x position
  UINT8 y; // background sprite y position
};

extern struct ChordStep chordsteppa[8];
// flag for the mode on chord page, 0 chord change or 1 steppa
extern UINT8 chord_mode;

// state of the chord step sequencer 0=off,1=on
extern BYTE play_chord_step;
extern UINT8 beats_per_step;
extern UINT8 beats_counter;
extern int current_seq_chord;

// counter for bpm
extern uint16_t tim_cnt;
// bpm variables
extern uint16_t bpm_in_cycles;
extern uint16_t bpm;
extern BYTE bpm_blink_state;

extern int chord_root_note; // root note taken from frequencies table
extern int major_minor; // 0 major, 1 minor
extern int aug_dim_norm; // 0 norm, 1 augmented, 2 diminished

// volume values 0-15
extern const UBYTE volumeValues[16];

void setBpm(uint16_t new_bpm);
extern const uint16_t bpm_cycles_lut[240];
void printChordSteppaOnOff(void);

#endif