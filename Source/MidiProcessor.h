#pragma once

#include "JuceHeader.h"
#include "Params.h"

class MidiProcessor
{
public:
    MidiProcessor();

    void process(MidiBuffer& midiMessages);

    void addParameters(AudioProcessor& p);

    // Parameters declared in helper struct. The lambda will be called when a corresponding parameter is changed.
    MidiParams midiParams{ [this]() { updateMidiParams(); } };
    NoteParams noteParams;
private:
    MidiBuffer processedMidi;

    // Local variables that get their values from parameters
    std::atomic<int>  inputChannel{ 1 };
    std::atomic<int>  outputChannel{ 1 };
    std::atomic<int>  octaveTranspose{ 0 };
    std::vector< std::vector<int> > mappingNotes;
    
    int lastNoteOn{ -1 };
    int currentNoteOutputChannel{ 1 };
    std::vector<int> currentInputNotesOn;
    std::vector<int> currentOutputNotesOn;
    
    // -----------------------------------
    // Process the input midi events
    void mapNote(const int note, const uint8 velocity, const bool noteOn, const int samplePosition);
    void playMappedNotes(const int note, const uint8 velocity, const int samplePosition);
    void playNote(const int note, const uint8 velocity, const int samplePosition);
    void stopCurrentNotes(const uint8 velocity, const int samplePosition);
    void removeHeldNote(const int note);
    // -----------------------------------

    // -----------------------------------
    // Manage mapping values
    void initParameters();
    void updateMidiParams();
    void updateNoteMapping(const NoteParam& noteParam);
    // -----------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiProcessor)
};
