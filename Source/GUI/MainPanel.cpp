#include "MainPanel.h"

namespace Gui
{

MainPanel::MainPanel(MidiBassPedalChordsAudioProcessor& p)
        : juce::Component("Main Panel"),
          keysPanel(p.getMidiProcessor().getNoteParams()),
          presetsPanel(p.getPresetManager()),
          settingsPanel(p)
{
    auto& uiSettings = p.getUISettings();
    auto& noteParams = p.getMidiProcessor().getNoteParams();

    const auto index = uiSettings.lastNoteIndex;
    updateNoteEdited(index);
    if (index > -1) {
        auto noteParam = noteParams.notes[index].get();
        initIntervalsPanel(*noteParam);
    }

    for (auto& noteKey: keysPanel.noteKeys) {
        noteKey->changeNote = [this, &uiSettings, &noteParams](int index) {
            if (uiSettings.lastNoteIndex == index) {
                intervalsPanel.reset(nullptr);
                dummyPanel.setVisible(true);
                resized();
                index = -1;
            } else {
                auto noteParam = noteParams.notes[index].get();
                initIntervalsPanel(*noteParam);
            }

            updateNoteEdited(index);
            uiSettings.lastNoteIndex = index;
        };
    }

    dummyPanel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    dummyPanel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(presetsPanel);
    addAndMakeVisible(settingsPanel);
    addAndMakeVisible(dummyPanel);
    addAndMakeVisible(keysPanel);
}

void MainPanel::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateColumns    = { Track(1_fr) };
    grid.templateRows       = { Track(1_fr), Track(2_fr), Track(3_fr), Track(3_fr) };

    // Define if there's an interval panel to display or not.
    juce::Component* middleItem = nullptr;
    if (intervalsPanel != nullptr) {
        middleItem = intervalsPanel.get();
    } else {
        middleItem = &dummyPanel;
    }

    grid.items = { juce::GridItem(presetsPanel), juce::GridItem(settingsPanel), juce::GridItem(*middleItem), juce::GridItem(keysPanel) };

    grid.performLayout (getLocalBounds());
}

void MainPanel::initIntervalsPanel(NoteParam& noteParam)
{
    intervalsPanel = std::make_unique<Gui::IntervalsPanel>(noteParam);
    dummyPanel.setVisible(false);
    addAndMakeVisible(intervalsPanel.get());
    resized();
}

void MainPanel::updateNoteEdited(const int index)
{
    for (auto& noteKey: keysPanel.noteKeys) {
        noteKey->isEdited = (noteKey->noteIndex == index);
    }
    repaint();
}

}