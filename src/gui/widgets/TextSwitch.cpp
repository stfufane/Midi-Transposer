#include "TextSwitch.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"

namespace Gui
{
    TextSwitch::TextSwitch(const juce::String& name, juce::String inOnText, juce::String inOffText, float inFontSize)
        : Gui::CustomToggleButton(name), onText(std::move(inOnText)), offText(std::move(inOffText)), fontSize(inFontSize)
    { }

    void TextSwitch::paint(juce::Graphics& g)
    {
        if (auto* main_panel = findParentComponentOfClass<Gui::MainPanel>(); main_panel) {
            const auto& coordinates = main_panel->getCoordinates();
            
            const auto total_bounds = getLocalBounds();

            const auto toggle_left = total_bounds.withWidth(total_bounds.getWidth() / 2).reduced(coordinates.mMargin);
            const auto toggle_right = toggle_left.translated(total_bounds.getWidth() / 2, 0);

            const auto bg_color_on = findColour(juce::Label::ColourIds::textColourId);
            const auto bg_color_off = findColour(juce::Label::ColourIds::backgroundColourId);

            g.setColour(bg_color_off);
            g.fillRoundedRectangle(total_bounds.toFloat(), coordinates.mKeyCorner);
            g.setColour(bg_color_on);
            g.fillRoundedRectangle(getToggleState() ? toggle_left.toFloat() : toggle_right.toFloat(), coordinates.mKeyCorner - 4.f);

            g.setFont(LnF::getDefaultFont(fontSize));
            g.setColour(getToggleState() ? bg_color_off : bg_color_on);
            g.drawText(onText, toggle_left, juce::Justification::centred);
            g.setColour(getToggleState() ? bg_color_on : bg_color_off);
            g.drawText(offText, toggle_right, juce::Justification::centred);
        }
    }
}