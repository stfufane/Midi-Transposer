#ifndef MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H

#include "JuceHeader.h"
#include "gui/Configuration.hpp"

namespace Gui::LnF
{

/**
 * @brief String representations of the implemented colors
 * that can be read from a json file.
 */
struct BaseColors {
    std::string mSliderBackground { "ffff0000" };
    std::string mSliderThumb { "ffff0000"};
    std::string mSliderTextColor { "fffff0000"};
    std::string mSliderTrack { "ffff0000"};

    static std::string getFileName() { return "colors.json"; }
};

inline void from_json(const nlohmann::json& j, BaseColors& colors)
{
    try {
        j.at("slider_background").get_to(colors.mSliderBackground);
        j.at("slider_thumb").get_to(colors.mSliderThumb);
        j.at("slider_text_color").get_to(colors.mSliderTextColor);
        j.at("slider_track").get_to(colors.mSliderTrack);
    } catch (std::exception& e) {
        std::cout << "One or several values were not defined in the json configuration file\n" << e.what() << "\n";
    }
}

inline juce::Font getDefaultFont(float inPointHeight = 16.f)
{
    return juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::iosevkaregular_ttf,
                                                   BinaryData::iosevkaregular_ttfSize))
                                                   .withPointHeight(inPointHeight);
}

class BaseLookAndFeel : public juce::LookAndFeel_V4,
        public Gui::Configuration<BaseColors>::Listener<BaseColors>
{
public:
    explicit BaseLookAndFeel(juce::Component* rootComponent);
    ~BaseLookAndFeel() override;

    juce::Rectangle<int> getTooltipBounds(const juce::String& tipText,
                                          juce::Point<int> screenPos,
                                          juce::Rectangle<int> parentArea) override;

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override;

    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point<float>& tip,
                    const juce::Rectangle<float>& body) override;

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    void onConfigChanged(const BaseColors& colors) override;

private:
    Gui::Configuration<BaseColors> mConfiguration;

    void resetColors();
};

}
#endif //MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
