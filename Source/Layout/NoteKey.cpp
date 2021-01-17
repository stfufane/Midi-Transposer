#include "Panels.h"

NoteKey::NoteKey(int index, Image* image) 
    : noteIndex(index), keyImage(image)
{
    addMouseListener(this, true);
}

void NoteKey::paint(Graphics& g)
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

void NoteKey::mouseDown(const MouseEvent&) 
{
    if (changeNote != nullptr)
        changeNote(noteIndex);
}

void NoteKey::mouseEnter(const MouseEvent&)  
{
    isOver = true;
    repaint();
}

void NoteKey::mouseExit(const MouseEvent&)
{
    isOver = false;
    repaint();
}
