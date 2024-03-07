#ifndef MIDITRANSPOSER_CUSTOMSLIDER_H
#define MIDITRANSPOSER_CUSTOMSLIDER_H

#include "params/Params.h"

namespace Gui
{

class CustomSlider final : public juce::Slider
{
public:
    explicit CustomSlider(const juce::String& name,
                 SliderStyle inStyle = Rotary,
                 TextEntryBoxPosition inPosition = NoTextBox);

    void setCustomTextLambda(std::function<juce::String(double value)> inLambda);

    void setCustomPaintLambda(std::function<void(juce::Graphics& g)> inLambda);

    void paint(juce::Graphics& g) override;

    juce::String getTextFromValue(double value) override;

private:
    std::function<juce::String(double value)> mCustomTextLambda = nullptr;

    std::function<void(juce::Graphics& g)> mCustomPaintLambda = nullptr;
};

} // Gui

#endif //MIDITRANSPOSER_CUSTOMSLIDER_H
