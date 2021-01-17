#include "Panels.h"

NoteMappingToggle::NoteMappingToggle(int index, NoteParam& noteParam, Image* image) 
    : noteIndex(index), powerImage(image)
{
    mapChoice = std::make_unique<AttachedComponent<IndexedToggleButton, ButtonParameterAttachment>>(
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
    mapChoice.get()->component.setBounds(getLocalBounds());
}