#ifndef MIDIBASSPEDALCHORDS_INTERVALSPANEL_H
#define MIDIBASSPEDALCHORDS_INTERVALSPANEL_H

#include "JuceHeader.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"
#include "gui/widgets/IndexedToggleButton.h"
#include "gui/lookandfeel/BarSliderLookAndFeel.h"

namespace Gui
{

/**
 * @brief This panel holds the 12 interval buttons for a note + a semitone transposition slider.
 */
class IntervalsPanel : public juce::Component
{
public:
    IntervalsPanel() = delete;
    explicit IntervalsPanel(Params::NoteParam& noteParam);

    ~IntervalsPanel() override;

    void resized() override;

private:
    LnF::BarSliderLookAndFeel mLookAndFeel;
    juce::Image buttonsImage { juce::ImageCache::getFromMemory(BinaryData::buttons_png, BinaryData::buttons_pngSize) };

    std::unique_ptr<AttachedComponent<CustomSlider, juce::SliderParameterAttachment>> transpose;
    std::vector<std::unique_ptr<AttachedComponent<Gui::IndexedToggleButton, juce::ButtonParameterAttachment>>> intervalsChoices;

    static constexpr int kNbIntervals { 12 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntervalsPanel)
};

}

#endif //MIDIBASSPEDALCHORDS_INTERVALSPANEL_H