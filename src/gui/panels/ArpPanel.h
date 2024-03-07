#ifndef MIDITRANSPOSER_ARPPANEL_H
#define MIDITRANSPOSER_ARPPANEL_H

#include "processor/PluginProcessor.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"
#include "gui/widgets/TextSwitch.h"

namespace Gui
{

class ArpPanel : public juce::Component
{
public:
    ArpPanel() = delete;
    explicit ArpPanel(MidiTransposerAudioProcessor& p);

    void resized() override;
private:
    std::unique_ptr< AttachedComponent<Gui::TextSwitch, juce::ButtonParameterAttachment> > arpActivated;
    std::unique_ptr< AttachedComponent<Gui::TextSwitch, juce::ButtonParameterAttachment> > arpSynced;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > arpSyncRate;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > arpRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpPanel)
};

}

#endif //MIDITRANSPOSER_ARPPANEL_H
