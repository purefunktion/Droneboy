#ifndef MIDI_H
#define MIDI_H

#define MIDI_STATUS_BIT 0x80
#define MIDI_STATUS_NOTE_ON 0x09
#define MIDI_STATUS_NOTE_OFF 0x08
#define MIDI_STATUS_AT 0x0A
#define MIDI_STATUS_CC 0x0B
#define MIDI_STATUS_PC 0x0C
#define MIDI_STATUS_AT_MONO 0x0D
#define MIDI_STATUS_PB 0x0E

#define MIDI_STATUS_SYSTEM 0xF0

// The midi channel to listen to
extern uint8_t midiChannel;

// MIDI status
extern uint8_t statusByte;
// MIDI address (e.g. note/CC control)
extern uint8_t addressByte;
// MIDI value (e.g. velocity/value)
extern uint8_t valueByte;
extern uint8_t capturedAddress;

extern uint16_t coarse_sweep_value;
extern uint16_t coarse_square_value;
extern uint16_t coarse_wave_value;

void updateMidiBuffer(void);
void eventMidiCCChannel(void);

void sweepMidiVolume(void);
void squareMidiVolume(void);
void waveMidiVolume(void);
void noiseMidiVolume(void);

void sweepMidiDuty(void);
void squareMidiDuty(void);
void waveMidiDuty(void);
void noiseMidiDuty(void);

void sweepMidiFreqCoarse(void);
void sweepMidiFine(void);

void squareMidiFreqCoarse(void);
void squareMidiFine(void);

void waveMidiFreqCoarse(void) ;
void waveMidiFine(void);

void changeWaveTypeMidi(void);
void toggleHighLowWaveTypeMidi(void);

void noiseMidiFreq(void);
void noiseMidiCounterStep(void);

// Sets parameter to change LSB
void nrpnLSB(void); // CC 98
// Sets parameter to change MSB
void nrpnMSB(void); // CC99
// Sets the data for nrpn parameter to change LSB
void nrpnDataLSB(void); // CC 98
// Sets the data for nrpn parameter to change MSB
void nrpnDataMSB(void); // CC99
// Run the NRPN parameter with the data
void runnrpn(void);

// mute channels
extern int old_sweep_midi_volume;
extern int old_square_midi_volume;
extern int old_wave_midi_volume;
extern int old_noise_midi_volume;

void muteSweepMidi(void);
void muteSquareMidi(void);
void muteWaveMidi(void);
void muteNoiseMidi(void);

// sequencer
void seqMidiStart(void);
void rewindToStartSeqMidi(void);
// general
void toggleChordOnOffMidi(void);

// on off messages
void eventMidiNoteOn(void);
void eventMidiNoteOff(void);

// increase decrease midichannel, done on bpm pag atm
void increaseMidiChannel(void);
void decreaseMidiChannel(void);
void printMidi(void);

#endif