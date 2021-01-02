#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../PluginProcessor.h"

struct MidiPanel : public Component
{
    MidiPanel(MidiBassPedalChordsAudioProcessor& processor)
    {
        inputChannel = std::make_unique< AttachedComponent<Slider, SliderParameterAttachment> >(
            *processor.midiProcessor.midiParams.inputChannel, *this,
            [](Slider& slider) {
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
                slider.setNormalisableRange({1, 16, 1});
                slider.setBounds(0, 0, 100, 100);
                slider.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
            }
        );

        outputChannel = std::make_unique< AttachedComponent<Slider, SliderParameterAttachment> >(
            *processor.midiProcessor.midiParams.outputChannel, *this,
            [](Slider& slider) {
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
                slider.setBounds(100, 0, 100, 100);
                slider.setNormalisableRange({1, 16, 1});
                slider.setTooltip("The transposed events will be routed to this channel.");
            }
        );

        octaveTranspose = std::make_unique< AttachedComponent<Slider, SliderParameterAttachment> >(
            *processor.midiProcessor.midiParams.octaveTranspose, *this,
            [](Slider& slider) {
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
                slider.setBounds(200, 0, 100, 100);
                slider.setNormalisableRange({-1, 4, 1});
                slider.setTooltip("This will play the root note at its original position and transpose the chord.");
            }
        );

        bypassChannels = std::make_unique< AttachedComponent<ToggleButton, ButtonParameterAttachment> >(
            *processor.midiProcessor.midiParams.bypassOtherChannels, *this,
            [](ToggleButton& button) {
                button.setButtonText("Bypass other channels");
                button.setColour(ToggleButton::tickColourId, Colours::black);
                button.setColour(ToggleButton::tickDisabledColourId, Colours::black);
                button.setBounds(300, 0, 200, 100);
                button.changeWidthToFitText();
            }
        );
    }

    void paint(Graphics& g) override
    {
        g.fillAll(juce::Colours::lightgrey);
    }

    void resized() override { }

    std::unique_ptr< AttachedComponent<Slider, SliderParameterAttachment> >   inputChannel;
    std::unique_ptr< AttachedComponent<Slider, SliderParameterAttachment> >   outputChannel;
    std::unique_ptr< AttachedComponent<Slider, SliderParameterAttachment> >   octaveTranspose;
    std::unique_ptr< AttachedComponent<ToggleButton, ButtonParameterAttachment> > bypassChannels;
};