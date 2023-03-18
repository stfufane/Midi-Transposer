#include "TextSwitch.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"

namespace Gui
{
    TextSwitch::TextSwitch(const juce::String& name, juce::String inOnText, juce::String inOffText)
        : Gui::CustomToggleButton(name), onText(std::move(inOnText)), offText(std::move(inOffText))
    { }

    void TextSwitch::paint(juce::Graphics& g)
    {
        if (auto* main_panel = findParentComponentOfClass<Gui::MainPanel>(); main_panel) {
            const auto& coordinates = main_panel->getCoordinates();
            
            const auto toggle_margin = static_cast<int>(coordinates.mToggleMargin);
            const auto margin = static_cast<int>(coordinates.mMargin);

            const auto total_bounds = getLocalBounds().reduced(toggle_margin, 0);

            const auto toggle_left = total_bounds.withWidth(total_bounds.getWidth() / 2).reduced(margin);
            const auto toggle_right = toggle_left.translated(total_bounds.getWidth() / 2, 0);

            const auto bg_color_on = findColour(juce::Label::ColourIds::textColourId);
            const auto bg_color_off = findColour(juce::Label::ColourIds::backgroundColourId);

            g.setColour(bg_color_off);
            g.fillRoundedRectangle(total_bounds.toFloat(), coordinates.mKeyCorner);
            g.setColour(bg_color_on);
            g.fillRoundedRectangle(getToggleState() ? toggle_left.toFloat() : toggle_right.toFloat(), coordinates.mKeyCorner - 4.f);

            g.setFont(LnF::getDefaultFont(coordinates.mToggleFontSize));
            g.setColour(getToggleState() ? bg_color_off : bg_color_on);
            g.drawText(onText, toggle_left, juce::Justification::centred);
            g.setColour(getToggleState() ? bg_color_on : bg_color_off);
            g.drawText(offText, toggle_right, juce::Justification::centred);
        }
    }

    void TextSwitch::mouseEnter(const juce::MouseEvent&)
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void TextSwitch::mouseExit(const juce::MouseEvent&)
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
    }
}