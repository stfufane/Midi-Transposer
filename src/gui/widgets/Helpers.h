#pragma once

#include "JuceHeader.h"

namespace Gui
{

/**
 * @brief Each gui component linked to a parameter is created as an AttachedComponent to manage the attachment in the same object.
 * It's automatically made visible when it's initialised. The init parameter function can be used to define the component properties.
 * Some extra args can be passed to construct the component
 * @tparam CompType The type of component to display (slider, button, etc.)
 * @tparam CompAttachment The ParameterAttachment linked to the component.
 */
template<typename CompType, typename CompAttachment>
class AttachedComponent
{
public:
    template<typename... Args>
    AttachedComponent<CompType, CompAttachment>(juce::RangedAudioParameter& param, juce::Component& parent,
                                                std::function<void(CompType&)> init = nullptr, Args&&... args)
    {
        component = std::make_unique<CompType>(std::forward<Args>(args)...);
        attachment = std::make_unique<CompAttachment>(param, *component);
        parent.addAndMakeVisible(component.get());
        if (init != nullptr) { init(*component); }
        attachment->sendInitialUpdate();
    }

    CompType& getComponent() { return *component; }
private:
    std::unique_ptr<CompType> component;
    std::unique_ptr<CompAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttachedComponent)
};

}