#ifndef MIDITRANSPOSER_CUSTOMTOGGLEBUTTON_H
#define MIDITRANSPOSER_CUSTOMTOGGLEBUTTON_H

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

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

#endif //MIDITRANSPOSER_CUSTOMTOGGLEBUTTON_H