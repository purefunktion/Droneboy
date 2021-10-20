# Droneboy
Drone music application for Gameboy. 
Droneboy creates endless tones or drones on the Gameboys four channels. You can manipulate volume, duty and frequency.

# Don't have a real Gameboy and flashcart?
There are numerous emulators for the Gameboy these days. The Droneboy ROM was developed using BGB emulator. Other famous once are SameBoy and Emoulicious. You can even try Droneboy in your browser over at https://wasmboy.app/ Download the droneboy.gb file above and try it out.

# How to use
The Droneboy is made up of three control pages volume, duty and frequency

Use **SELECT** to switch between the pages.

A common feature on all control pages is the macro and inverted macro markers. Use **A** button to place a macro marker under each fader/control. Press **A** again to place a inverted macro marker. And a third time to delete marker. When a macro marker is activated that fader will respond to whatever change is done on the other channels on that control page. For example a macro marker on the sweep channel on the volume page will increase in volume if the square(or any other) fader is increased. A inverted macro marker will do the opposite, if another fader/control is increased the inverted one will decrease. With this utility you can for example put on all channels volume at once or change the octave of a minor chord on the frequency page.

On the volume page:

**RIGHT/LEFT** change sound channel. 
The channels are from left to right sweep, square, wave and noise

**UP/DOWN** to increase/decrease volume
**HOLD B + UP/DOWN** will maximize/minimize sound volume

On the duty page:

**RIGHT/LEFT** change sound channel.  
Channels(left to right) sweep, square, wave, noise. The noise channel has no duty so the fader will instead control the "dividing ratio of frequencies" in the polynomial counter.

**UP/DOWN** to cycle through duty settings

**B** on noise channel will toggle the counter step of the polynomal counter. 

On frequency page:

**HOLD A and press B** changes the mode between notes and frequency

In frequency mode:

**RIGHT/LEFT** change channel.  
Top sweep and square, bottom wave and noise

**UP** changes the frquency 1 step

**HOLD A + UP** changes frequency 100 steps

**HOLD B + UP** changes frequency 10 steps

In note mode:

**RIGHT/LEFT** change channel

**UP/DOWN** changes note 1 semitone

**HOLD A + UP** changes note 12 semitones(octave)

**HOLD B + UP** changes note 3 semitones

The noise channel freq mode controls the Shift Clock Frequency. The note mode has two notes that are "close enough" to authors ears of a D and G#.  

**START** will display the credit page

# Made with
GBDK-2020(gbdk-4.0.5) https://github.com/gbdk-2020/gbdk-2020  
Emulator used in development BGB https://bgb.bircd.org/  
Sprites and maps made with GBTD/GBMB http://www.devrs.com/gb/hmgd/intro.html  