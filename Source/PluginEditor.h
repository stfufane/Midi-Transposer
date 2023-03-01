#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/BaseLookAndFeel.h"
#include "GUI/MainPanel.h"

class MidiBassPedalChordsAudioProcessorEditor : public juce::AudioProcessorEditor
        , public juce::KeyListener
{
public:
    explicit MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p);
    ~MidiBassPedalChordsAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    /**
     * @brief Reload the GUI when pressing F5
     */
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

private:
    Gui::BaseLookAndFeel mLookAndFeel;

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
