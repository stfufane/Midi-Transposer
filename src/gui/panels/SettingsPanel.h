#ifndef MIDIBASSPEDALCHORDS_SETTINGSPANEL_H
#define MIDIBASSPEDALCHORDS_SETTINGSPANEL_H

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/widgets/Helpers.h"
#include "gui/widgets/CustomSlider.h"
#include "gui/Configuration.hpp"

namespace Gui
{

struct CompPositions {
    juce::Rectangle<int> mInputCoords { 0, 0, 100, 100 };

    static std::string getFileName() { return "positions.json"; }
};

inline void from_json(const nlohmann::json& j, CompPositions& pos)
{
    auto input_coords = j.at("input_coords");
    pos.mInputCoords = { input_coords.at("x"), input_coords.at("y"), input_coords.at("w"), input_coords.at("h") };
}

/**
 * @brief The header with MIDI params and Arpeggiator params + preset manager
 */
class SettingsPanel : public juce::Component, public Configuration<CompPositions>::Listener<CompPositions>
{
public:
    SettingsPanel() = delete;
    explicit SettingsPanel(MidiBassPedalChordsAudioProcessor& p);
    ~SettingsPanel() override;

    void resized() override;

    void paint(juce::Graphics& g) override;

private:
    void onConfigChanged(const CompPositions& positions) override;

    Configuration<CompPositions> mConfiguration;

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
