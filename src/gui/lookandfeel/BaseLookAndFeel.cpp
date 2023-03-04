#include "BaseLookAndFeel.h"

namespace Gui::LnF
{

BaseLookAndFeel::BaseLookAndFeel(Gui::Configuration* config)
    : Gui::Configuration::Listener(config)
{
    resetColors(config->getColors());
}

void BaseLookAndFeel::onColorsChanged(const nlohmann::json& colors)
{
    resetColors(colors);
}

void BaseLookAndFeel::resetColors(const nlohmann::json& colors) {
    // Slider colors
    setColour(juce::Slider::backgroundColourId, juce::Colour::fromString(
            colors.value("slider_background", "ffa5a5a5")));
    setColour(juce::Slider::thumbColourId, juce::Colour::fromString(
            colors.value("slider_thumb", "ff00bfff")));
    setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::fromString(
            colors.value("slider_text_colour", "ff000000")));
    setColour(juce::Slider::trackColourId, juce::Colour::fromString(
            colors.value("slider_track", "0f00008b")));

    // Label colors
    setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);

    // Tooltip colors
    setColour(juce::TooltipWindow::backgroundColourId, juce::Colours::whitesmoke);
    setColour(juce::TooltipWindow::textColourId, juce::Colours::black);
    setColour(juce::TooltipWindow::outlineColourId, juce::Colours::darkblue);

    setColour(juce::BubbleComponent::outlineColourId, juce::Colours::darkblue);
    setColour(juce::BubbleComponent::backgroundColourId, juce::Colours::whitesmoke);

    // ToggleButton colors
    setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
    setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);
    setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::darkgrey);
}


juce::Rectangle<int> BaseLookAndFeel::getTooltipBounds(const juce::String& /* tipText */,
                                      juce::Point<int> /* screenPos */,
                                      juce::Rectangle<int> parentArea)
{
    // Return the global bounds of the tooltips, at the bottom of the plugin area.
    return juce::Rectangle<int>(0, 0, parentArea.getWidth(), parentArea.getHeight()).reduced(1);
}

void BaseLookAndFeel::drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height)
{
    juce::Rectangle<int> bounds(width, height);
    g.setColour(juce::LookAndFeel::findColour(juce::TooltipWindow::backgroundColourId));
    g.fillRect(bounds.toFloat());

    g.setColour (findColour (juce::TooltipWindow::ColourIds::outlineColourId));
    g.drawRect(bounds.toFloat().reduced (0.5f, 0.5f), 1.0f);

    juce::AttributedString s;
    s.setJustification(juce::Justification::centredLeft);
    s.append(text, juce::Font(14.0f, juce::Font::plain), findColour(juce::TooltipWindow::textColourId));

    juce::TextLayout tl;
    tl.createLayoutWithBalancedLineLengths(s, (float) width);
    tl.draw(g, { static_cast<float> (width), static_cast<float> (height) });
}

void BaseLookAndFeel::drawBubble(juce::Graphics& g, juce::BubbleComponent& /* bubble */, const juce::Point<float>& /* tip */,
                    const juce::Rectangle<float>& body)
{
    // Only draw a very basic rectangle, without the tip
    juce::Path p;

    p.addRectangle(body);

    g.setColour(findColour(juce::BubbleComponent::backgroundColourId));
    g.fillPath(p);

    g.setColour(findColour(juce::BubbleComponent::outlineColourId));
    g.strokePath(p, juce::PathStrokeType(1.0f));
}


/**
 * @brief Draw a simple rectangular slider with a square thumb
 */
void BaseLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       float /* minSliderPos */, float /* maxSliderPos */, const juce::Slider::SliderStyle,
                                       juce::Slider& slider)
{
    auto trackWidth = juce::jmin (12.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);

    juce::Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                             slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

    juce::Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                           slider.isHorizontal() ? startPoint.y : (float) y);

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath (startPoint);
    backgroundTrack.lineTo (endPoint);
    g.setColour (slider.findColour (juce::Slider::backgroundColourId));
    g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::mitered, juce::PathStrokeType::square });

    juce::Path valueTrack;
    juce::Point<float> minPoint, maxPoint;

    auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
    auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

    minPoint = startPoint;
    maxPoint = { kx, ky };

    auto thumbWidth = trackWidth * 1.3f; // getSliderThumbRadius (slider);

    valueTrack.startNewSubPath (minPoint);
    valueTrack.lineTo (maxPoint);
    g.setColour (slider.findColour (juce::Slider::trackColourId));
    g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::mitered, juce::PathStrokeType::square });

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect(juce::Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (maxPoint));
}

}