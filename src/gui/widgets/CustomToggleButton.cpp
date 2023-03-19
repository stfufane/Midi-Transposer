#include "CustomToggleButton.h"

namespace Gui
{
    CustomToggleButton::CustomToggleButton(const juce::String &name)
        : juce::ToggleButton(name)
    { }

    juce::String CustomToggleButton::getTooltip()
    {
        if (mCustomTooltipLambda != nullptr) {
            return mCustomTooltipLambda();
        }
        return juce::ToggleButton::getTooltip();
    }

    void CustomToggleButton::mouseEnter(const juce::MouseEvent&)
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void CustomToggleButton::mouseExit(const juce::MouseEvent&)
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
    }

    void CustomToggleButton::setCustomTooltipLambda(std::function<juce::String()> inLambda)
    {
        mCustomTooltipLambda = std::move(inLambda);
    }
}