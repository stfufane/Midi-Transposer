#ifndef MIDIBASSPEDALCHORDS_CUSTOMSLIDER_H
#define MIDIBASSPEDALCHORDS_CUSTOMSLIDER_H

#include "JuceHeader.h"
#include "params/Params.h"

namespace Gui
{

class CustomSlider : public juce::Slider
{
public:
    explicit CustomSlider(const juce::String& name,
                 juce::Slider::SliderStyle style = juce::Slider::SliderStyle::Rotary,
                 juce::Slider::TextEntryBoxPosition position = juce::Slider::TextEntryBoxPosition::NoTextBox);

    void setCustomTextLambda(std::function<juce::String(double value)> inLambda);

    void setCustomPaintLambda(std::function<void(juce::Graphics& g)> inLambda);

    void paint(juce::Graphics& g) override;

protected:
    juce::String getTextFromValue(double value) override;

private:
    std::function<juce::String(double value)> mCustomTextLambda = nullptr;

    std::function<void(juce::Graphics& g)> mCustomPaintLambda = nullptr;
};

} // Gui

#endif //MIDIBASSPEDALCHORDS_CUSTOMSLIDER_H
