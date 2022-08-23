#ifndef MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H
#define MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H

#include <JuceHeader.h>
#include "Helpers.h"

namespace Gui
{

/**
 * @brief This is the power on/off button on top of each note.
 */
struct NoteMappingToggle : public juce::Component
{
    NoteMappingToggle() = delete;
    explicit NoteMappingToggle(int index, NoteParam& noteParam, juce::Image* image);
    void resized() override;

    int noteIndex;
    juce::Image* powerImage = nullptr;
    std::unique_ptr<AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>> mapChoice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteMappingToggle)
};

}

#endif //MIDIBASSPEDALCHORDS_NOTEMAPPINGTOGGLE_H
