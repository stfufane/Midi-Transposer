#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/MainPanel.h"
#include "GUI/SidePanel.h"

class MidiBassPedalChordsAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p);

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    /**
     * @brief Contains the MIDI settings and arpeggiator settings
     */
    Gui::SidePanel sidePanel;

    /**
     * @brief Contains the preset panel, the keys panel and the intervals panel
     */
    Gui::MainPanel mainPanel;

    juce::TooltipWindow tooltipWindow;

    static constexpr float kWindowRatio = 1.5f;
    static constexpr int kWindowWidth = 600;
    static constexpr int kWindowHeight = 400;
    static constexpr float kMaxResize = 1.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
