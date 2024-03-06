#ifndef MIDITRANSPOSER_BASELOOKANDFEEL_H
#define MIDITRANSPOSER_BASELOOKANDFEEL_H

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"
#include "gui/Configuration.hpp"
#include "gui/CompColors.h"

namespace Gui::LnF
{

inline juce::Font getDefaultFont(float inPointHeight = 16.f)
{
    return juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::iosevkaregular_ttf,
                                                   BinaryData::iosevkaregular_ttfSize))
                                                   .withPointHeight(inPointHeight);
}

class BaseLookAndFeel : public juce::LookAndFeel_V4,
        public Gui::Configuration<CompColors>::Listener<CompColors>
{
public:
    explicit BaseLookAndFeel(juce::Component* rootComponent);
    ~BaseLookAndFeel() override;

    juce::Rectangle<int> getTooltipBounds(const juce::String& tipText,
                                          juce::Point<int> screenPos,
                                          juce::Rectangle<int> parentArea) override;

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override;

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;

    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override;

    void onConfigChanged(const CompColors& colors) override;

    void drawPopupMenuBackground (juce::Graphics& g, int width, int height) override;

    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                 const bool isSeparator, const bool isActive,
                                 const bool isHighlighted, const bool isTicked,
                                 const bool hasSubMenu, const juce::String& text,
                                 const juce::String& shortcutKeyText,
                                 const juce::Drawable* icon, const juce::Colour* const textColourToUse) override;

private:
    Gui::Configuration<CompColors> mConfiguration;

    void resetColors();
};

}
#endif //MIDITRANSPOSER_BASELOOKANDFEEL_H
