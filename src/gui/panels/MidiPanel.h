#ifndef MIDITRANSPOSER_MIDIPANEL_H
#define MIDITRANSPOSER_MIDIPANEL_H

#include "processor/PluginProcessor.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"

namespace Gui
{

/**
 * @brief The header with MIDI params and Arpeggiator params + preset manager
 */
class MidiPanel : public juce::Component
{
public:
    MidiPanel() = delete;
    explicit MidiPanel(MidiTransposerAudioProcessor& p);

    void resized() override;

private:
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > inputChannel;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > outputChannel;
    std::unique_ptr< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> > octaveTranspose;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPanel)
};

}
#endif //MIDITRANSPOSER_MIDIPANEL_H
