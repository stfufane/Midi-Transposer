#ifndef MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H
#define MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H

#include <JuceHeader.h>
#include "Helpers.h"

namespace Gui
{

/**
 * @brief This is the power on/off button on top of each note.
 */
class NoteMappingToggle : public juce::Component
{
public:
    NoteMappingToggle() = delete;
    explicit NoteMappingToggle(NoteParam& noteParam, juce::Image* image);
    void resized() override;

private:
    juce::Image* powerImage = nullptr;
    std::unique_ptr<AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>> mapChoice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteMappingToggle)
};

}

#endif //MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H
