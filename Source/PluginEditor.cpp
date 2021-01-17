#include "PluginEditor.h"

constexpr auto WINDOW_RATIO = 1.5; //12.0 / 9.0;
constexpr auto WINDOW_WIDTH = 600;
constexpr auto WINDOW_HEIGHT = WINDOW_WIDTH / WINDOW_RATIO;
constexpr auto MAX_RESIZE = 1.5;

// using Track = Grid::TrackInfo;
// using Fr = Grid::Fr;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : AudioProcessorEditor(&p), 
      processor(p), 
      uiSettings(p.uiSettings),
      noteParams(p.midiProcessor.noteParams),
      midiParams(p.midiProcessor.midiParams),
      headerPanel(p.midiProcessor.midiParams),
      keysPanel(p.midiProcessor.noteParams)
{
    setResizable(true, true);
    // Restore the last size if it exists.
    // if (processor.uiSettings.width != 0) {
    //     setSize(processor.uiSettings.width, processor.uiSettings.height);
    // } else {
        setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // }
    setResizeLimits(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH * MAX_RESIZE, WINDOW_HEIGHT * MAX_RESIZE);
    getConstrainer()->setFixedAspectRatio(WINDOW_RATIO);

    addAndMakeVisible(headerPanel);
    addAndMakeVisible(keysPanel);

    auto index = uiSettings.lastNoteIndex;
    updateNoteEdited(index);
    if (index > -1) 
    {
        auto noteParam = noteParams.notes[index].get();
        initIntervalsPanel(*noteParam);
    }
    
    for (auto& noteKey: keysPanel.noteKeys) 
    {
        noteKey->changeNote = [this] (int index) 
        { 
            auto noteParam = noteParams.notes[index].get();
            initIntervalsPanel(*noteParam);
            updateNoteEdited(index);
            uiSettings.lastNoteIndex = index;
        };
    }

    // Reset the edited note when clicking the header.
    headerPanel.onClick = [this]()
    {
        updateNoteEdited(-1);
        if (intervalsPanel != nullptr)
            intervalsPanel.get()->setVisible(false);
    };
}

MidiBassPedalChordsAudioProcessorEditor::~MidiBassPedalChordsAudioProcessorEditor() { }

//==============================================================================
void MidiBassPedalChordsAudioProcessorEditor::paint(Graphics& g) 
{
    g.fillAll(Colours::white);
}

void MidiBassPedalChordsAudioProcessorEditor::resized() 
{
    processor.setEditorSize(getWidth(), getHeight());

    Grid grid;
 
    grid.templateColumns    = { Track(Fr(1)) };
    grid.templateRows       = { Track(Fr(2)), Track(Fr(7)) };

    grid.items = { GridItem(headerPanel), GridItem(keysPanel) };

    grid.performLayout (getLocalBounds());

    if (intervalsPanel != nullptr)
    {
        auto width = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();
        auto intervalsHeight = width / 4.0f;
        intervalsPanel.get()->setBounds(0, height - intervalsHeight, width, intervalsHeight);
    }
}

void MidiBassPedalChordsAudioProcessorEditor::initIntervalsPanel(NoteParam& noteParam)
    {
        intervalsPanel.reset(new IntervalsPanel(noteParam));
        addAndMakeVisible(intervalsPanel.get());
        resized();
    }

void MidiBassPedalChordsAudioProcessorEditor::updateNoteEdited(const int index)
{
    for (auto& noteKey: keysPanel.noteKeys) 
    {
        noteKey->isEdited = (noteKey->noteIndex == index);
    }
    repaint();
}
