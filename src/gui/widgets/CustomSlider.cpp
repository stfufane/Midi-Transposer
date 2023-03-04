#include "CustomSlider.h"

namespace Gui
{
    CustomSlider::CustomSlider(const juce::String& name,
                               juce::Slider::SliderStyle inStyle,
                               juce::Slider::TextEntryBoxPosition inPosition)
        : juce::Slider(name)
    {
        // Init default slider form.
        setSliderStyle(inStyle);
        setTextBoxStyle(inPosition, true, getWidth(), getWidth());
    }

    juce::String CustomSlider::getTextFromValue(double value)
    {
        if (mCustomTextLambda != nullptr) {
            return mCustomTextLambda(value);
        }
        return juce::Slider::getTextFromValue(value);
    }

void CustomSlider::setCustomTextLambda(const std::function<juce::String(double)>& inLambda)
{
    mCustomTextLambda = inLambda;
}
} // Gui