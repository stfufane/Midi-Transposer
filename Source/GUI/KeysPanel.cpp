#include "Panels.h"

KeysPanel::KeysPanel(NoteParams& noteParams)
{
    for (int i = 0; i < NB_NOTES; i++)
    {
        auto noteKey = new NoteKey(i, whiteNotes[i] ? &whiteKey : &blackKey);
        noteKeys.add(noteKey);
        addAndMakeVisible(noteKey);

        auto noteParam = noteParams.notes[i].get();
        auto noteMappingToggle = new NoteMappingToggle(i, *noteParam, &powerImage);
        noteMappingToggles.add(noteMappingToggle);
        addAndMakeVisible(noteMappingToggle);
    }
}

void KeysPanel::resized()
{
    auto xMargin = static_cast<float>(getWidth()) * 0.1208f;
    auto width = (static_cast<float>(getWidth()) - (2 * xMargin)) / (NB_NOTES + 1);
    auto height = getLocalBounds().getHeight();
    for (auto i = 0; i < noteKeys.size(); i++)
    {
        auto x = (i < 5 ? static_cast<float>(i) : static_cast<float>(i + 1));
        auto key_x = static_cast<int>(xMargin + width * x);
        auto key_width = static_cast<int>(width);
        noteKeys[i]->setBounds(key_x, 0, key_width, height);
        noteMappingToggles[i]->setBounds(key_x, 0, key_width, key_width);
    }
}