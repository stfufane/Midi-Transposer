#pragma once

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"
#include "gin/utilities/gin_filesystemwatcher.h"

class MidiBassPedalChordsAudioProcessorEditor : public juce::AudioProcessorEditor, gin::FileSystemWatcher::Listener
{
public:
    explicit MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p);
    ~MidiBassPedalChordsAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void fileChanged(juce::File, gin::FileSystemWatcher::FileSystemEvent) override;

private:
    Gui::LnF::BaseLookAndFeel mLookAndFeel;

    gin::FileSystemWatcher mFileSystemWatcher;

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
