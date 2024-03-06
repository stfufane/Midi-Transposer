# Midi Transposer


A VST3 plugin developed with JUCE framework to map chords to a MIDI bass pedal (or any MIDI controller)

## General purpose


The idea is to use my MIDI bass pedal (Roland PK5-A) and expand its usage by mapping a root note and up to 12 intervals to each note of the octave. 
This way I can build a sequence of chords to play with my feet while improvising on any other instrument.


It was mainly a way to start learning how to use the JUCE framework and make an application I can use instead of a dull tutorial. 
I'll improve it as ideas come and I make progress learning the framework. 

It uses version 7.0.10 of JUCE by pulling the master branch from CMake. It's compatible with Windows and macOS as far as I tested. I intend to test on Linux too.

You're welcome to clone it and use it as you want. And if you see stupid code, don't hesitate to tell me, I'll be happy to improve and fix it :) 

## Build

### CMake ###

To build the project using CMake, use the command line

```bash
# Generate
cmake . -B build
# Build
cmake --build build # --config Release
```

If you want the plugin to be copied in your VST3 folder, change `COPY_PLUGIN_AFTER_BUILD` to TRUE in Source/CMakeLists.txt

If you don't need JUCE extras (I use AudioPluginHost for debugging in Visual Studio Code), pass the `JUCE_BUILD_EXTRAS` option to OFF in the root CMakeLists.txt

For a great guide about configuring a CMake project with Visual Studio Code, I advise you to check [this great repository](https://github.com/tomoyanonymous/juce_cmake_vscode_example).

## Features

The interface is almost complete with this design.
![interface](./Midi%20Transposer%20v0.5.png)

* **Input Midi Channel** : Defines from which channel the incoming MIDI events will be processed
* **Output Midi Channel** : Defines to which channel the processed MIDI events will be routed
* **Octave Transpose** : If a value other than 0 is selected, the root note will be played at its original height and the selected chord will be transposed to the chosen octave
* **Arpeggiator** : Self explanatory :)

When you click on a note, you have access to the intervals panel.

There you can select which intervals will be played for the selected note. You can also decide to transpose the whole chord by a number of semitones. To close the panel, click again on the note.

And that's it, it's pretty straight forward.

## Hot Reload and configuration

The project uses [Gin](https://github.com/FigBug/Gin) for its filewatcher class that allows to configure some GUI values with hot reload. If you want to change some colors, you can edit data/config/colors.json and define different ones.

The values in data/config/positions.json were mostly helpful during the development phase to define the different block sizes and positions and tweak all the coordinates without having to recompile everything.

## Next

Some ideas I have in mind to improve it :
 * Shortcuts to add some intervals to all notes
 * Shortcut to reset the intervals of one note
 * Different arpeggiator patterns

 ## Third party dependencies

- [JUCE](https://juce.com/)
- [Gin](https://github.com/FigBug/Gin)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Iosevka font](https://github.com/be5invis/Iosevka)
- [Scarlab Solid Oval Interface Icons Collection](https://www.svgrepo.com/collection/scarlab-solid-oval-interface-icons/)