#ifndef MIDIBASSPEDALCHORDS_SETTINGSPANEL_H
#define MIDIBASSPEDALCHORDS_SETTINGSPANEL_H

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"

namespace Gui
{

/**
 * @brief The header with MIDI params and Arpeggiator params + preset manager
 */
class SettingsPanel : public juce::Component
{
public:
    SettingsPanel() = delete;
    explicit SettingsPanel(MidiBassPedalChordsAudioProcessor& p);

    void resized() override;

    void paint(juce::Graphics& g) override;

private:
    void initLabel(juce::Label& ioLabel);

    juce::Label lblInputChannel    { "lblInputChannel",    "Input Channel" };
    juce::Label lblOutputChannel   { "lblOutputChannel",   "Output Channel" };
    juce::Label lblOctaveTranspose { "lblOctaveTranspose", "Transposition" };

    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > inputChannel;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > outputChannel;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > octaveTranspose;

    juce::Label lblArpRate { "lblArpRate", "Rate" };

    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpActivated;
    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpSynced;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > arpSyncRate;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > arpRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPanel)
};

}
#endif //MIDIBASSPEDALCHORDS_SETTINGSPANEL_H
