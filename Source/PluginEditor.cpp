#include "PluginEditor.h"

constexpr auto WINDOW_RATIO = 16.0 / 9.0;
constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = WINDOW_WIDTH / WINDOW_RATIO;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p), midiPanel(p), notesPanel(p)
{
    // addAndMakeVisible(midiPanel);
    addAndMakeVisible(notesPanel);

    setResizable(true, true);
    // Restore the last size if it exists.
    if (processor.uiSettings.exists) {
        setSize(processor.uiSettings.width, processor.uiSettings.height);
    } else {
        setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    setResizeLimits(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH * 1.5, WINDOW_HEIGHT * 1.5);
    getConstrainer()->setFixedAspectRatio(WINDOW_RATIO);
}

MidiBassPedalChordsAudioProcessorEditor::~MidiBassPedalChordsAudioProcessorEditor() { }

//==============================================================================
void MidiBassPedalChordsAudioProcessorEditor::paint(Graphics& g) 
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));    
}

void MidiBassPedalChordsAudioProcessorEditor::resized() 
{
    notesPanel.setBounds(0, 0, getLocalBounds().getWidth(), getHeight());
    // midiPanel.setBounds(0, getHeight() - 100, getLocalBounds().getWidth(), 100);
    processor.setUISettings(getWidth(), getHeight());
}
