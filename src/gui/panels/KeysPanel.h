#ifndef MIDITRANSPOSER_KEYSPANEL_H
#define MIDITRANSPOSER_KEYSPANEL_H

#include "params/Params.h"
#include "gui/widgets/NoteKey.h"

namespace Gui
{
/**
 * @brief The header contains all the note names + their transposition sliders.
 */
class KeysPanel : public juce::Component
{
public:
    KeysPanel();

    void resized() override;

    std::vector<std::unique_ptr<NoteKey>>& getNoteKeys() { return noteKeys; }
    void setNoteKeyEdited(int index);
    void setNotePlayed(int index);
private:
    std::vector<std::unique_ptr<NoteKey>> noteKeys;

    static constexpr auto kNbNotes = 12;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeysPanel)
};
}

#endif //MIDITRANSPOSER_KEYSPANEL_H
