#pragma once

#include "JuceHeader.h"
#include "Params/Params.h"

class MidiProcessor
{
public:
    MidiProcessor();

    void prepareToPlay(const double rate);
    void process(juce::MidiBuffer& midiMessages, const int numSamples, juce::AudioPlayHead* playHead);
    void addParameters(juce::AudioProcessor& p);

    MidiParams& getMidiParams();
    NoteParams& getNoteParams();
    ArpeggiatorParams& getArpeggiatorParams();
private:
    // Parameters declared in helper struct. The lambda will be called when a corresponding parameter is changed.
    MidiParams midiParams{ [this]() { updateMidiParams(); } };
    NoteParams noteParams;
    ArpeggiatorParams arpeggiatorParams{ [this]() { updateArpeggiatorParams(); } };

    juce::MidiBuffer processedMidi;

    // Local variables that get their values from parameters
    std::atomic<int>  inputChannel{ 0 };
    std::atomic<int>  outputChannel{ 0 };
    std::atomic<int>  octaveTranspose{ 0 };
    std::vector< std::vector<int> > notesMapping;

    std::atomic<bool> arpeggiatorActivated{ false };
    std::atomic<float> arpeggiatorRate{ 0.5f };
    
    // Used to calculate the arpeggiator note positions.
    float sampleRate;
    int time;
    
    int currentNote;
    int lastNoteOn{ -1 };

    struct NoteState {
        int note;
        int channel;
        juce::uint8 velocity;

        void reset() { note = -1; }
    };
    std::vector<NoteState> currentInputNotesOn;
    std::vector<NoteState> currentOutputNotesOn;
    NoteState lastArpeggiatorNote;
    
    // -----------------------------------
    // Process the input midi events
    void mapNote(const juce::MidiMessage& m, const int samplePosition);
    void playMappedNotes(const NoteState& noteState, const int samplePosition);
    std::vector<NoteState> getMappedNotes(const NoteState& noteState);
    void playCurrentNotes(const int samplePosition);
    void stopCurrentNotes(const uint8 velocity, const int samplePosition);
    void removeHeldNote(const int note);

    void arpeggiate(const int numSamples, const int noteDuration);
    int getNoteDuration(juce::AudioPlayHead* playHead);
    // -----------------------------------

    // -----------------------------------
    // Manage mapping values
    void initParameters();
    void updateMidiParams();
    void updateArpeggiatorParams();
    void updateNoteMapping(const NoteParam& noteParam);
    // -----------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiProcessor)
};
