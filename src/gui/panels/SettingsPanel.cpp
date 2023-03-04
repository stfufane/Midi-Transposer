#include "SettingsPanel.h"

namespace Gui
{

SettingsPanel::SettingsPanel(MidiBassPedalChordsAudioProcessor& p)
    : juce::Component("Side Panel")
{
    auto& midiParams = p.getMidiProcessor().getMidiParams();
    auto& arpParams = p.getMidiProcessor().getArpeggiatorParams();

    inputChannel = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *midiParams.inputChannel, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
            slider.setPopupDisplayEnabled(true, true, nullptr);
        },
        "Midi Input Slider", juce::Slider::SliderStyle::LinearHorizontal
    );

    outputChannel = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *midiParams.outputChannel, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("The transposed events will be routed to this channel.");
            slider.setPopupDisplayEnabled(true, true, nullptr);
        },
        "Midi Output Slider", juce::Slider::SliderStyle::LinearHorizontal
    );

    octaveTranspose = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *midiParams.octaveTranspose, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({-1, 4, 1});
            slider.setTooltip("This will play the root note at its original position and transpose the chord.");
            slider.setPopupDisplayEnabled(true, true, nullptr);
            slider.setCustomTextLambda([](double value) -> juce::String {
                return juce::String(value) + " octave" + (std::abs(value) > 1 ? "s" : "");
            });
        },
        "Octave Slider", juce::Slider::SliderStyle::LinearHorizontal
    );

    arpActivated = std::make_unique< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> >(
        *arpParams.activated, *this,
        [](juce::ToggleButton& button) {
            button.setButtonText("Arpeggiator");
        }
    );

    arpRate = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *arpParams.rate, *this,
        [](Gui::CustomSlider& slider) {
            slider.setRange(0., 1.0, 0.01);
            slider.setNumDecimalPlacesToDisplay(2);
            slider.setPopupDisplayEnabled(true, true, nullptr);
        },
        "Arp Rate Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    arpSyncRate = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *arpParams.syncRate, *this,
        [](Gui::CustomSlider& slider) {
            double nbDivisions = (double) Notes::divisions.size() - 1;
            slider.setNormalisableRange({ 0, nbDivisions, 1 });
            slider.setPopupDisplayEnabled(true, true, nullptr);
            slider.setCustomTextLambda([](double value) -> juce::String {
                return Notes::divisions[static_cast<size_t>(value)].label;
            });
        },
        "Arp Sync Rate Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    arpSynced = std::make_unique< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> >(
        *arpParams.synced, *this,
        [this](juce::ToggleButton& button) {
            button.setButtonText("Tempo Sync");
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
    ioLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ioLabel);
}

void SettingsPanel::paint(juce::Graphics& g)
{
    // TODO some custom stuff.
}

void SettingsPanel::resized()
{
    using juce::operator""_px;
    using juce::operator""_fr;

    arpRate->getComponent().setVisible(!arpSynced->getComponent().getToggleState());
    arpSyncRate->getComponent().setVisible(arpSynced->getComponent().getToggleState());

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateRows    = { Track (1_fr), Track(1_fr) };
    grid.templateColumns       = { Track (1_fr), Track (1_fr), Track (1_fr),
                                   Track (1_fr), Track (1_fr), };

    grid.alignContent    = juce::Grid::AlignContent::center;
    grid.justifyContent  = juce::Grid::JustifyContent::center;
    grid.alignItems      = juce::Grid::AlignItems::center;
    grid.justifyItems    = juce::Grid::JustifyItems::center;

    grid.columnGap = 10_px;
    grid.rowGap = 10_px;

    grid.items = {
            juce::GridItem(inputChannel->getComponent()),
            juce::GridItem(outputChannel->getComponent()),
            juce::GridItem(octaveTranspose->getComponent()),
            juce::GridItem(arpActivated->getComponent()),
            juce::GridItem(lblArpRate),
            juce::GridItem(lblInputChannel),
            juce::GridItem(lblOutputChannel),
            juce::GridItem(lblOctaveTranspose),
            juce::GridItem(arpSynced->getComponent()),
            juce::GridItem(arpSynced->getComponent().getToggleState() ? arpSyncRate->getComponent() : arpRate->getComponent())
    };

    grid.performLayout (getLocalBounds());
}

}