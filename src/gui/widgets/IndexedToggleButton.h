#ifndef MIDIBASSPEDALCHORDS_INDEXEDTOGGLEBUTTON_H
#define MIDIBASSPEDALCHORDS_INDEXEDTOGGLEBUTTON_H

#include "JuceHeader.h"

namespace Gui
{

/**
* @brief This is a custom toggle button that seeks a specific part of a sprite image to display
* depending on the interval index. It centers it in the local bounds of the component.
*/
class IndexedToggleButton : public juce::ToggleButton
{
public:
    IndexedToggleButton() = default;

    void paintButton(juce::Graphics& g, bool, bool) override;

    void setImage(juce::Image* image, int i, float nb_tiles, bool center = true);

private:
    int index { 0 };
    int buttonWidth { 0 };
    int buttonHeight { 0 };
    float ratio { 1.0f };
    bool centered { true };
    juce::Image* buttonsImage = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IndexedToggleButton)
};

} // Gui

#endif //MIDIBASSPEDALCHORDS_INDEXEDTOGGLEBUTTON_H
