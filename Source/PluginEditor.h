#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Layout/Helpers.h"
#include "Layout/Panels.h"

class MidiBassPedalChordsAudioProcessorEditor : public AudioProcessorEditor
{
public:
    MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor&);
    ~MidiBassPedalChordsAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
private:
    MidiBassPedalChordsAudioProcessor& processor;

    UISettings& uiSettings;
    NoteParams& noteParams;
    MidiParams& midiParams;

    HeaderPanel headerPanel;
    KeysPanel keysPanel;
    std::unique_ptr<IntervalsPanel> intervalsPanel = nullptr;

    TooltipWindow tooltipWindow;

    void initIntervalsPanel(NoteParam& noteParam);
    void updateNoteEdited(const int index);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
