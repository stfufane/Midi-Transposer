#pragma once

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"

class MidiBassPedalChordsAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p);

    ~MidiBassPedalChordsAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;

    void resized() override;

private:
    /**
     * @brief The main LookAndFeel that will be applied to all the children components
     */
    Gui::LnF::BaseLookAndFeel mLookAndFeel;

    /**
     * @brief Arranges the different sections of the plugin in one place.
     */
    Gui::MainPanel mainPanel;

    juce::TooltipWindow tooltipWindow;

    static constexpr float kWindowRatio = 1.6f;
    static constexpr int kWindowWidth = 741;
    static constexpr int kWindowHeight = 462;
    static constexpr float kMaxResize = 1.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};