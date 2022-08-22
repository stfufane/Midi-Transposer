#include "PluginEditor.h"

constexpr auto kWindowRatio = 16.0f / 10.0f;
constexpr auto kWindowWidth = 800.0f;
constexpr auto kWindowHeight = kWindowWidth / kWindowRatio;
constexpr auto kMaxResize = 1.5f;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), 
      headerPanel(p),
      keysPanel(p.getMidiProcessor().getNoteParams())
{
    setResizable(true, true);

    // Retrieve useful data from the processor.
    auto& uiSettings = p.getUISettings();
    auto& noteParams = p.getMidiProcessor().getNoteParams();

    // Restore the last size if it exists.
    if (uiSettings.width != 0) {
        setSize(uiSettings.width, uiSettings.height);
    } else {
        setSize(kWindowWidth, kWindowHeight);
    }
    setResizeLimits(kWindowWidth, kWindowHeight, kWindowWidth * kMaxResize, kWindowHeight * kMaxResize);
    getConstrainer()->setFixedAspectRatio(kWindowRatio);

    addAndMakeVisible(headerPanel);
    addAndMakeVisible(keysPanel);

    auto index = uiSettings.lastNoteIndex;
    updateNoteEdited(index);
    if (index > -1) {
        auto noteParam = noteParams.notes[index].get();
        initIntervalsPanel(*noteParam);
    }
    
    for (auto& noteKey: keysPanel.noteKeys) {
        noteKey->changeNote = [this, &uiSettings, &noteParams] (int index)
        {
            if (uiSettings.lastNoteIndex == index) {
                intervalsPanel->setVisible(false);
                resized();
                index = -1;
            }
            else {
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
    auto* p = dynamic_cast<MidiBassPedalChordsAudioProcessor*>(&processor);
    p->saveEditorSize(getWidth(), getHeight());

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
    headerPanel.setAlpha(index == -1 ? 1.f : 0.f);
    repaint();
}
