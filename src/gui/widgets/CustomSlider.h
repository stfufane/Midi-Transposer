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

    void setCustomTextLambda(const std::function<juce::String(double value)>& inLambda);

protected:
    juce::String getTextFromValue(double value) override;

private:
    std::function<juce::String(double value)> mCustomTextLambda = nullptr;
};

} // Gui

#endif //MIDIBASSPEDALCHORDS_CUSTOMSLIDER_H
