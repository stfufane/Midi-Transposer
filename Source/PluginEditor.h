/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct KeyPosition {
    Image* keyImage;
    int x;
    int y;
};

//==============================================================================
/**
*/
class MidiBassPedalChordsAudioProcessorEditor : public AudioProcessorEditor, private Timer
{
public:
    MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor&, AudioProcessorValueTreeState&);
    ~MidiBassPedalChordsAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    MidiBassPedalChordsAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    Image backgroundImage;
    Image whiteKey;
    Image blackKey;

    int currentNotePlayed = -1;

    const std::vector<KeyPosition> keyPositions = {
        { &whiteKey, 41, 401 }, // C
        { &blackKey, 84, 297 }, // C#
        { &whiteKey, 133, 401 }, // D
        { &blackKey, 177, 297 }, // D#
        { &whiteKey, 223, 401 }, // E
        { &whiteKey, 313, 401 }, // F
        { &blackKey, 359, 297 }, // F#
        { &whiteKey, 403, 401 }, // G
        { &blackKey, 449, 297 }, // G#
        { &whiteKey, 494, 401 }, // A
        { &blackKey, 539, 297 }, // A#
        { &whiteKey, 587, 401 }, // B
    };

    TooltipWindow tooltipWindow;

    ComboBox inputChannelChoice;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> inputChannelAttachment;

    ComboBox outputChannelChoice;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> outputChannelAttachment;

    ToggleButton bypassChannels;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> bypassChannelsAttachment;

    ComboBox octaveTransposeChoice;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> octraveTransposeAttachment;

    std::vector <std::unique_ptr<ComboBox>> noteChoices;
    std::vector<std::unique_ptr<ComboBox>> chordChoices;

    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> noteChoicesAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> chordChoicesAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
