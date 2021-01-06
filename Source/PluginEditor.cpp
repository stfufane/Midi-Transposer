#include "PluginEditor.h"

constexpr auto WINDOW_RATIO = 9.0 / 5.4;
constexpr auto WINDOW_WIDTH = 900;
constexpr auto WINDOW_HEIGHT = WINDOW_WIDTH / WINDOW_RATIO;

using Track = Grid::TrackInfo;
using Fr = Grid::Fr;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p), midiPanel(p), notesPanel(p, 0)
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
    Grid grid;

    grid.templateColumns    = { Track (Fr(1)), Track(Fr(4)) };
    grid.templateRows       = { Track (Fr(1)) };

    grid.items = { GridItem(midiPanel), GridItem(notesPanel) };

    grid.performLayout (getLocalBounds());

    processor.setUISettings(getWidth(), getHeight());
}
