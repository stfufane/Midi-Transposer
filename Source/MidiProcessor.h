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
    std::vector< std::vector<int> > mapping_notes;

    AudioParameterInt* inputChannelParameter;
    AudioParameterInt* outputChannelParameter;
    std::vector<AudioParameterChoice*> noteParameters;
    std::vector<AudioParameterChoice*> chordParameters;

    int inputChannel = 1;
    int outputChannel = 1;
    int lastNoteOn = -1;
    std::vector<int> currentNotesOn;
    
    void mapNote(int note, juce::uint8 velocity, bool noteOn, int time, MidiBuffer& processedMidi);
    void updateMapping();
    void addMappedNotes(const int note_origine, const int new_note, const String chord);
};