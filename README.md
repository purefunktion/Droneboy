# Droneboy
Drone music application for Game Boys.
Droneboy creates endless tones or drones on the Gameboys four channels. You can manipulate volume, duty and frequency and play and sequence chords. It is also MIDI compatible using an Arduinoboy.

# Don't have a real Gameboy and flashcart?
There are numerous emulators for the Gameboy these days. The Droneboy ROM was developed using BGB, SameBoy and Emoulicious. Get one of them and download the latest release (see the "Releases" section on the right). You can also try Droneboy in your browser over at https://hh.gbdev.io/game/droneboy.

# Easy version
This project started as a learning experience and the code was kept fairly simple for clarity. The "easy to understand" version has been frozen in the `easy` folder (and will be moved out shortly) for anyone starting out with c and gbdk programming to check out. Going forward, there will be more "advanced" features used that might be a bit intimidating to beginners.

# Documentation
Read the manual in the release or [Droneboy_manual.pdf](https://github.com/purefunktion/Droneboy/tree/master/docs/Droneboy_manual.pdf)

# About
Written in C with sprinkles of ASM using GBDK-2020(version gbdk-4.4.0)

# Build
Use `make` in root of project. The `.gb` ROM file will end up in `obj` directory.