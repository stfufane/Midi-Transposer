#include "SettingsPanel.h"

namespace Gui
{

SettingsPanel::SettingsPanel(MidiBassPedalChordsAudioProcessor& p)
    : juce::Component("Side Panel")
{
    auto& midiParams = p.getMidiProcessor().getMidiParams();
    auto& arpParams = p.getMidiProcessor().getArpeggiatorParams();

    inputChannel = std::make_unique< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> >(
        *midiParams.inputChannel, *this,
        [](HorizontalSlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
        }
    );

    outputChannel = std::make_unique< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> >(
        *midiParams.outputChannel, *this,
        [](HorizontalSlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("The transposed events will be routed to this channel.");
        }
    );

    octaveTranspose = std::make_unique< AttachedComponent<OctaveSlider, juce::SliderParameterAttachment> >(
        *midiParams.octaveTranspose, *this,
        [](OctaveSlider& slider) {
            slider.setNormalisableRange({-1, 4, 1});
            slider.setTooltip("This will play the root note at its original position and transpose the chord.");
        }
    );

    arpActivated = std::make_unique< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> >(
        *arpParams.activated, *this,
        [](juce::ToggleButton& button) {
            button.setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
            button.setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);
            button.setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::darkgrey);
            button.setButtonText("Arpeggiator");
        }
    );

    arpRate = std::make_unique< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> >(
        *arpParams.rate, *this,
        [](HorizontalSlider& slider) {
            slider.setRange(0., 1.0, 0.01);
            slider.textFromValueFunction = nullptr;
            slider.setNumDecimalPlacesToDisplay(2);
        }
    );

    arpSyncRate = std::make_unique< AttachedComponent<SyncRateSlider, juce::SliderParameterAttachment> >(
        *arpParams.syncRate, *this,
        [](SyncRateSlider& slider) {
            double nbDivisions = (double) Notes::divisions.size() - 1;
            slider.setNormalisableRange({ 0, nbDivisions, 1 });
        }
    );

    arpSynced = std::make_unique< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> >(
        *arpParams.synced, *this,
        [this](juce::ToggleButton& button) {
            button.setButtonText("Tempo Sync");
            button.setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
            button.setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);
            button.setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::darkgrey);
            button.onStateChange = [this]() {
                resized();
            };
        }
    );

    initLabel(lblInputChannel);
    initLabel(lblOutputChannel);
    initLabel(lblOctaveTranspose);
    initLabel(lblArpRate);
}

void SettingsPanel::initLabel(juce::Label& ioLabel)
{
    ioLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    ioLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ioLabel);
}

void SettingsPanel::resized()
{
    arpRate->component.setVisible(!arpSynced->component.getToggleState());
    arpSyncRate->component.setVisible(arpSynced->component.getToggleState());

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateRows    = { Track (1_fr), Track(1_fr) };
    grid.templateColumns       = { Track (1_fr), Track (1_fr), Track (1_fr),
                                   Track (1_fr), Track (1_fr), };

    grid.alignContent    = juce::Grid::AlignContent::center;
    grid.justifyContent  = juce::Grid::JustifyContent::center;
    grid.alignItems      = juce::Grid::AlignItems::center;
    grid.justifyItems    = juce::Grid::JustifyItems::center;

    grid.items = {
            juce::GridItem(inputChannel->component),
            juce::GridItem(outputChannel->component),
            juce::GridItem(octaveTranspose->component),
            juce::GridItem(arpActivated->component),
            juce::GridItem(lblArpRate),
            juce::GridItem(lblInputChannel),
            juce::GridItem(lblOutputChannel),
            juce::GridItem(lblOctaveTranspose),
            juce::GridItem(arpSynced->component),
            juce::GridItem(arpSynced->component.getToggleState() ? arpSyncRate->component : arpRate->component)
    };

    grid.performLayout (getLocalBounds());

    arpSyncRate->component.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, arpSyncRate->component.getWidth(), arpSyncRate->component.getTextBoxHeight());
}

}