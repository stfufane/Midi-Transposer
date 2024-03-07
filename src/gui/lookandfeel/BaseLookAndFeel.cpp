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

    setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::transparentWhite);
}

juce::Rectangle<int> BaseLookAndFeel::getTooltipBounds(const juce::String& /* tipText */,
                                      juce::Point<int> /* screenPos */,
                                      const juce::Rectangle<int> parentArea)
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
    constexpr auto h_ratio = .72f;

    const juce::Point startPoint (static_cast<float>(x) + static_cast<float>(width) * 0.5f, static_cast<float>(height + y) * h_ratio);
    const juce::Point endPoint (startPoint.x, static_cast<float>(y) * h_ratio);

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath (startPoint);
    backgroundTrack.lineTo (endPoint);
    g.setColour (slider.findColour (juce::Slider::backgroundColourId));
    g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::mitered, juce::PathStrokeType::square });

    juce::Path valueTrack;

    const auto kx = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
    const auto ky = sliderPos * h_ratio;

    const juce::Point<float> minPoint = startPoint;
    const juce::Point<float> maxPoint = { kx, ky };

    valueTrack.startNewSubPath (minPoint);
    valueTrack.lineTo (maxPoint);
    g.setColour (slider.findColour (juce::Slider::backgroundColourId));
    g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::mitered, juce::PathStrokeType::square });

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect(juce::Rectangle<float> (trackWidth * 2.f, trackWidth / 2.f).withCentre (maxPoint));

    const auto value = slider.getValue();
    const auto text = (value > 0 ? "+" : "") + juce::String(value);
    const auto text_bounds = juce::Rectangle<float>(0.f, static_cast<float>(slider.getHeight()) * h_ratio,
                                                    static_cast<float>(width), static_cast<float>(slider.getHeight()) * (1.f - h_ratio));

    g.setFont(LnF::getDefaultFont(20.f));
    g.setColour(findColour(juce::Label::ColourIds::backgroundColourId));
    g.drawText(text, text_bounds, juce::Justification::centred);
}

void BaseLookAndFeel::drawRotarySlider (juce::Graphics& g, int /* x */, const int y, const int width, const int height,
                                        const float sliderPos, const float rotaryStartAngle,
                                        const float rotaryEndAngle, juce::Slider& slider)
{
    const auto outline = slider.findColour (juce::Slider::backgroundColourId);
    const auto fill    = slider.findColour (juce::Slider::trackColourId);

    const auto bounds = juce::Rectangle<int> ((width - height) / 2, y, // TODO: calculate properly
                                              juce::jmin(width, height), juce::jmin(width, height)).toFloat().reduced (5);

    const auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    const auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    constexpr auto lineW = 2.0f;
    const auto arcRadius = radius - lineW * 0.5f + 2.0f;

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
    constexpr auto thumbWidth = lineW * 2.f;
    const juce::Point thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                                bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect(juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
}

void BaseLookAndFeel::drawComboBox (juce::Graphics& g, const int width, const int height, bool /* isButtonDown */,
                                    int /* buttonX */, int /* buttonY */, int /* buttonW */, int /* buttonH */,
                                    juce::ComboBox& box)
{
    constexpr auto cornerSize = 16.f;
    const auto bounds = juce::Rectangle<int> (0, 0, width, height).toFloat().reduced (3.f, 0.f);

    g.setColour (box.findColour (juce::Label::backgroundColourId).darker(box.isMouseOver(true) ? .2f : .1f));
    g.fillRoundedRectangle (bounds, cornerSize);
}

void BaseLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setBounds (juce::Rectangle<int>(0, 0, box.getWidth(), box.getHeight()).reduced(3, 1));
    label.setFont (LnF::getDefaultFont(18.f));
    label.setJustificationType(juce::Justification::centred);
}

void BaseLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    g.fillAll(juce::Colours::transparentWhite);

    g.setColour(findColour(juce::Label::backgroundColourId));
    g.fillRect(juce::Rectangle<int>(0, 0, width - 30, height).withCentre(juce::Point<int>(width / 2, height / 2)));
}

void BaseLookAndFeel::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                        const bool /* isSeparator */, const bool isActive,
                                        const bool isHighlighted, const bool /* isTicked */,
                                        const bool /* hasSubMenu */, const juce::String& text,
                                        const juce::String& /* shortcutKeyText */,
                                        const juce::Drawable* /* icon */, const juce::Colour* const /* textColourToUse */)
{
    g.fillAll(juce::Colours::transparentWhite);

    const auto textColour = findColour (juce::Label::textColourId);
    auto r  = area.reduced (1);
    r.reduce (juce::jmin (5, area.getWidth() / 20), 0);

    if (isHighlighted && isActive) {
        g.setColour (findColour (juce::Label::backgroundColourId).brighter(0.2f));
        g.fillRect (r);

        g.setColour (findColour (juce::PopupMenu::highlightedTextColourId));
    } else {
        g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
    }

    auto font = getDefaultFont();
    const auto maxFontHeight = static_cast<float>(r.getHeight()) / 1.3f;
    if (font.getHeight() > maxFontHeight)
        font.setHeight (maxFontHeight);

    g.setFont (font);
    g.drawFittedText (text, r, juce::Justification::centred, 1);

}

}