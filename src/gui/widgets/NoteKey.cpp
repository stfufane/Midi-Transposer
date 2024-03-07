#include "NoteKey.h"
#include "params/Params.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"

namespace Gui
{

NoteKey::NoteKey(const int index)
        : TextButton("Note Key " + juce::String(index)), noteIndex(index)
{
    Button::setTooltip("Click to change the intervals for this note.");
}

void NoteKey::setPlayed(const int index) 
{
    const auto oldPlayed = isPlayed;
    isPlayed = (noteIndex == index % 12); 
    if (oldPlayed != isPlayed) {
        repaint();
    }
}

void NoteKey::paint(juce::Graphics& g)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    if (const auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();
        const auto key_bounds = juce::Rectangle(0.f, 0.f,
                                                       static_cast<float>(getWidth()),
                                                       static_cast<float>(getHeight())).reduced(3.f);
        auto draw_key = [&](int background_color, int text_color) {
            if (isPlayed) {
                background_color = juce::TextButton::ColourIds::buttonColourId;
                text_color = juce::TextButton::ColourIds::textColourOnId;
            } else if (isEdited) {
                background_color = juce::Slider::backgroundColourId;
                text_color = juce::Slider::ColourIds::textBoxTextColourId;
            } 

            if (isOver) {
                g.addTransform(juce::AffineTransform::scale(coordinates.mKeyOver));
            }

            g.setColour(findColour(background_color));
            g.fillRoundedRectangle(key_bounds, coordinates.mKeyCorner);
            g.setColour(findColour(text_color));
            g.drawRoundedRectangle(key_bounds, coordinates.mKeyCorner, 1.f);
            g.setFont(LnF::getDefaultFont(coordinates.mKeyFontSize));
            g.drawText(std::string(Notes::labels[static_cast<size_t>(noteIndex)]), key_bounds, juce::Justification::centred);
        };
        if (Notes::whiteNotes[static_cast<size_t>(noteIndex)]) {
            draw_key(juce::Label::ColourIds::textColourId, juce::Label::ColourIds::backgroundColourId);
        } else {
            draw_key(juce::Label::ColourIds::backgroundColourId, juce::Label::ColourIds::textColourId);
        }
    }
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