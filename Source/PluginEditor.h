/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiTransposerAudioProcessorEditor : public AudioProcessorEditor
{
public:
    MidiTransposerAudioProcessorEditor(MidiTransposerAudioProcessor&, AudioProcessorValueTreeState&);
    ~MidiTransposerAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

private:
    MidiTransposerAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    ComboBox inputChannelChoice;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> inputChannelAttachment;

    ComboBox outputChannelChoice;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> outputChannelAttachment;

    std::vector <std::unique_ptr<ComboBox>> noteChoices;
    std::vector<std::unique_ptr<ComboBox>> chordChoices;

    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> noteChoicesAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> chordChoicesAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiTransposerAudioProcessorEditor)
};