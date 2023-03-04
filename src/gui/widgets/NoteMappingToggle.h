#ifndef MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H
#define MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H

#include "JuceHeader.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/IndexedToggleButton.h"
#include "params/Params.h"

namespace Gui
{

/**
 * @brief This is the power on/off button on top of each note.
 */
class NoteMappingToggle : public juce::Component
{
public:
    NoteMappingToggle() = delete;
    explicit NoteMappingToggle(Params::NoteParam& noteParam, juce::Image* image);
    void resized() override;

private:
    juce::Image* powerImage = nullptr;
    std::unique_ptr<AttachedComponent<Gui::IndexedToggleButton, juce::ButtonParameterAttachment>> mapChoice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteMappingToggle)
};

}

#endif //MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H
