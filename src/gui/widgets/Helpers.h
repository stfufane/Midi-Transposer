#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

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
    : component(std::forward<Args>(args)...),
      attachment(param, component)
    {
        parent.addAndMakeVisible(component);
        if (init != nullptr) { init(component); }
        attachment.sendInitialUpdate();
    }

    CompType& getComponent() { return component; }
private:
    CompType component;
    CompAttachment attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttachedComponent)
};

}