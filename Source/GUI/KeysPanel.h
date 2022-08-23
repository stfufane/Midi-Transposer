#ifndef MIDIBASSPEDALCHORDS_KEYSPANEL_H
#define MIDIBASSPEDALCHORDS_KEYSPANEL_H

#include <JuceHeader.h>
#include "../Params/Params.h"
#include "NoteKey.h"
#include "NoteMappingToggle.h"

namespace Gui
{
/**
 * @brief The header contains all the note names + their transposition sliders.
 */
struct KeysPanel : public juce::Component
{
    KeysPanel() = delete;
    explicit KeysPanel(NoteParams& noteParams);
    void resized() override;

    std::vector<std::unique_ptr<NoteKey>> noteKeys;
    std::vector<std::unique_ptr<NoteMappingToggle>> noteMappingToggles;
    const std::array<bool, 12> whiteNotes { true, false, true, false, true, true, false, true, false, true, false, true };

    juce::Image whiteKey { juce::ImageCache::getFromMemory(BinaryData::key_white_sprites_png, BinaryData::key_white_sprites_pngSize) };
    juce::Image blackKey { juce::ImageCache::getFromMemory(BinaryData::key_black_sprites_png, BinaryData::key_black_sprites_pngSize) };
    juce::Image powerImage { juce::ImageCache::getFromMemory(BinaryData::power_button_png, BinaryData::power_button_pngSize) };

    static constexpr auto kNbNotes = 12;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeysPanel)
};
}

#endif //MIDIBASSPEDALCHORDS_KEYSPANEL_H
