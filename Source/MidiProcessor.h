/*
  ==============================================================================

    MidiProcessor.h
    Created: 15 Apr 2020 7:12:57pm
    Author:  Stfufane

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace IDs
{
    static String groupChannels{ "channels" };
    static String paramInChannel{ "in_channel" };
    static String paramOutChannel{ "out_channel" };
}

class MidiProcessor : public ValueTree::Listener
{
public:
    static StringArray notes;
    static StringArray chords;

    MidiProcessor(AudioProcessorValueTreeState& state);

    void process(MidiBuffer& midiMessages);

    void valueTreePropertyChanged(ValueTree& treeWhosePropertyChanged, const Identifier& property) override;

private:
    AudioProcessorValueTreeState& state;
    const std::vector< std::vector<int> > chordIntervals = {
        {0},            // None
        {0, 4, 7},      // maj
        {0, 3, 7},      // min
        {0, 5, 7},      // sus4
        {0, 4, 7, 11},  // maj7
        {0, 3, 7, 10},  // min7
        {0, 4, 7, 11},  // 7
        {0, 3, 6, 10}   // m7b5
    };
    std::vector< std::vector<int> > mappingNotes;

    AudioParameterInt* inputChannelParameter;
    AudioParameterInt* outputChannelParameter;
    std::vector<AudioParameterChoice*> noteParameters;
    std::vector<AudioParameterChoice*> chordParameters;

    int inputChannel = 1;
    int outputChannel = 1;
    int lastNoteOn = -1;
    int currentNoteOutputChannel = 1;
    std::vector<int> currentInputNotesOn;
    std::vector<int> currentOutputNotesOn;
    
    void mapNote(int note, juce::uint8 velocity, bool noteOn, int time, MidiBuffer& processedMidi);
    void updateMapping();
    void addMappedNotes(const int note_origine, const int new_note, const int chord);
};