#pragma once

#include <JuceHeader.h>
#include "../Params/Params.h"

/**
 * @brief Each GUI component linked to a parameter is created as an AttachedComponent to manage the attachment in the same object.
 * It's automatically made visible when it's initialised. The init parameter function can be used to define the component properties.
 * @tparam CompType The type of component to display (slider, button, etc.)
 * @tparam CompAttachment The ParameterAttachment linked to the component.
 */
template <typename CompType, typename CompAttachment>
class AttachedComponent
{
public:
    AttachedComponent<CompType, CompAttachment>(juce::RangedAudioParameter& param, juce::Component& parent, std::function<void(CompType&)> init = nullptr)
    {
        attachment.reset(new CompAttachment(param, component));
        parent.addAndMakeVisible(component);
        if (init != nullptr) init(component);
        attachment->sendInitialUpdate();
    }
    CompType component;
private:
    std::unique_ptr<CompAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttachedComponent)
};

struct HorizontalSlider : public juce::Slider
{
    HorizontalSlider()
        : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight) 
    {
        setTextBoxIsEditable(false);
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HorizontalSlider)
};

struct SyncRateSlider : public HorizontalSlider
{
    SyncRateSlider() : HorizontalSlider() 
    {
        setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    };

    juce::String getTextFromValue(double value) override
    {
        return Notes::divisions[(int) value].label;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SyncRateSlider)
};

struct SemitoneSlider : public HorizontalSlider
{
    SemitoneSlider() : HorizontalSlider() {};

    juce::String getTextFromValue(double value) override
    {
        return juce::String(value) + " semitone" + (std::abs(value) > 1 ? "s" : "");
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SemitoneSlider)
};

struct OctaveSlider : public HorizontalSlider
{
    OctaveSlider() : HorizontalSlider() {};

    juce::String getTextFromValue(double value) override
    {
        return juce::String(value) + " octave" + (std::abs(value) > 1 ? "s" : "");
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OctaveSlider)
};

/**
 * @brief This is a custom toggle button that seeks a specific part of a sprite image to display
 * depending on the interval index. It centers it in the local bounds of the component.
 */
class IndexedToggleButton : public juce::ToggleButton
{
public:
    IndexedToggleButton() : juce::ToggleButton() { };
    void paintButton (juce::Graphics& g, bool, bool) override
    {
        // Calculations needed to draw the button centered in its container,
        // maximizing its bigger side.
        auto y_index = getToggleState() ? 1 : 0;
        auto bounds = getLocalBounds();
        
        float side, displayWidth, displayHeight, top_x, top_y;
        side = jmin(bounds.getWidth(), bounds.getHeight());
        if (buttonWidth >= buttonHeight) {
            displayWidth = side;
            displayHeight = displayWidth / ratio;
            top_x = centered ? (bounds.getWidth()  - displayWidth) / 2.0f : 0;
            top_y = centered ? (bounds.getHeight() - displayHeight) / 2.0f : 0;
        } else {
            displayHeight = side;
            displayWidth  = displayHeight * ratio;
            top_x = centered ? (bounds.getWidth()  - displayWidth) / 2.0f : 0;
            top_y = centered ? (bounds.getHeight() - displayHeight) / 2.0f : 0;
        }

        g.drawImage(*buttonsImage, top_x, top_y, displayWidth, displayHeight, 
            buttonWidth * index, buttonHeight * y_index, buttonWidth, buttonHeight);
    }
    
    void setImage(juce::Image* image, int i, float nb_tiles, bool center = true)
    {
        index = i;
        buttonsImage = image;
        buttonWidth = buttonsImage->getWidth() / nb_tiles;
        buttonHeight = buttonsImage->getHeight() / 2.0f;
        ratio = buttonWidth / buttonHeight;
        centered = center;
    }
private:
    int index { 0 };
    float buttonWidth { 0.0f };
    float buttonHeight { 0.0f };
    float ratio { 1.0f };
    bool centered { true };
    juce::Image* buttonsImage = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IndexedToggleButton)
};
