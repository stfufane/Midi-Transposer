#ifndef MIDIBASSPEDALCHORDS_INTERVALSPANEL_H
#define MIDIBASSPEDALCHORDS_INTERVALSPANEL_H

#include <JuceHeader.h>
#include "Helpers.h"

namespace Gui
{

/**
 * @brief This panel holds the 12 interval buttons for a note + a semitone transposition slider.
 */
struct IntervalsPanel : public juce::Component
{
    IntervalsPanel() = delete;
    explicit IntervalsPanel(NoteParam& noteParam);

    void resized() override;
    void paint(juce::Graphics &g) override;

    juce::Image buttonsImage { juce::ImageCache::getFromMemory(BinaryData::buttons_png, BinaryData::buttons_pngSize) };

    std::unique_ptr<AttachedComponent<SemitoneSlider, juce::SliderParameterAttachment>> transpose;
    std::vector<std::unique_ptr<AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>>> intervalsChoices;

    static constexpr int kNbIntervals { 12 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntervalsPanel)
};

}

#endif //MIDIBASSPEDALCHORDS_INTERVALSPANEL_H
