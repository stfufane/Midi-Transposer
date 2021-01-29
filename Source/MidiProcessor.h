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
    MidiParams midiParams;
    NoteParams noteParams;
    ArpeggiatorParams arpeggiatorParams;

    juce::MidiBuffer processedMidi;

    std::vector< std::vector<int> > notesMapping;
    
    struct NoteState {
        int note { -1 };
        int channel;
        juce::uint8 velocity;

        void reset() { note = -1; }
    };
    NoteState lastNoteOn;
    std::vector<NoteState> currentInputNotesOn;
    std::vector<NoteState> currentOutputNotesOn;

    // Used to calculate the arpeggiator note positions.
    struct Arpeggiator 
    {
        float     sampleRate;
        int       time;
        double    division;
        double    lastBeatPosition;
        int       currentIndex;
        NoteState currentNote;

        void reset()
        {
            time             = 0;
            currentIndex     = 0;
            division         = 0.;
            lastBeatPosition = 0.;
            currentNote.reset();
        }
    };
    Arpeggiator arp;
    
    // -----------------------------------
    // Process the input midi events
    void mapNote(const juce::MidiMessage& m, const int samplePosition);
    void playMappedNotes(const NoteState& noteState, const int samplePosition);
    std::vector<NoteState> getMappedNotes(const NoteState& noteState) const;
    void playCurrentNotes(const int samplePosition);
    void stopCurrentNotes(const uint8 velocity, const int samplePosition);
    void removeHeldNote(const int note);

    void processArpeggiator(const int numSamples, juce::AudioPlayHead* playHead);
    int  getArpeggiatorNoteDuration(const juce::AudioPlayHead::CurrentPositionInfo& positionInfo);
    void arpeggiate(const int numSamples, const juce::AudioPlayHead::CurrentPositionInfo& positionInfo);
    void arpeggiateSync(const int numSamples, const juce::AudioPlayHead::CurrentPositionInfo& positionInfo);
    void playArpeggiatorNote(const int offset);
    // -----------------------------------

    // -----------------------------------
    // Manage mapping values
    void initParameters();
    void updateNoteMapping(const NoteParam& noteParam);
    // -----------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiProcessor)
};
