#include "NoteMappingToggle.h"

namespace Gui
{

NoteMappingToggle::NoteMappingToggle(NoteParam& noteParam, juce::Image* image)
        : juce::Component("Note Mapping Toggle " + juce::String(noteParam.noteIndex)), powerImage(image)
{
    mapChoice = std::make_unique<AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>>(
            *noteParam.mapNote, *this,
            [this](IndexedToggleButton& button) {
                button.setImage(powerImage, 0, 1, false);
                button.onStateChange = [&button]() {
                    if (button.getToggleState()) {
                        button.setTooltip("Press to deactivate mapping for this note");
                    } else {
                        button.setTooltip("Press to activate mapping for this note");
                    }
                };
            }
    );
}

void NoteMappingToggle::resized()
{
    mapChoice->component.setBounds(getLocalBounds());
}

}