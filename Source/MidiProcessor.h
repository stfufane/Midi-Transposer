#pragma once

#include "JuceHeader.h"

namespace ParamIDs
{
    static String inChannel{ "in_channel" };
    static String outChannel{ "out_channel" };
    static String octaveTranspose{ "octave_transpose" };
    static String bypassChannels{ "bypass_other_channels" };
    static String noteChoice{ "_note" };
    static String chordChoice{ "_chord" };
}

class MidiProcessor : AudioProcessorValueTreeState::Listener
{
public:
    MidiProcessor();

    void process(MidiBuffer& midiMessages);

    void registerListeners(AudioProcessorValueTreeState& treeState);
    void parameterChanged(const String& parameterID, float newValue) override;

    int getLastNoteOn();

    StringArray& getNotes() { return notes; }
    StringArray& getChords() { return chords; }

    AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

private:
    std::vector<RangedAudioParameter*> params;

    // Local variables that get their values from parameters
    int inputChannel{ 1 };
    int outputChannel{ 1 };
    std::vector< std::vector<int> > mappingNotes;
    bool bypassOtherChannels{ false };
    int octaveTranspose{ 0 };

    StringArray notes{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    StringArray chords{ "None", "maj", "min", "sus4", "maj7", "min7", "7", "m7b5" };

    const std::vector< std::vector<int> > chordIntervals {
        {0},            // None
        {0, 4, 7},      // maj
        {0, 3, 7},      // min
        {0, 5, 7},      // sus4
        {0, 4, 7, 11},  // maj7
        {0, 3, 7, 10},  // min7
        {0, 4, 7, 11},  // 7
        {0, 3, 6, 10}   // m7b5
    };
    
    int lastNoteOn{ -1 };
    int currentNoteOutputChannel{ 1 };
    std::vector<int> currentInputNotesOn;
    std::vector<int> currentOutputNotesOn;
    
    // -----------------------------------
    // Process the input midi events
    void mapNote(const int note, const juce::uint8 velocity, const bool noteOn, const int samplePosition, MidiBuffer& processedMidi);
    void playMappedNotes(const int note, const juce::uint8 velocity, const int samplePosition, MidiBuffer& processedMidi);
    void stopCurrentNotes(const juce::uint8 velocity, const int samplePosition, MidiBuffer& processedMidi);
    void removeHeldNote(const int note);
    // -----------------------------------

    // -----------------------------------
    // Manage mapping values
    void updateParameters();
    void setMappedNotes(const int note_origine, const int new_note, const int chord);
    // -----------------------------------
};
