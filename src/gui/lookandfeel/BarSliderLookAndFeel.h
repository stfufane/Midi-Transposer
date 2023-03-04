#ifndef MIDIBASSPEDALCHORDS_BARSLIDERLOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BARSLIDERLOOKANDFEEL_H

#include "JuceHeader.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"

namespace Gui::LnF
{

class BarSliderLookAndFeel : public BaseLookAndFeel
{
public:
    explicit BarSliderLookAndFeel(Gui::Configuration* configuration)
        : BaseLookAndFeel(configuration)
    {}

protected:
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float /* minSliderPos */, float /* maxSliderPos */,
                          const juce::Slider::SliderStyle, juce::Slider& slider) override
    {
        const auto slider_value = slider.getTextFromValue(slider.getValue());
        // Fill the background.
        g.setColour(slider.findColour(juce::Slider::ColourIds::backgroundColourId));
        g.fillRect(juce::Rectangle<float>(static_cast<float>(width - height) / 2.f, 0.f,
                                            static_cast<float>(height), static_cast<float>(height)));

        g.setColour (slider.findColour (juce::Slider::ColourIds::trackColourId));
        g.fillRect (slider.isHorizontal() ?
            juce::Rectangle<float> (static_cast<float> (x), (float) y + 0.5f,  sliderPos - (float) x, (float) height - 1.0f)
          : juce::Rectangle<float> (static_cast<float>(width - height) / 2.f, sliderPos, static_cast<float>(height), (float) y + ((float) height - sliderPos)));

        g.setColour(slider.findColour(juce::Slider::ColourIds::textBoxTextColourId));
        g.drawText(slider_value, juce::Rectangle<float>(0.f, static_cast<float>(height) / 4.f,
                                                        static_cast<float>(width), static_cast<float>(height) / 2.f),
                   juce::Justification::centred);
    }
};

}

#endif //MIDIBASSPEDALCHORDS_BARSLIDERLOOKANDFEEL_H
