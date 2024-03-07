#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "params/Params.h"

class MidiProcessor
{
public:
    MidiProcessor() = default;

    void prepareToPlay(double rate);
    void process(juce::MidiBuffer& midiMessages, int numSamples, const juce::AudioPlayHead* playHead);
    void addParameters(juce::AudioProcessor& p);

    [[nodiscard]] int getLastNoteOn() const { return lastNoteOn.note; }

    Params::MidiParams& getMidiParams();

    Params::NoteParams& getNoteParams() { return noteParams; }
    Params::ArpeggiatorParams& getArpeggiatorParams() { return arpeggiatorParams; }
private:
    // Parameters declared in helper struct. The lambda will be called when a corresponding parameter is changed.
    Params::MidiParams midiParams;
    Params::NoteParams noteParams;
    Params::ArpeggiatorParams arpeggiatorParams;

    juce::MidiBuffer processedMidi;

    std::vector< std::set<int> > notesMapping { 12, std::set({ 0 }) };
    
    struct NoteState {
        int note { -1 };
        int channel { 0 };
        juce::uint8 velocity { 0 };

        void reset() { note = -1; }
    };
    NoteState lastNoteOn;
    std::vector<NoteState> currentInputNotesOn;
    std::vector<NoteState> currentOutputNotesOn;

    // Used to calculate the arpeggiator note positions.
    struct Arpeggiator 
    {
        float     sampleRate { 0.f };
        int       time { 0 };
        double    division { 0. };
        double    nextBeatPosition { 0. };
        int       currentIndex { 0 };
        int       noteUpdated { -1 };
        NoteState currentNote;

        void reset()
        {
            time             = 0;
            currentIndex     = 0;
            noteUpdated      = -1;
            division         = 0.;
            nextBeatPosition = 0.;
            currentNote.reset();
        }
    };
    Arpeggiator arp;
    
    // -----------------------------------
    // Process the input midi events
    void mapNote(const juce::MidiMessage& m, int samplePosition);
    void playMappedNotes(const NoteState& noteState, int samplePosition);
    [[nodiscard]] std::vector<NoteState> getMappedNotes(const NoteState& noteState) const;
    void playCurrentNotes(int samplePosition);
    void stopCurrentNotes(juce::uint8 velocity, int samplePosition);
    void removeHeldNote(int note);

    void processArpeggiator(int numSamples, const juce::AudioPlayHead* playHead);
    int  getArpeggiatorNoteDuration(const juce::AudioPlayHead::PositionInfo& positionInfo);
    void arpeggiate(int numSamples, const juce::AudioPlayHead::PositionInfo& positionInfo);
    void arpeggiateSync(int numSamples, const juce::AudioPlayHead::PositionInfo& positionInfo);
    void playArpeggiatorNote(int offset);
    // -----------------------------------

    // -----------------------------------
    // Manage mapping values
    void initParameters();
    void updateNoteMapping(const Params::NoteParam& inNoteParam);
    // -----------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiProcessor)
};
