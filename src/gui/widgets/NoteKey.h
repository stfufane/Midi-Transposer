#ifndef MIDIBASSPEDALCHORDS_NOTEKEY_H
#define MIDIBASSPEDALCHORDS_NOTEKEY_H

#include "JuceHeader.h"

namespace Gui
{

/**
 * @brief The drawing of a key with an event to display its intervals.
 */
class NoteKey : public juce::Component
{
public:
    NoteKey() = delete;
    explicit NoteKey(int index, juce::Image* image);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;

    void setEdited(const int index) { isEdited = (noteIndex == index); }
    void setChangeNoteCallback(std::function<void(int)> inCallback) { changeNote = std::move(inCallback); }

private:
    int noteIndex;
    bool isEdited = false;
    bool isOver = false;
    juce::Image* keyImage = nullptr;
    std::function<void(int index)> changeNote = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteKey)
};

}

#endif //MIDIBASSPEDALCHORDS_NOTEKEY_H
