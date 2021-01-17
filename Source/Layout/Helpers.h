#pragma once

#include <JuceHeader.h>

// Each GUI component linked to a parameter is created as an AttachedComponent to manage the attachment in the same object.
// It's automatically made visible when it's initialised. The init parameter function can be used to define the component properties.
template <typename CompType, typename CompAttachment>
class AttachedComponent
{
public:
    AttachedComponent<CompType, CompAttachment>(RangedAudioParameter& param, Component& parent, std::function<void(CompType&)> init = nullptr)
    {
        if (init != nullptr) init(component);
        parent.addAndMakeVisible(component);
        attachment.reset(new CompAttachment(param, component));
        attachment->sendInitialUpdate();
    }
    CompType component;
private:
    std::unique_ptr<CompAttachment> attachment;
};

/**
 *  Utility class to instanciate sliders with specific style and positions directly.
 */
struct RotarySlider : public Slider
{
    RotarySlider() 
        : Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag , Slider::TextEntryBoxPosition::NoTextBox) { };
};

/**
 * This is a custom toggle button that seeks a specific part of a sprite image to display
 * depending on the interval index. It centers it in the local bounds of the component.
 */
class IndexedToggleButton : public ToggleButton
{
public:
    void paintButton (Graphics& g, bool, bool) override
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
    
    void setImage(Image* image, int i, float nb_tiles, bool center = true)
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
    Image* buttonsImage = nullptr;
};
