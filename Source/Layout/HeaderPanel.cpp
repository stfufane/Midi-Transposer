#include "Panels.h"

HeaderPanel::HeaderPanel(MidiParams& midiParams)
{
    inputChannel = std::make_unique< AttachedComponent<RotarySlider, juce::SliderParameterAttachment> >(
        *midiParams.inputChannel, *this,
        [](RotarySlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
        }
    );

    outputChannel = std::make_unique< AttachedComponent<RotarySlider, juce::SliderParameterAttachment> >(
        *midiParams.outputChannel, *this,
        [](RotarySlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("The transposed events will be routed to this channel.");
        }
    );

    octaveTranspose = std::make_unique< AttachedComponent<RotarySlider, juce::SliderParameterAttachment> >(
        *midiParams.octaveTranspose, *this,
        [](RotarySlider& slider) {
            slider.setNormalisableRange({-1, 4, 1});
            slider.setTooltip("This will play the root note at its original position and transpose the chord.");
        }
    );
}

void HeaderPanel::paint(juce::Graphics& g)
{
    g.drawImage(background, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 
        0, 0, background.getWidth(), background.getHeight());
}

void HeaderPanel::resized()
{
    juce::Grid grid;

    grid.templateRows    = { Track (Fr(1)) };
    grid.templateColumns = { Track (Fr(1)), Track(Fr(1)), Track(Fr(1)) };

    grid.items = { 
        juce::GridItem(inputChannel->component).withMargin(juce::GridItem::Margin(15)), 
        juce::GridItem(outputChannel->component).withMargin(juce::GridItem::Margin(15)), 
        juce::GridItem(octaveTranspose->component).withMargin(juce::GridItem::Margin(15)) 
    };

    grid.performLayout (getLocalBounds());
}