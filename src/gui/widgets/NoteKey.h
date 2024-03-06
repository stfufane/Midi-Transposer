#ifndef MIDITRANSPOSER_NOTEKEY_H
#define MIDITRANSPOSER_NOTEKEY_H

#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui
{

/**
 * @brief The drawing of a key with an event to display its intervals.
 */
class NoteKey : public juce::TextButton
{
public:
    NoteKey() = delete;
    explicit NoteKey(int index);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;

    void setEdited(const int index) { isEdited = (noteIndex == index); }
    void setPlayed(const int index);
    void setChangeNoteCallback(std::function<void(int)> inCallback) { changeNote = std::move(inCallback); }

private:
    int noteIndex;
    bool isEdited   = false;
    bool isOver     = false;
    bool isPlayed   = false;
    std::function<void(int index)> changeNote = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteKey)
};

}

#endif //MIDITRANSPOSER_NOTEKEY_H
