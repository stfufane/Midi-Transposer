#include "CustomSlider.h"

namespace Gui
{
    CustomSlider::CustomSlider(const juce::String& name,
                               const SliderStyle inStyle,
                               const TextEntryBoxPosition inPosition)
        : juce::Slider(name)
    {
        // Init default slider form.
        setSliderStyle(inStyle);
        setTextBoxStyle(inPosition, true, getWidth(), getWidth());
    }

    void CustomSlider::paint(juce::Graphics& g)
    {
        // Draw the regular slider in every case.
        juce::Slider::paint(g);
        // Potentially paint something over it.
        if (mCustomPaintLambda != nullptr) {
            mCustomPaintLambda(g);
        }
    }

    juce::String CustomSlider::getTextFromValue(double value)
    {
        if (mCustomTextLambda != nullptr) {
            return mCustomTextLambda(value);
        }
        return juce::Slider::getTextFromValue(value);
    }

    void CustomSlider::setCustomTextLambda(std::function<juce::String(double)> inLambda)
    {
        mCustomTextLambda = std::move(inLambda);
    }

    void CustomSlider::setCustomPaintLambda(std::function<void(juce::Graphics&)> inLambda)
    {
        mCustomPaintLambda = std::move(inLambda);
    }
} // Gui