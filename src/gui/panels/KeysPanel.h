#ifndef MIDIBASSPEDALCHORDS_KEYSPANEL_H
#define MIDIBASSPEDALCHORDS_KEYSPANEL_H

#include "JuceHeader.h"
#include "params/Params.h"
#include "gui/widgets/NoteKey.h"
#include "gui/widgets/NoteMappingToggle.h"

namespace Gui
{
/**
 * @brief The header contains all the note names + their transposition sliders.
 */
class KeysPanel : public juce::Component
{
public:
    KeysPanel() = delete;
    explicit KeysPanel(Params::NoteParams& noteParams);

    void resized() override;

    std::vector<std::unique_ptr<Gui::NoteKey>>& getNoteKeys() { return noteKeys; }
    void setNoteKeyEdited(int index);
private:
    std::vector<std::unique_ptr<Gui::NoteKey>> noteKeys;
    std::vector<std::unique_ptr<Gui::NoteMappingToggle>> noteMappingToggles;

    juce::Image powerImage { juce::ImageCache::getFromMemory(BinaryData::power_button_png, BinaryData::power_button_pngSize) };

    static constexpr auto kNbNotes = 12;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeysPanel)
};
}

#endif //MIDIBASSPEDALCHORDS_KEYSPANEL_H
