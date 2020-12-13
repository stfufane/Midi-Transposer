#pragma once

#include "JuceHeader.h"
#include "Params.h"

class MidiProcessor
{
public:
    MidiProcessor();

    void process(MidiBuffer& midiMessages);
    int getLastNoteOn();

    AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

    void registerListeners(AudioProcessorValueTreeState& treeState);
private:
    MidiBuffer processedMidi;
    // Parameters declared in helper struct. The lambda will be called when a corresponding parameter is changed.
    MidiParams midiParams{ [this]() { updateMidiParams(); } };
    NoteParams noteParams;

    // Local variables that get their values from parameters
    std::atomic<int>  inputChannel{ 1 };
    std::atomic<int>  outputChannel{ 1 };
    std::atomic<int>  octaveTranspose{ 0 };
    std::atomic<bool> bypassOtherChannels{ false };
    std::vector< std::vector<int> > mappingNotes;

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
    void mapNote(const int note, const juce::uint8 velocity, const bool noteOn, const int samplePosition);
    void playMappedNotes(const int note, const juce::uint8 velocity, const int samplePosition);
    void playNote(const int note, const juce::uint8 velocity, const int samplePosition);
    void stopCurrentNotes(const juce::uint8 velocity, const int samplePosition);
    void removeHeldNote(const int note);
    // -----------------------------------

    // -----------------------------------
    // Manage mapping values
    void initParameters();
    void updateMidiParams();
    void updateNoteParams(const NoteParam& noteParam);
    void setMappedNotes(const int from_note, const int to_note, const int chord);
    // -----------------------------------
};
