#pragma once

#include <JuceHeader.h>

// Each GUI component linked to a parameter is created as an AttachedComponent to manage the attachment in the same object.
// It's automatically made visible when it's initialised. The init parameter function can be used to define the component properties.
template <typename CompType, typename CompAttachment>
class AttachedComponent
{
public:
    AttachedComponent<CompType, CompAttachment>(RangedAudioParameter& param, Component& parent, std::function<void(CompType&)> init = nullptr)
    {
        if (init != nullptr) init(component);
        attachment.reset(new CompAttachment(param, component));
        parent.addAndMakeVisible(component);
        attachment->sendInitialUpdate();
    }
    CompType component;
private:
    std::unique_ptr<CompAttachment> attachment;
};
