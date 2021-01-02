#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Layout/Helpers.h"
#include "Layout/MidiPanel.h"
#include "Layout/NotesPanel.h"

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

    MidiPanel midiPanel;
    NotesPanel notesPanel;

    TooltipWindow tooltipWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
