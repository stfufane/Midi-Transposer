#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/Panels.h"

class MidiBassPedalChordsAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor&);
    ~MidiBassPedalChordsAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    /**
     * @brief Resets the intervals panel with the currently edited note.
     * @param noteParam the note that is edited.
     */
    void initIntervalsPanel(NoteParam& noteParam);
    /**
     * @brief Notifies all the note keys to tell which one is edited and update its color.
     * @param index the index of the edited note. -1 means there's no note currently edited.
     */
    void updateNoteEdited(int index);

    /**
     * @brief Contains the main parameters (midi channel, octave, transpose, arpeggiator, presets)
     */
    HeaderPanel headerPanel;
    /**
     * @brief Contains the twelve keys that trigger the display of the intervals panel.
     */
    KeysPanel keysPanel;
    /**
     * @brief Contains toggle buttons to activate the intervals for a certain key.
     */
    std::unique_ptr<IntervalsPanel> intervalsPanel = nullptr;

    juce::Image backgroundImage { juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize) };

    juce::TooltipWindow tooltipWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
