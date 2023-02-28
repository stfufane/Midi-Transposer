#ifndef MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
#define MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H

#include <JuceHeader.h>

namespace Gui
{

class BaseLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BaseLookAndFeel()
    {
        // Slider colors
        setColour(juce::Slider::backgroundColourId, juce::Colours::whitesmoke);
        setColour(juce::Slider::thumbColourId, juce::Colours::deepskyblue);
        setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
        setColour(juce::Slider::trackColourId, juce::Colours::darkblue.withAlpha(0.5f));

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

    juce::Rectangle<int>
    getTooltipBounds(const juce::String& tipText, juce::Point<int> screenPos, juce::Rectangle<int> parentArea) override
    {
        return juce::Rectangle<int>(0, 0, parentArea.getWidth(), parentArea.getHeight()).reduced(1);
    }

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override
    {
        juce::Rectangle<int> bounds(width, height);
        g.setColour(findColour(juce::TooltipWindow::backgroundColourId));
        g.fillRect(bounds.toFloat());

//        g.setColour (findColour (TooltipWindow::outlineColourId));
//        g.drawRect(bounds.toFloat().reduced (0.5f, 0.5f), 1.0f);

//        const float tooltipFontSize = 14.0f;
//        const int maxToolTipWidth = 400;

        juce::AttributedString s;
        s.setJustification(juce::Justification::centredLeft);
        s.append(text, juce::Font(14.0f, juce::Font::plain), findColour(juce::TooltipWindow::textColourId));

        juce::TextLayout tl;
        tl.createLayoutWithBalancedLineLengths(s, (float) width);
        tl.draw(g, { static_cast<float> (width), static_cast<float> (height) });
    }

    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point<float>& tip,
                    const juce::Rectangle<float>& body) override
    {
        juce::Path p;
//        p.addBubble (body.reduced (0.5f), body.getUnion (Rectangle<float> (tip.x, tip.y, 1.0f, 1.0f)),
//                     tip, 5.0f, jmin (15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));
        p.addRectangle(body);

        g.setColour(findColour(juce::BubbleComponent::backgroundColourId));
        g.fillPath(p);

        g.setColour(findColour(juce::BubbleComponent::outlineColourId));
        g.strokePath(p, juce::PathStrokeType(1.0f));
    }
};

}

#endif //MIDIBASSPEDALCHORDS_BASELOOKANDFEEL_H
