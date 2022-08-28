#include "NoteKey.h"

namespace Gui
{

NoteKey::NoteKey(int index, juce::Image* image)
        : juce::Component("Note Key " + juce::String(index)), noteIndex(index), keyImage(image)
{}

void NoteKey::paint(juce::Graphics& g)
{
    // The image is in 3 parts
    // First part is the inactive note, second part is the edited note and third part is the hovered note.
    if (isEdited) {
        g.drawImage(*keyImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(),
                    keyImage->getWidth() / 3, 0, keyImage->getWidth() / 3, keyImage->getHeight());
        return;
    }
    if (isOver) {
        g.drawImage(*keyImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(),
                    keyImage->getWidth() * 2 / 3, 0, keyImage->getWidth() / 3, keyImage->getHeight());
        return;
    }
    // Draw the default note.
    g.drawImage(*keyImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(),
                0, 0, keyImage->getWidth() / 3, keyImage->getHeight());
}

void NoteKey::mouseDown(const juce::MouseEvent&)
{
    if (changeNote != nullptr) {
        changeNote(noteIndex);
    }
}

void NoteKey::mouseEnter(const juce::MouseEvent&)
{
    isOver = true;
    repaint();
}

void NoteKey::mouseExit(const juce::MouseEvent&)
{
    isOver = false;
    repaint();
}

}