#include "PluginEditor.h"
#include "MidiProcessor.h"

constexpr auto WINDOW_RATIO = 16.0f / 10.0f;
constexpr auto WINDOW_WIDTH = 800.0f;
constexpr auto WINDOW_HEIGHT = WINDOW_WIDTH / WINDOW_RATIO;
constexpr auto MAX_RESIZE = 1.5f;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), 
      processor(p), 
      uiSettings(p.getUISettings()),
      noteParams(p.getMidiProcessor().getNoteParams()),
      midiParams(p.getMidiProcessor().getMidiParams()),
      headerPanel(p.getMidiProcessor().getMidiParams(), p.getMidiProcessor().getArpeggiatorParams()),
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
                intervalsPanel->setVisible(false);
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

MidiBassPedalChordsAudioProcessorEditor::~MidiBassPedalChordsAudioProcessorEditor() = default;

//==============================================================================
void MidiBassPedalChordsAudioProcessorEditor::paint(juce::Graphics& g) 
{
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 
        0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
}

void MidiBassPedalChordsAudioProcessorEditor::resized() 
{
    processor.setEditorSize(getWidth(), getHeight());

    juce::Grid grid;
 
    grid.templateColumns    = { Track(Fr(1)) };
    grid.templateRows       = { Track(Fr(3)), Track(Fr(7)) };

    grid.items = { juce::GridItem(headerPanel), juce::GridItem(keysPanel) };

    grid.performLayout (getLocalBounds());

    if (intervalsPanel != nullptr)
    {
        auto intervalsHeight = headerPanel.getHeight();
        intervalsPanel->setBounds(0, 0, getWidth(), intervalsHeight);
    }         
}

void MidiBassPedalChordsAudioProcessorEditor::initIntervalsPanel(NoteParam& noteParam)
    {
        intervalsPanel = std::make_unique<IntervalsPanel>(noteParam);
        addAndMakeVisible(intervalsPanel.get());
        resized();
    }

void MidiBassPedalChordsAudioProcessorEditor::updateNoteEdited(const int index)
{
    for (auto& noteKey: keysPanel.noteKeys) 
    {
        noteKey->isEdited = (noteKey->noteIndex == index);
    }
    headerPanel.setAlpha((float) index == -1);
    repaint();
}
