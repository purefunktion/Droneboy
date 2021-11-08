#ifndef GLOBALS_H
#define GLOBALS_H

//Globals

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
extern int square_volume;
extern int wave_volume;
extern int noise_volume;

// notes
extern int sweep_note;  
extern int square_note;
extern int wave_note;
extern int noise_note;

extern int current_channel;

// duty
extern int duty_sweep;
extern int duty_square;
extern int duty_wave; // sample index nums for square wave in wave channel

//wave type in wave cahnnel

enum WAVES {SQUARE, SAW, RAMP, TRIANGLE, SINE};
extern enum WAVES wave_type;

struct fader {
  UINT8 x;
  UINT8 y;
  UINT8 upper_bound;
  UINT8 lower_bound;
  UINT8 step;
  UINT8 fader_position;
};

extern struct fader fader_group[4];
extern struct fader duty_fader_group[4];

extern int frequency_mode;

struct NoiseyStruct {
  //NR43 0xFF22
  int dividing_ratio  ;//: 3;
  int counter_step ;//: 1;
  int clock_freq ;//: 4 bits;
} ;

extern struct NoiseyStruct noiseStruct;

extern UINT8 num_control_pages;
extern int active_control_page;
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
extern int domacro;

#endif