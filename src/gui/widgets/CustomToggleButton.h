#ifndef MIDIBASSPEDALCHORDS_CUSTOMTOGGLEBUTTON_H
#define MIDIBASSPEDALCHORDS_CUSTOMTOGGLEBUTTON_H

#include "JuceHeader.h"

namespace Gui
{

class CustomToggleButton : public juce::ToggleButton
{
public:
    explicit CustomToggleButton(const juce::String& name);

    juce::String getTooltip() override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;

    void setCustomTooltipLambda(std::function<juce::String()> inLambda);

private:
    std::function<juce::String()> mCustomTooltipLambda = nullptr;
};

} // Gui

#endif //MIDIBASSPEDALCHORDS_CUSTOMTOGGLEBUTTON_H