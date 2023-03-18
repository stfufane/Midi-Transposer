#ifndef MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H

#include "JuceHeader.h"
#include "gui/Configuration.hpp"
#include "gui/CompColors.h"

namespace Gui::LnF
{

inline juce::Font getDefaultFont(float inPointHeight = 16.f)
{
    return juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::iosevkaregular_ttf,
                                                   BinaryData::iosevkaregular_ttfSize))
                                                   .withPointHeight(inPointHeight);
}

class BaseLookAndFeel : public juce::LookAndFeel_V4,
        public Gui::Configuration<CompColors>::Listener<CompColors>
{
public:
    explicit BaseLookAndFeel(juce::Component* rootComponent);
    ~BaseLookAndFeel() override;

    juce::Rectangle<int> getTooltipBounds(const juce::String& tipText,
                                          juce::Point<int> screenPos,
                                          juce::Rectangle<int> parentArea) override;

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override;

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    void onConfigChanged(const BaseColors& colors) override;

private:
    Gui::Configuration<CompColors> mConfiguration;

    void resetColors();
};

}
#endif //MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
