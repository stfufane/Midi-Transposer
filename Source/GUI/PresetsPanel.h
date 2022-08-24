#ifndef MIDIBASSPEDALCHORDS_PRESETSPANEL_H
#define MIDIBASSPEDALCHORDS_PRESETSPANEL_H

#include <JuceHeader.h>
#include "../PresetBrowser/PresetManager.h"

namespace Gui
{
/**
 * @brief Contains the current preset name + presets browsing/save/load
 */
class PresetsPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
{
public:
    PresetsPanel() = delete;
    explicit PresetsPanel(PresetBrowser::PresetManager& pm);
    ~PresetsPanel() override;

    void initButton(juce::Button& ioButton, const juce::String& inText);

    void resized() override;
    void paint(juce::Graphics &g) override;
    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    void validatePresetSave(int result);
private:
    void updatePresetsList();

    PresetBrowser::PresetManager& presetManager; // Only a reference, the audio processor is owning it.

    juce::TextButton presetSaveButton { "btnSavePreset" };
    juce::TextButton presetResetButton { "btnResetPreset" };

    juce::ComboBox presetListComboBox { "cmbPresetList" };

    std::unique_ptr<juce::AlertWindow> presetNameChooser = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsPanel)
};
}

#endif //MIDIBASSPEDALCHORDS_PRESETSPANEL_H
