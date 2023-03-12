#include "MainPanel.h"

namespace Gui
{

MainPanel::MainPanel(MidiBassPedalChordsAudioProcessor& p)
        : juce::Component("Main Panel"),
          mConfiguration(CONFIG_FOLDER, this),
          presetsPanel(p.getPresetManager()),
          midiPanel(p),
          arpPanel(p),
          keysPanel(p.getMidiProcessor().getNoteParams())
{
    mConfiguration.addListener(this);

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
    addAndMakeVisible(midiPanel);
    addAndMakeVisible(arpPanel);
    addAndMakeVisible(dummyPanel);
    addAndMakeVisible(keysPanel);
    addAndMakeVisible(tooltipPanel);
}

MainPanel::~MainPanel()
{
    mConfiguration.removeListener(this);
}

void MainPanel::onConfigChanged(const CompCoordinates&)
{
    resized();
}

void MainPanel::resized()
{
    // Define if there's an interval panel to display or not.
    juce::Component* middleItem = nullptr;
    if (intervalsPanel != nullptr) {
        middleItem = intervalsPanel.get();
    } else {
        middleItem = &dummyPanel;
    }

    midiPanel.setBounds(mConfiguration.getData().mMidiPanel);
    arpPanel.setBounds(mConfiguration.getData().mArpPanel);
    presetsPanel.setBounds(mConfiguration.getData().mPresetsPanel);
    keysPanel.setBounds(mConfiguration.getData().mKeysPanel);
    middleItem->setBounds(mConfiguration.getData().mIntervalsPanel);
    tooltipPanel.setBounds(mConfiguration.getData().mTooltipsPanel);

    // Force resized on children because it's not triggered when using a transform to scale components.
    for (auto* child: getChildren()) {
        child->resized();
    }
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