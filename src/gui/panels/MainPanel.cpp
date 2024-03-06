#include "MainPanel.h"

#include "gui/lookandfeel/BaseLookAndFeel.h"

namespace Gui
{

MainPanel::MainPanel(MidiBassPedalChordsAudioProcessor& p)
        : juce::Component("Main Panel"),
          mConfiguration(CONFIG_FOLDER, this),
          presetsPanel(p.getPresetManager()),
          midiPanel(p),
          arpPanel(p)
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

    // Get the last note played from the audio processor.
    getNotePlayed = [&p]() {
        return p.getMidiProcessor().getLastNoteOn();
    };

    // TODO: custom dummy panel that displays all the transpositions
    // Invert label color so it's black on white.
    dummyPanel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    dummyPanel.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::whitesmoke);
    dummyPanel.setFont(LnF::getDefaultFont(28.f));
    dummyPanel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(presetsPanel);
    addAndMakeVisible(midiPanel);
    addAndMakeVisible(arpPanel);
    addAndMakeVisible(dummyPanel);
    addAndMakeVisible(keysPanel);
    addAndMakeVisible(tooltipPanel);

    startTimerHz(20);
}

MainPanel::~MainPanel()
{
    mConfiguration.removeListener(this);
}

void MainPanel::onConfigChanged(const CompCoordinates&)
{
    resized();
}

void MainPanel::timerCallback()
{
    if (getNotePlayed) {
        const auto note = getNotePlayed();
        keysPanel.setNotePlayed(note);
    }
}

void MainPanel::paint(juce::Graphics& g)
{
    // Draw all the frames for the different panels.
    const auto& coordinates = getCoordinates();

    // Midi frame
    g.setColour(findColour(juce::Label::ColourIds::backgroundColourId));
    g.drawRoundedRectangle(coordinates.mMidiPanel.reduced(coordinates.mMargin),
                           coordinates.mFrameCorner, 1.f);
    const auto midi_header_coordinates = juce::Rectangle<float>(0.f, 0.f,
                                                           coordinates.mMidiPanel.getWidth(),
                                                           coordinates.mHeaderHeight).reduced(coordinates.mMargin * 2.f);
    g.fillRoundedRectangle(midi_header_coordinates, coordinates.mFrameCorner / 2.f);
    g.drawRoundedRectangle(coordinates.mMidiLabels, coordinates.mKeyCorner, 1.f);
    g.drawRoundedRectangle(coordinates.mMidiOct, coordinates.mKeyCorner, 1.f);

    // Arp frame
    g.drawRoundedRectangle(coordinates.mArpPanel.reduced(coordinates.mMargin),
                           coordinates.mFrameCorner, 1.f);
    const auto arp_header_x = coordinates.mArpPanel.getX();
    const auto arp_header_coordinates = juce::Rectangle<float>(arp_header_x, 0.f,
                                                           coordinates.mArpPanel.getWidth(),
                                                           coordinates.mHeaderHeight).reduced(coordinates.mMargin * 2.f);
    g.fillRoundedRectangle(arp_header_coordinates, coordinates.mFrameCorner / 2.f);

    // Presets frame
    const auto presets_panel_x = coordinates.mPresetsPanel.getX();
    const auto presets_panel_width = static_cast<float>(coordinates.mPresetsPanel.getWidth());
    const auto presets_panel_height = static_cast<float>(coordinates.mPresetsPanel.getHeight());
    g.drawRoundedRectangle(juce::Rectangle<float>(presets_panel_x, 0.f,
                                          presets_panel_width, presets_panel_height).reduced(coordinates.mMargin),
                           coordinates.mFrameCorner, 1.f);
    const auto preset_header_coordinates = juce::Rectangle<float>(presets_panel_x, 0.f,
                                                                  presets_panel_width,
                                                           coordinates.mHeaderHeight).reduced(coordinates.mMargin * 2.f);
    g.fillRoundedRectangle(preset_header_coordinates, coordinates.mFrameCorner / 2.f);

    // Tooltips frame
    g.drawRoundedRectangle(coordinates.mTooltipsPanel.reduced(coordinates.mMargin),
                           coordinates.mFrameCorner, 1.f);
    
    // Intervals frame
    g.drawRoundedRectangle(coordinates.mIntervalsPanel.reduced(coordinates.mMargin),
                           coordinates.mFrameCorner, 1.f);
    g.drawRoundedRectangle(coordinates.mIntervalsTransposeLabel, coordinates.mIntervalsLabelCorner, 1.f);
    g.drawRoundedRectangle(coordinates.mIntervalsSlidersLabel, coordinates.mIntervalsLabelCorner, 1.f);

    // Header texts
    g.setColour(findColour(juce::Label::ColourIds::textColourId));
    g.setFont(LnF::getDefaultFont(coordinates.mHeaderFontSize));
    g.drawText("MIDI", midi_header_coordinates, juce::Justification::centred);
    g.drawText("ARP", arp_header_coordinates, juce::Justification::centred);
    g.drawText("PRESETS", preset_header_coordinates, juce::Justification::centred);
    
    // Label texts
    g.setFont(coordinates.mLabelFontSize);
    g.setColour(findColour(juce::Label::ColourIds::backgroundColourId));
    g.drawText("IN", coordinates.mMidiLabels.withX(coordinates.mMidiInX), juce::Justification::left);
    g.drawText("OUT", coordinates.mMidiLabels.withX(coordinates.mMidiOutX), juce::Justification::left);
    g.drawText(juce::CharPointer_UTF8("OCT±"), coordinates.mMidiOct.withX(coordinates.mMidiOctX), juce::Justification::left);
    
    g.setFont(coordinates.mIntervalsLabelFontSize);
    g.drawText("Transpose", coordinates.mIntervalsTransposeLabel, juce::Justification::centred);
}

void MainPanel::resized()
{
    const auto& coordinates = getCoordinates();

    // Define if there's an interval panel to display or not.
    juce::Component* middleItem = nullptr;
    if (intervalsPanel != nullptr) {
        middleItem = intervalsPanel.get();
        middleItem->setBounds(coordinates.mIntervalsPanel.toNearestInt());
    } else {
        middleItem = &dummyPanel;
        middleItem->setBounds(coordinates.mIntervalsPanel.toNearestInt().reduced(static_cast<int>(coordinates.mMargin) * 2));
    }

    midiPanel.setBounds(coordinates.mMidiPanel.toNearestInt());
    arpPanel.setBounds(coordinates.mArpPanel.toNearestInt());
    presetsPanel.setBounds(coordinates.mPresetsPanel.toNearestInt());
    keysPanel.setBounds(coordinates.mKeysPanel.toNearestInt());
    tooltipPanel.setBounds(coordinates.mTooltipsPanel.toNearestInt().reduced(static_cast<int>(coordinates.mMargin)));

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