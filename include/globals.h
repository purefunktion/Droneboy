#ifndef GLOBALS_H
#define GLOBALS_H

//Globals
enum CHANNELS {SWEEP, SQUARE, WAVE, NOISE};

// Zalo key pad logic
// https://github.com/Zal0
extern int8_t keys;
extern uint8_t previous_keys;

// frequencies
extern uint16_t sweep_freq;
extern uint16_t square_freq;
extern uint16_t wave_freq;
extern uint8_t noise_freq;

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
  uint8_t x;
  uint8_t y;
  uint8_t fader_position;
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

extern uint8_t num_control_pages;
extern Event current_state; // what page we are on

extern const char noteNames[72][5];
extern const uint16_t frequencies[72];
extern const uint8_t squareSamplesLow[64];
extern const uint8_t squareSamplesHigh[32];
extern const uint8_t sawSamplesLow[256];
extern const uint8_t sawSamplesHigh[128];
extern const uint8_t triangleSamplesLow[256];
extern const uint8_t triangleSamplesHigh[128];
extern const uint8_t sineSamplesLow[256];
extern const uint8_t sineSamplesHigh[128];
extern const uint8_t noiseNotesFrequencies[6]; // this is the uint8_t for FF22 - NR43 - Polynomial Counter
extern const int noiseNoteNameIndex[6]; // this is the index in noteNames
extern const uint8_t volumeFaderPosition[16];
extern uint8_t waveToBeLoaded[16]; // this is the arae where waves are manipulated

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

#define MAX_SEQUENCE_STEPS 8
#define MAX_SEQUENCE_INDEX (MAX_SEQUENCE_STEPS - 1)

// A-button state, because A is used as semi tone hop as well, on chord page
extern int doPlayCurrentChord;
// B-button state on chord page
extern int doSetCurrentStep;

// Chord step struct
struct ChordStep {
  uint8_t root; // chord root note
  uint8_t majmin; // major or minor maj=0 min=1
  uint8_t adn; // norm, augmented, diminished norm=0, aug=1, dim=2
  int x; // background sprite x position
  uint8_t y; // background sprite y position
};

extern struct ChordStep chordsteppa[8];
// flag for the mode on chord page, 0 chord change or 1 steppa
extern uint8_t chord_mode;
// chord on or off
extern uint8_t chord_on;

// state of the chord step sequencer 0=off,1=on
extern int8_t play_chord_step;
extern uint8_t beats_per_step;
extern uint8_t beats_counter;
extern int current_seq_chord;

// counter for bpm
extern uint16_t tim_cnt;
// bpm variables
extern uint16_t bpm_in_cycles; // counter
extern uint16_t bpm; // bpm
extern int8_t bpm_blink_state; // state for the blinker

extern int chord_root_note; // root note taken from frequencies table
extern int major_minor; // 0 major, 1 minor
extern int aug_dim_norm; // 0 norm, 1 augmented, 2 diminished

// volume values 0-15
extern const uint8_t volumeValues[16];

void setBpm(uint16_t new_bpm);
// lut for bpm counter
extern const uint16_t bpm_cycles_lut[240];
// display playing the chord stepper or not.
void printChordSteppaOnOff(void);

// this tells if we are currently reding from link port or not
extern int8_t system_idle;

// this is to tell if we are playing the longer or shorter waves
// in the wave channel
extern int8_t low_or_high_wave_freq; // 0 = low

// in frequency button controller flip if b is independently pressed
extern int8_t low_high_wave_flip;

#endif