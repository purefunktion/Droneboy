# Droneboy
Drone music application for Gameboy. 
Droneboy creates endless tones or drones on the Gameboys four channels. You can manipulate volume, duty and frequency.

# Don't have a real Gameboy and flashcart?
There are numerous emulators for the Gameboy these days. The Droneboy ROM was developed using BGB emulator. Other famous once are SameBoy and Emoulicious. You can even try Droneboy in your browser over at https://wasmboy.app/ Download the droneboy.gb file above and try it out.

# How to use
The Droneboy is made up of four control pages volume, duty, frequency and chord/chord step.

Use **SELECT** to switch between the pages.

A common feature on most control pages is the macro and inverted macro markers. Use **A** button to place a macro marker under each fader/control. Press **A** again to place a inverted macro marker. And a third time to delete marker. When a macro marker is activated that fader will respond to whatever change is done on the other channels on that control page. For example a macro marker on the sweep channel on the volume page will increase in volume if the square(or any other) fader is increased. A inverted macro marker will do the opposite, if another fader/control is increased the inverted one will decrease. With this utility you can for example put on all channels volume at once or change the octave of a minor chord on the frequency page.

On the volume page:

**RIGHT/LEFT** change sound channel. 
The channels are from left to right sweep, square, wave and noise

**UP/DOWN** to increase/decrease volume
**HOLD B + UP/DOWN** will maximize/minimize sound volume

On the duty page:

**RIGHT/LEFT** change sound channel.  
Channels(left to right) sweep, square, wave, noise. The noise channel has no duty so the fader will instead control the "dividing ratio of frequencies" in the polynomial counter.

**UP/DOWN** to cycle through duty settings

**B** on noise channel will toggle the counter step of the polynomial counter. 

**A** on wave channel will change wave type. There is a square, saw, ramp, triangle and sine wave to choose from. These use half the wave ram period(to follow the sweep/square channels frequency), so the sample rate is very limited(aka lofi charm). 

On frequency page:

**HOLD A and press B** changes the mode between notes and frequency

In frequency mode:

**RIGHT/LEFT** change channel.  
Top sweep and square, bottom wave and noise

**UP** changes the frequency 1 step

**HOLD A + UP** changes frequency 100 steps

**HOLD B + UP** changes frequency 10 steps

In note mode:

**RIGHT/LEFT** change channel

**UP/DOWN** changes note 1 semitone

**HOLD A + UP/DOWN** changes note 12 semitones(octave)

**HOLD B + UP/DOWN** changes note 3 semitones

The noise channel freq mode controls the Shift Clock Frequency. The note mode has two notes that are "close enough" to authors ears of a D and G#.  

On chord/chord step page:

On the chord step page there are two different modes. One is the "chord change" mode and the other is for playing the chord step. When in chord change mode the marker(square) will be under the chord that is being manipulated. There are three parts of the chord that can be changed root note, minor/major and augmented/diminished/normal. Press **RIGHT/LEFT** to change between parts and up and down to manipulate the values. When on the root note part **HOLD A + UP/DOWN** to jump 12 semitones(octave) and **HOLD B + UP/DOWN** to jump 3 semitones. Just **UP/DOWN** will change the root note one semitone. To play the current chord  press **A**, pressing **A** again won't turn the sound off but you can now change to a chord you like and then retrigger that by pressing **A** to "on" again. This is indicated by the on/off sprite above the chord change part.

The chord step part has 8 chord slots that can be filled with the chords you set in the chord change part. To make things a bit smoother there is a "record marker" that can be moved when in the chord change mode. By **HOLD A + LEFT/RIGHT** you can change the record markers place in the sequence of chord slots. By pressing **B** you set the currently selected slot to the chord chosen in the chord mode. This way you can create a sequence of 8 chords.

To play the sequence you have to be in the chord play mode. Change mode by **HOLD A and press B** and the square marker will jump down to the chord step sequence. In chord step mode go **RIGHT/LEFT** to change slot and press **B** to play the current chord. The chord retriggers if you play press **B** repeatedly on the same chord slot.


**START** will display the credit page

# Made with
GBDK-2020(version gbdk-4.0.5) https://github.com/gbdk-2020/gbdk-2020  
Emulator used in development BGB https://bgb.bircd.org/  
Sameboy also used in development, very good for examining wave ram https://sameboy.github.io/  
Sprites and maps made with GBTD/GBMB http://www.devrs.com/gb/hmgd/intro.html  
Invaluable reference document for Gameboy https://gbdev.io/pandocs/  
Sound table reference for Gameboy sound frequency/note/hz/MIDI http://www.devrs.com/gb/files/sndtab.html  