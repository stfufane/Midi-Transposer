#include "KeysPanel.h"

namespace Gui
{

KeysPanel::KeysPanel(NoteParams& noteParams)
    : juce::Component("Keys Panel")
{
    noteKeys.reserve(kNbNotes);
    noteMappingToggles.reserve(kNbNotes);

    for (int i = 0; i < kNbNotes; i++) {
        noteKeys.emplace_back(new NoteKey(i, whiteNotes[i] ? &whiteKey : &blackKey));
        addAndMakeVisible(noteKeys.back().get());

        auto* noteParam = noteParams.notes[i].get();
        noteMappingToggles.emplace_back(new NoteMappingToggle(i, *noteParam, &powerImage));
        addAndMakeVisible(noteMappingToggles.back().get());
    }
}

void KeysPanel::resized()
{
    auto xMargin = static_cast<float>(getWidth()) * 0.1208f;
    auto width = (static_cast<float>(getWidth()) - (2 * xMargin)) / (kNbNotes + 1);
    auto height = getLocalBounds().getHeight();
    for (auto i = 0; i < noteKeys.size(); i++) {
        auto x = (i < 5 ? static_cast<float>(i) : static_cast<float>(i + 1));
        auto key_x = static_cast<int>(xMargin + width * x);
        auto key_width = static_cast<int>(width);
        noteKeys[i]->setBounds(key_x, 0, key_width, height);
        noteMappingToggles[i]->setBounds(key_x, 0, key_width, key_width);
    }
}

}