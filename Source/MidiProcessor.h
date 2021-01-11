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
    std::atomic<int>  inputChannel{ 0 };
    std::atomic<int>  outputChannel{ 0 };
    std::atomic<int>  octaveTranspose{ 0 };
    std::vector< std::vector<int> > mappingNotes;
    
    int lastNoteOn{ -1 };

    struct NoteState {
        int note;
        int channel;
        uint8 velocity;
    };
    std::vector<NoteState> currentInputNotesOn;
    std::vector<NoteState> currentOutputNotesOn;
    
    // -----------------------------------
    // Process the input midi events
    void mapNote(const MidiMessage& m, const int samplePosition);
    void playMappedNotes(const NoteState& noteState, const int samplePosition);
    void playNote(const NoteState& noteState, const int samplePosition);
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
