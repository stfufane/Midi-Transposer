#include "Panels.h"

HeaderPanel::HeaderPanel(MidiParams& midiParams, ArpeggiatorParams& arpParams)
{
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
    
    addAndMakeVisible(lblInputChannel);
    addAndMakeVisible(lblOutputChannel);
    addAndMakeVisible(lblOctaveTranspose);
    addAndMakeVisible(lblArpRate);
}

void HeaderPanel::paint(juce::Graphics&) {}

void HeaderPanel::resized()
{
    juce::Grid grid;

    grid.templateRows    = { Track (Fr(1)), Track (Fr(1)), Track (Fr(1)) };
    grid.templateColumns = { Track (Fr(1)), Track (Fr(1)), Track (Fr(1)), Track(Fr(1)), Track(Fr(1)) };
    grid.alignContent    = Grid::AlignContent::center;
    grid.justifyContent  = Grid::JustifyContent::center;
    grid.alignItems      = Grid::AlignItems::center;
    grid.justifyItems    = Grid::JustifyItems::center;

    arpRate->component.setVisible(!arpSynced->component.getToggleState());
    arpSyncRate->component.setVisible(arpSynced->component.getToggleState());

    grid.items = {
        juce::GridItem(lblInputChannel).withArea(1, 1),
        juce::GridItem(inputChannel->component).withArea(1, 2), 
        juce::GridItem(lblOutputChannel).withArea(2, 1),
        juce::GridItem(outputChannel->component).withArea(2, 2),
        juce::GridItem(lblOctaveTranspose).withArea(3, 1),
        juce::GridItem(octaveTranspose->component).withArea(3, 2),
        juce::GridItem(arpActivated->component).withArea(1, 4),
        juce::GridItem(arpSynced->component).withArea(1, 5),
        juce::GridItem(lblArpRate).withArea(2, 4),
        juce::GridItem(arpSyncRate->component).withArea(2, 5),
        juce::GridItem(arpRate->component).withArea(2, 5)
    };
    
    grid.performLayout (getLocalBounds().reduced(0, getHeight() / 10.f));

    // Make the rate slider text box larger.
    auto& rateSlider = arpSyncRate->component;
    rateSlider.setTextBoxStyle(rateSlider.getTextBoxPosition(), false, rateSlider.getWidth() * 0.9f, rateSlider.getTextBoxHeight());
}