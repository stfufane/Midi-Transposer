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
    auto width = getLocalBounds().getWidth() / float(NB_NOTES);
    auto height = getLocalBounds().getHeight();
    for (auto& noteHeader: noteKeys)
        noteHeader->setBounds(width * noteHeader->noteIndex, 0, width, height);
    for (auto& noteMappingToggle: noteMappingToggles)
        noteMappingToggle->setBounds(width * noteMappingToggle->noteIndex, 0, width, width);
}