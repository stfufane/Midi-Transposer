#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../PluginProcessor.h"

using Track = Grid::TrackInfo;
using Fr = Grid::Fr;

struct MidiPanel : public Component
{
    MidiPanel(MidiParams& midiParams)
    {
        inputChannel = std::make_unique< AttachedComponent<Slider, SliderParameterAttachment> >(
            *midiParams.inputChannel, *this,
            [](Slider& slider) {
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setNormalisableRange({1, 16, 1});
                slider.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
            }
        );

        outputChannel = std::make_unique< AttachedComponent<Slider, SliderParameterAttachment> >(
            *midiParams.outputChannel, *this,
            [](Slider& slider) {
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setNormalisableRange({1, 16, 1});
                slider.setTooltip("The transposed events will be routed to this channel.");
            }
        );

        octaveTranspose = std::make_unique< AttachedComponent<NumericSlider, SliderParameterAttachment> >(
            *midiParams.octaveTranspose, *this,
            [](NumericSlider& slider) {
                slider.setUnity("octave");
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setNormalisableRange({-1, 4, 1});
                slider.setTooltip("This will play the root note at its original position and transpose the chord.");
            }
        );
    }

    void paint(Graphics&) override { }

    void resized() override {         
        Grid grid;

        grid.templateColumns    = { Track (Fr(1)) };
        grid.templateRows       = { Track (Fr(1)), Track(Fr(1)), Track(Fr(1)) };

        grid.items = { 
            GridItem(inputChannel->component).withMargin(GridItem::Margin(15)), 
            GridItem(outputChannel->component).withMargin(GridItem::Margin(15)), 
            GridItem(octaveTranspose->component).withMargin(GridItem::Margin(15)) 
        };

        grid.performLayout (getLocalBounds());

        inputChannel->component.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);
        outputChannel->component.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);
        octaveTranspose->component.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);
    }

    std::unique_ptr< AttachedComponent<Slider, SliderParameterAttachment> > inputChannel;
    std::unique_ptr< AttachedComponent<Slider, SliderParameterAttachment> > outputChannel;
    std::unique_ptr< AttachedComponent<NumericSlider, SliderParameterAttachment> > octaveTranspose;
};