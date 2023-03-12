#include "BaseLookAndFeel.h"

namespace Gui::LnF
{

BaseLookAndFeel::BaseLookAndFeel(juce::Component* rootComponent)
    : mConfiguration(CONFIG_FOLDER, rootComponent)
{
    mConfiguration.addListener(this);
    resetColors();
}

BaseLookAndFeel::~BaseLookAndFeel()
{
    mConfiguration.removeListener(this);
}

void BaseLookAndFeel::onConfigChanged(const BaseColors&)
{
    resetColors();
}

void BaseLookAndFeel::resetColors() {
    const auto& colors = mConfiguration.getData();
    // Slider colors
    setColour(juce::Slider::backgroundColourId, juce::Colour::fromString(colors.mSliderBackground));
    setColour(juce::Slider::thumbColourId, juce::Colour::fromString(colors.mSliderThumb));
    setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::fromString(colors.mSliderText));
    setColour(juce::Slider::trackColourId, juce::Colour::fromString(colors.mSliderTrack));

    // Label colors
    setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromString(colors.mLabelText));
    setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour::fromString(colors.mLabelBackground));

    // Tooltip colors
    setColour(juce::TooltipWindow::backgroundColourId, juce::Colours::whitesmoke);
    setColour(juce::TooltipWindow::textColourId, juce::Colours::black);

    setColour(juce::BubbleComponent::outlineColourId, juce::Colour::fromString(colors.mLabelBackground));
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
    return juce::Rectangle<int>(0, 0, parentArea.getWidth(), parentArea.getHeight()).reduced(3, 1);
}

void BaseLookAndFeel::drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height)
{
    juce::Rectangle<int> bounds(width, height);
    // Fill the background
    g.setColour(findColour(juce::TooltipWindow::backgroundColourId));
    g.fillRect(bounds.toFloat());

    // Draw the text
    g.setColour(findColour(juce::Label::backgroundColourId));
    g.setFont(getDefaultFont());
    g.drawText(text, bounds, juce::Justification::centredLeft);
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

void BaseLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                       float sliderPos, float rotaryStartAngle,
                       float rotaryEndAngle, juce::Slider& slider)
{
    auto outline = slider.findColour (juce::Slider::backgroundColourId);
    auto fill    = slider.findColour (juce::Slider::trackColourId);

    auto bounds = juce::Rectangle<int> ((width - height) / 2, y, // TODO: calculate properly
                                        juce::jmin(width, height), juce::jmin(width, height)).toFloat().reduced (5);

    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = 2.0f;
    auto arcRadius = radius - lineW * 0.5f + 2.0f;

    g.setColour (outline);
    g.fillEllipse(bounds.reduced(3.0f));

    if (slider.isEnabled())
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);

        g.setColour (fill);
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::mitered, juce::PathStrokeType::square));
    }

    auto thumbWidth = lineW * 2.f;
    juce::Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect(juce::Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (thumbPoint));
}

}