#include "Panels.h"

NoteKey::NoteKey(int index, juce::Image* image) 
    : noteIndex(index), keyImage(image)
{ }

void NoteKey::paint(juce::Graphics& g)
{
    g.drawImage(*keyImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 
                0, 0, keyImage->getWidth() / 3.0, keyImage->getHeight());
    if (isEdited)
    {
        g.drawImage(*keyImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 
                keyImage->getWidth() / 1.5, 0, keyImage->getWidth() / 3.0, keyImage->getHeight());
    }
    if (isOver)
    {
        g.drawImage(*keyImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 
                keyImage->getWidth() / 3.0, 0, keyImage->getWidth() / 3.0, keyImage->getHeight());
    }
}

void NoteKey::mouseDown(const juce::MouseEvent&) 
{
    if (changeNote != nullptr)
        changeNote(noteIndex);
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
