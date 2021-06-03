// credit Zalo for keypad logic
// https://github.com/Zal0
#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previous_keys & (K)))

// fader stuff
void moveFader();
void change_fader(BYTE direction);
void updateFaderMarker();

void updateSweepFreq(UWORD new_freq);
void updateSquareFreq(UWORD new_freq);
void updateWaveFreq(UWORD new_freq);
void updateNoiseFreq(UBYTE new_freq);

void clearCounterValues(UINT8 position);
void setCounterSprites(UINT8 position, int value);