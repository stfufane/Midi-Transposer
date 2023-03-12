#ifndef MIDIBASSPEDALCHORDS_ARPPANEL_H
#define MIDIBASSPEDALCHORDS_ARPPANEL_H

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"

namespace Gui
{

class ArpPanel : public juce::Component
{
public:
    ArpPanel() = delete;
    explicit ArpPanel(MidiBassPedalChordsAudioProcessor& p);

    void resized() override;
private:
    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpActivated;
    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpSynced;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > arpSyncRate;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > arpRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpPanel)
};

}

#endif //MIDIBASSPEDALCHORDS_ARPPANEL_H
