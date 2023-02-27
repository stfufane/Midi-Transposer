#ifndef MIDIBASSPEDALCHORDS_BARSLIDERLOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BARSLIDERLOOKANDFEEL_H

#include <JuceHeader.h>
#include "BaseLookAndFeel.h"

namespace Gui
{

class BarSliderLookAndFeel : public BaseLookAndFeel
{
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos,
                          const Slider::SliderStyle, juce::Slider& slider) override
    {
        auto slider_value = slider.getTextFromValue(slider.getValue());
        // Fill the background.
        g.setColour(slider.findColour(Slider::backgroundColourId));
        g.fillRect(Rectangle<float>(static_cast<float>(width - height / 2), 0.f, static_cast<float>(height), static_cast<float>(height)));

        g.setColour(slider.findColour(Slider::thumbColourId));
        g.drawText(slider_value, Rectangle<float>(0.f, static_cast<float>(height) / 4.f,
                                                  static_cast<float>(width), static_cast<float>(height) / 2.f),
                   Justification::centred);

//        g.setColour (slider.findColour (Slider::trackColourId));
//        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
//                                          : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));
    }
};

}

#endif //MIDIBASSPEDALCHORDS_BARSLIDERLOOKANDFEEL_H
