#ifndef MIDITRANSPOSER_INTERVALSPANEL_H
#define MIDITRANSPOSER_INTERVALSPANEL_H

#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"
#include "gui/widgets/TextSwitch.h"

namespace Gui
{

/**
 * @brief This panel holds the 12 interval buttons for a note + a semitone transposition slider.
 */
class IntervalsPanel : public juce::Component
{
public:
    IntervalsPanel() = delete;
    explicit IntervalsPanel(const Params::NoteParam& noteParam);

    void resized() override;

private:
    /**
     * @brief Global transpose knob of the note
     */
    std::unique_ptr<AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment>> transpose;
    /**
     * @brief List of all the possible intervals
     */
    std::vector<std::unique_ptr<AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment>>> intervalsChoices;
    /**
     * @brief Button to toggle the mapping
     */
    std::unique_ptr<AttachedComponent<Gui::TextSwitch, juce::ButtonParameterAttachment>> mapChoice;

    static constexpr int kNbIntervals { 12 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntervalsPanel)
};

}

#endif //MIDITRANSPOSER_INTERVALSPANEL_H
