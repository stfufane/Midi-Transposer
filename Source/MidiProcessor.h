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
    static StringArray midiChannels;
    static StringArray notes;
    static StringArray chords;

    std::atomic<float>* inputChannelParameter = nullptr;
    std::atomic<float>* outputChannelParameter = nullptr;
    int inputChannel = 1;
    int outputChannel = 1;

    std::vector<AudioParameterChoice*> noteParameters;
    std::vector<AudioParameterChoice*> chordParameters;

    MidiProcessor(AudioProcessorValueTreeState& state);

    void process(MidiBuffer& midiMessages);

    static void addMappingParameters(AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addChannelParameters(AudioProcessorValueTreeState::ParameterLayout& layout);

    void valueTreePropertyChanged(ValueTree& treeWhosePropertyChanged, const Identifier& property) override;

private:
    AudioProcessorValueTreeState& state;
    std::vector< std::vector<int> > mapping_notes;
    int lastNoteOn = 0;
    
    void mapNote(int note, juce::uint8 velocity, bool noteOn, int time, MidiBuffer& processedMidi);
    void updateMapping();
    void addMappedNotes(const int note_origine, const int new_note, const String chord);
};