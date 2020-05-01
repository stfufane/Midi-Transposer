# Midi-Bass-Pedal-Chords
A VST3 plugin developed with JUCE framework to map chords to a MIDI bass pedal (or any MIDI controller)

## General purpose
The idea is to use my MIDI bass pedal (Roland PK5-A) and expand its usage by mapping a root note and chord to each note of the octave. 
This way I can build a sequence of chords to play with my feet while improvising on any other instrument.


It was mainly a way to start learning how to use the JUCE framework and make an application I can use instead of a dull tutorial. 
I'll improve it as ideas come and I make progress learning the framework. 


It was build using version 5.4.7 of the Projucer on Windows 10 with Visual Studio 2019, I didn't try other IDEs or operating systems.


You're welcome to clone it and use it as you want. And if you see stupid code, don't hesitate to tell me, I'll be happy to improve and fix it :) 

## Features


![screenshot](./BassPedalVST.png "Screenshot")


* Input Midi Channel : Defines from which channel the incoming MIDI events will be processed
* Output Midi Channel : Defines to which channel the processed MIDI events will be routed
* Octave Transpose : If a value other than 0 is selected, the root note will be played at its original height and the selected chord will be transposed to the chosen octave
* Bypass other channels : If this is checked, all events from other channels than the input MIDI channel selected will be filtered. Otherwise they'll pass without manipulation.


Then for each note, you can select the associated root note (meaning every note could become a G# for example) and the chord that will be played.


And that's it, it's pretty straight forward.