#include "PluginEditor.h"

constexpr auto WINDOW_RATIO = 1.5; //12.0 / 9.0;
constexpr auto WINDOW_WIDTH = 600;
constexpr auto WINDOW_HEIGHT = WINDOW_WIDTH / WINDOW_RATIO;
constexpr auto MAX_RESIZE = 1.5;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : AudioProcessorEditor(&p), 
      processor(p), 
      uiSettings(p.getUISettings()),
      noteParams(p.getMidiProcessor().getNoteParams()),
      midiParams(p.getMidiProcessor().getMidiParams()),
      headerPanel(p.getMidiProcessor().getMidiParams()),
      keysPanel(p.getMidiProcessor().getNoteParams())
{
    setResizable(true, true);
    // Restore the last size if it exists.
    if (uiSettings.width != 0) {
        setSize(uiSettings.width, uiSettings.height);
    } else {
        setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
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
            if (uiSettings.lastNoteIndex == index)
            {
                intervalsPanel.get()->setVisible(false);
                resized();
                index = -1;
            }
            else
            {
                auto noteParam = noteParams.notes[index].get();
                initIntervalsPanel(*noteParam);
            }
            
            updateNoteEdited(index);
            uiSettings.lastNoteIndex = index;
        };
    }
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
    grid.templateRows       = { Track(Fr(3)), Track(Fr(7)) };

    grid.items = { GridItem(headerPanel), GridItem(keysPanel) };

    grid.performLayout (getLocalBounds());

    if (intervalsPanel != nullptr)
    {
        auto intervalsHeight = headerPanel.getHeight();
        intervalsPanel.get()->setBounds(0, 0, getWidth(), intervalsHeight);
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
