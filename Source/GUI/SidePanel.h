#ifndef MIDIBASSPEDALCHORDS_SIDEPANEL_H
#define MIDIBASSPEDALCHORDS_SIDEPANEL_H

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Helpers.h"

namespace Gui
{

/**
 * @brief The header with MIDI params and Arpeggiator params + preset manager
 */
struct SidePanel : public juce::Component
{
    SidePanel() = delete;
    explicit SidePanel(MidiBassPedalChordsAudioProcessor& p);

    void resized() override;

    void initLabel(juce::Label& ioLabel);

    juce::Label lblInputChannel    { "lblInputChannel",    "Input Channel" };
    juce::Label lblOutputChannel   { "lblOutputChannel",   "Output Channel" };
    juce::Label lblOctaveTranspose { "lblOctaveTranspose", "Transposition" };

    std::unique_ptr< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> > inputChannel;
    std::unique_ptr< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> > outputChannel;
    std::unique_ptr< AttachedComponent<OctaveSlider, juce::SliderParameterAttachment> > octaveTranspose;

    juce::Label lblArpRate { "lblArpRate", "Rate" };

    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpActivated;
    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpSynced;
    std::unique_ptr< AttachedComponent<SyncRateSlider, juce::SliderParameterAttachment> > arpSyncRate;
    std::unique_ptr< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> > arpRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidePanel)
};

}
#endif //MIDIBASSPEDALCHORDS_SIDEPANEL_H
