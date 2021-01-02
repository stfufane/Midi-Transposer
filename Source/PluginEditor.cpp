#include "PluginEditor.h"

constexpr auto WINDOW_WIDTH = 500;
constexpr auto WINDOW_HEIGHT = 400;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p), midiPanel(p), notesPanel(p)
{
    addAndMakeVisible(midiPanel);
    addAndMakeVisible(notesPanel);

    setResizable(true, true);
    // Restore the last size if it exists.
    if (processor.uiSettings.exists) {
        setSize(processor.uiSettings.width, processor.uiSettings.height);
    } else {
        setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    setResizeLimits(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH * 1.5, WINDOW_HEIGHT * 1.5);
    getConstrainer()->setFixedAspectRatio(5.0 / 4.0);
}

MidiBassPedalChordsAudioProcessorEditor::~MidiBassPedalChordsAudioProcessorEditor() { }

//==============================================================================
void MidiBassPedalChordsAudioProcessorEditor::paint(Graphics& g) 
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));    
}

void MidiBassPedalChordsAudioProcessorEditor::resized() 
{
    midiPanel.setBounds(0, 0, getLocalBounds().getWidth(), 100);
    notesPanel.setBounds(0, 100, getLocalBounds().getWidth(), getHeight() - 100);
    processor.setUISettings(getWidth(), getHeight());
}
