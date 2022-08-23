#include "SidePanel.h"

namespace Gui
{

SidePanel::SidePanel(MidiBassPedalChordsAudioProcessor& p)
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
            button.setButtonText("Arpeggiator");
        }
    );

    arpRate = std::make_unique< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> >(
        *arpParams.rate, *this,
        [](HorizontalSlider& slider) {
            slider.setRange(0., 1.0, 0.01);
            slider.textFromValueFunction = nullptr;
            slider.setNumDecimalPlacesToDisplay(2);
            slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, slider.getTextBoxWidth(), slider.getTextBoxHeight());
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

void SidePanel::initLabel(juce::Label& ioLabel)
{
    ioLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    ioLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ioLabel);
}

void SidePanel::resized()
{
    arpRate->component.setVisible(!arpSynced->component.getToggleState());
    arpSyncRate->component.setVisible(arpSynced->component.getToggleState());

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateColumns    = { Track (1_fr) };
    grid.templateRows       = { Track (1_fr), Track (1_fr), Track (1_fr), Track (1_fr),
                                Track (1_fr), Track (1_fr), Track (1_fr), Track (1_fr),
                                Track (1_fr), Track (1_fr), Track (1_fr) };

    grid.alignContent    = juce::Grid::AlignContent::center;
    grid.justifyContent  = juce::Grid::JustifyContent::center;
    grid.alignItems      = juce::Grid::AlignItems::center;
    grid.justifyItems    = juce::Grid::JustifyItems::center;

    grid.items = {
        juce::GridItem(lblInputChannel),
        juce::GridItem(inputChannel->component),
        juce::GridItem(lblOutputChannel),
        juce::GridItem(outputChannel->component),
        juce::GridItem(lblOctaveTranspose),
        juce::GridItem(octaveTranspose->component),
        juce::GridItem(arpActivated->component),
        juce::GridItem(arpSynced->component),
        juce::GridItem(lblArpRate),
        juce::GridItem(arpSynced->component.getToggleState() ? arpSyncRate->component : arpRate->component)
    };

    grid.performLayout (getLocalBounds());
//
//    // Make the rate slider text box larger.
//    auto& rateSlider = arpSyncRate->component;
//    rateSlider.setTextBoxStyle(rateSlider.getTextBoxPosition(),
//                               false,
//                               rateSlider.getWidth() * 10 / 9,
//                               rateSlider.getTextBoxHeight());
}

}