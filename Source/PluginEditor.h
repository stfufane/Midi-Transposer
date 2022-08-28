#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/MainPanel.h"

class MidiBassPedalChordsAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p);

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    /**
     * @brief Arranges the different sections of the plugin in one place.
     */
    Gui::MainPanel mainPanel;

    juce::TooltipWindow tooltipWindow;

    static constexpr float kWindowRatio = 1.25f;
    static constexpr int kWindowWidth = 600;
    static constexpr int kWindowHeight = 480;
    static constexpr float kMaxResize = 1.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
