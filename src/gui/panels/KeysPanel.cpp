#include "KeysPanel.h"

namespace Gui
{

KeysPanel::KeysPanel(Params::NoteParams& noteParams)
    : juce::Component("Keys Panel")
{
    noteKeys.reserve(kNbNotes);
    noteMappingToggles.reserve(kNbNotes);

    for (size_t i = 0; i < kNbNotes; i++) {
        noteKeys.emplace_back(new Gui::NoteKey(static_cast<int>(i), whiteNotes[i] ? &whiteKey : &blackKey));
        addAndMakeVisible(noteKeys.back().get());

        auto* noteParam = noteParams.notes[i].get();
        noteMappingToggles.emplace_back(new Gui::NoteMappingToggle(*noteParam, &powerImage));
        // addAndMakeVisible(noteMappingToggles.back().get()); //TODO: move somewhere else later.
    }
}

void KeysPanel::setNoteKeyEdited(const int index)
{
    for (auto& noteKey: noteKeys) {
        noteKey->setEdited(index);
    }
}

void KeysPanel::resized()
{
    // Keys are drawn on 2 lines, white keys at the bottom and black keys at the top.
    auto line_height = static_cast<float>(getLocalBounds().getHeight()) / 2.f;
    // Keys are square and have a small margin
    auto keys_side = line_height * 0.9f;
    auto keys_margin = line_height * 0.1f;
    auto white_keys_side_margin = (static_cast<float>(getLocalBounds().getWidth()) - (7.f * keys_side + 6.f * keys_margin)) / 2.f;
    auto black_keys_side_margin = white_keys_side_margin + line_height / 2.f;
    auto white_key = 0, black_key = 0;
    for (size_t i = 0; i < noteKeys.size(); i++) {
        float x, y;
        if (whiteNotes[i]) {
            x = white_keys_side_margin + static_cast<float>(white_key) * (keys_side + keys_margin);
            y = line_height;
            white_key++;
        } else {
            // Leave an extra space between E and F.
            if (black_key ==  2) {
                black_key++;
            }
            x = black_keys_side_margin + static_cast<float>(black_key) * (keys_side + keys_margin);
            y = 0;
            black_key++;
        }
        noteKeys[i]->setBounds(static_cast<int>(x), static_cast<int>(y),
                               static_cast<int>(keys_side), static_cast<int>(keys_side));
    }
}

}