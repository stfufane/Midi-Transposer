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
    auto xMargin = static_cast<int>(static_cast<float>(getWidth()) * 0.1208f);
    auto width = (getWidth() - (2 * xMargin)) / (NB_NOTES + 1);
    auto height = getLocalBounds().getHeight();
    for (auto i = 0; i < noteKeys.size(); i++)
    {
        auto x = (i < 5 ? i : i + 1);
        noteKeys[i]->setBounds(xMargin + width * x, 0, width, height);
        noteMappingToggles[i]->setBounds(xMargin + width * x, 0, width, width);
    }
}