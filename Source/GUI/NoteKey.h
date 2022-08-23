#ifndef MIDIBASSPEDALCHORDS_NOTEKEY_H
#define MIDIBASSPEDALCHORDS_NOTEKEY_H

#include <JuceHeader.h>

namespace Gui
{

/**
 * @brief The drawing of a key with an event to display its intervals.
 */
struct NoteKey : public juce::Component
{
    NoteKey() = delete;
    explicit NoteKey(int index, juce::Image* image);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;

    int noteIndex;
    bool isEdited = false;
    bool isOver = false;
    juce::Image* keyImage = nullptr;
    std::function<void(int index)> changeNote = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteKey)
};

}

#endif //MIDIBASSPEDALCHORDS_NOTEKEY_H
