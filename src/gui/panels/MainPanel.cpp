#include "MainPanel.h"
#include "gui/PluginEditor.h"

namespace Gui
{

MainPanel::MainPanel(MidiBassPedalChordsAudioProcessor& p)
        : juce::Component("Main Panel"),
          presetsPanel(p.getPresetManager()),
          settingsPanel(p),
          keysPanel(p.getMidiProcessor().getNoteParams())
{
    auto& uiSettings = p.getUISettings();
    auto& noteParams = p.getMidiProcessor().getNoteParams();
    const auto index = uiSettings.lastNoteIndex;

    updateNoteEdited(index);
    if (index > -1) {
        auto noteParam = noteParams.notes[static_cast<size_t>(index)].get();
        initIntervalsPanel(*noteParam);
    }

    for (auto& noteKey: keysPanel.getNoteKeys()) {
        noteKey->setChangeNoteCallback([this, &uiSettings, &noteParams](int idx) {
            if (uiSettings.lastNoteIndex == idx) {
                intervalsPanel.reset(nullptr);
                dummyPanel.setVisible(true);
                resized();
                idx = -1;
            } else {
                auto noteParam = noteParams.notes[static_cast<size_t>(idx)].get();
                initIntervalsPanel(*noteParam);
            }

            updateNoteEdited(idx);
            uiSettings.lastNoteIndex = idx;
        });
    }

    dummyPanel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(presetsPanel);
    addAndMakeVisible(settingsPanel);
    addAndMakeVisible(dummyPanel);
    addAndMakeVisible(keysPanel);
    addAndMakeVisible(tooltipPanel);
}

void MainPanel::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using juce::operator""_fr;

    grid.templateColumns    = { Track(1_fr) };
    grid.templateRows       = { Track(1_fr), Track(2_fr), Track(3_fr), Track(3_fr), Track(1_fr) };

    // Define if there's an interval panel to display or not.
    juce::Component* middleItem = nullptr;
    if (intervalsPanel != nullptr) {
        middleItem = intervalsPanel.get();
    } else {
        middleItem = &dummyPanel;
    }

    grid.items = { juce::GridItem(presetsPanel), juce::GridItem(settingsPanel), juce::GridItem(*middleItem), juce::GridItem(keysPanel), juce::GridItem(tooltipPanel) };

    grid.performLayout (getLocalBounds());
}

void MainPanel::initIntervalsPanel(Params::NoteParam& noteParam)
{
    intervalsPanel = std::make_unique<Gui::IntervalsPanel>(noteParam);
    dummyPanel.setVisible(false);
    addAndMakeVisible(intervalsPanel.get());
    resized();
}

void MainPanel::updateNoteEdited(const int index)
{
    keysPanel.setNoteKeyEdited(index);
    repaint();
}

}