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

void BaseLookAndFeel::onConfigChanged(const CompColors&)
{
    resetColors();
}

void BaseLookAndFeel::resetColors() {
    const auto& colors = mConfiguration.getData();
    // Slider colors
    setColour(juce::Slider::backgroundColourId, juce::Colour::fromString(colors.mSliderBackground));
    setColour(juce::Slider::thumbColourId, juce::Colour::fromString(colors.mSliderThumb));
    setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromString(colors.mSliderText));
    setColour(juce::Slider::trackColourId, juce::Colour::fromString(colors.mSliderTrack));
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromString(colors.mSliderOutline));

    // Label colors
    setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromString(colors.mLabelText));
    setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour::fromString(colors.mLabelBackground));

    // Tooltip colors
    setColour(juce::TooltipWindow::backgroundColourId, juce::Colours::whitesmoke);
    setColour(juce::TooltipWindow::textColourId, juce::Colours::black);

    setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromString(colors.mKeyPlayedBackground));
    setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour::fromString(colors.mKeyPlayedText));

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

/**
 * @brief Draw a simple rectangular slider with a square thumb. Handling only vertical because I'm lazy and only use this one.
 */
void BaseLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       float /* minSliderPos */, float /* maxSliderPos */, const juce::Slider::SliderStyle,
                                       juce::Slider& slider)
{
    auto trackWidth = 10.f;
    auto h_ratio = .72f;

    juce::Point<float> startPoint ((float) x + (float) width * 0.5f, (float) (height + y) * h_ratio);
    juce::Point<float> endPoint (startPoint.x, (float) y * h_ratio);

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath (startPoint);
    backgroundTrack.lineTo (endPoint);
    g.setColour (slider.findColour (juce::Slider::backgroundColourId));
    g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::mitered, juce::PathStrokeType::square });

    juce::Path valueTrack;
    juce::Point<float> minPoint, maxPoint;

    auto kx = (float) x + (float) width * 0.5f;
    auto ky = sliderPos * h_ratio;

    minPoint = startPoint;
    maxPoint = { kx, ky };

    valueTrack.startNewSubPath (minPoint);
    valueTrack.lineTo (maxPoint);
    g.setColour (slider.findColour (juce::Slider::backgroundColourId));
    g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::mitered, juce::PathStrokeType::square });

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect(juce::Rectangle<float> (trackWidth * 2.f, trackWidth / 2.f).withCentre (maxPoint));

    const auto value = slider.getValue();
    auto text = (value > 0 ? "+" : "") + juce::String(value);
    auto text_bounds = juce::Rectangle<float>(0.f, static_cast<float>(slider.getHeight()) * h_ratio, 
        static_cast<float>(width), static_cast<float>(slider.getHeight()) * (1.f - h_ratio));

    g.setFont(LnF::getDefaultFont(20.f));
    g.setColour(findColour(juce::Label::ColourIds::backgroundColourId));
    g.drawText(text, text_bounds, juce::Justification::centred);
}

void BaseLookAndFeel::drawRotarySlider (juce::Graphics& g, int /* x */, int y, int width, int height,
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

    // First draw the outline
    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 toAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
    g.strokePath(backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::mitered, juce::PathStrokeType::square));

    // Then the value
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
    
    // The thumb is just a small square
    auto thumbWidth = lineW * 2.f;
    juce::Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect(juce::Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (thumbPoint));
}

void BaseLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool /* isButtonDown */,
                                    int /* buttonX */, int /* buttonY */, int /* buttonW */, int /* buttonH */,
                                    juce::ComboBox& box)
{
    auto cornerSize = 16.f;
    auto bounds = juce::Rectangle<int> (0, 0, width, height).toFloat().reduced (3.f, 0.f);

    g.setColour (box.findColour (juce::Label::backgroundColourId).darker(box.isMouseOver(true) ? .2f : .1f));
    g.fillRoundedRectangle (bounds, cornerSize);
}

void BaseLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setBounds (juce::Rectangle<int>(0, 0, box.getWidth(), box.getHeight()).reduced(3, 1));
    label.setFont (LnF::getDefaultFont(18.f));
    label.setJustificationType(juce::Justification::centred);
}

}