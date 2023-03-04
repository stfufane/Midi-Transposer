#ifndef MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H

#include "JuceHeader.h"
#include "gui/Configuration.h"

namespace Gui::LnF
{

class BaseLookAndFeel : public juce::LookAndFeel_V4, public Gui::Configuration::Listener
{
public:
    explicit BaseLookAndFeel(Gui::Configuration* config);

    juce::Rectangle<int> getTooltipBounds(const juce::String& tipText,
                                          juce::Point<int> screenPos,
                                          juce::Rectangle<int> parentArea) override;

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override;

    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point<float>& tip,
                    const juce::Rectangle<float>& body) override;

    void drawLinearSlider(juce::Graphics &, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider &) override;

    void onColorsChanged(const nlohmann::json& colors) override;

private:
    void resetColors(const nlohmann::json& colors);
};

}
#endif //MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
