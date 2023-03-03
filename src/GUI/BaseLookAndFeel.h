#ifndef MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H

#include <JuceHeader.h>

#include "nlohmann/json.hpp"

namespace Gui
{

class BaseLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BaseLookAndFeel();

    void resetConfiguration();

    juce::Rectangle<int> getTooltipBounds(const juce::String& tipText,
                                          juce::Point<int> screenPos,
                                          juce::Rectangle<int> parentArea) override;

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override;

    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point<float>& tip,
                    const juce::Rectangle<float>& body) override;

private:
    void readConfiguration();

    void initColors();

    /**
     * @brief Load the basic GUI config from a json file to allow dynamic repaint
     */
    nlohmann::json conf_json;
};

}
#endif //MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
