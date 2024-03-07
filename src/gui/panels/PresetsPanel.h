#ifndef MIDITRANSPOSER_PRESETSPANEL_H
#define MIDITRANSPOSER_PRESETSPANEL_H

#include "presets/PresetManager.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui
{
/**
 * @brief Contains the current preset name + presets browsing/save/load
 */
class PresetsPanel final : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
{
public:
    PresetsPanel() = delete;
    explicit PresetsPanel(PresetBrowser::PresetManager& pm);
    ~PresetsPanel() override;

    void initButton(juce::DrawableButton& ioButton, const juce::Drawable* inDrawable, const juce::String& inTooltip);

    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    void validatePresetSave(int result);
private:
    void updatePresetsList();

    /**
     * @brief Load a preset from the preset manager
     * @param offset Offset from the current preset (-1 for previous, 1 for next)
    */
    void loadPreset(int offset);

    void savePreset();

    PresetBrowser::PresetManager& presetManager; // Only a reference, the audio processor is owning it.

    juce::DrawableButton presetNewButton { "btnNewPreset", juce::DrawableButton::ButtonStyle::ImageFitted };
    juce::DrawableButton presetSaveButton { "btnSavePreset", juce::DrawableButton::ButtonStyle::ImageFitted };
    juce::DrawableButton presetCopyButton { "btnCopyPreset", juce::DrawableButton::ButtonStyle::ImageFitted };
    juce::DrawableButton presetDeleteButton { "btnDeletePreset", juce::DrawableButton::ButtonStyle::ImageFitted };

    juce::DrawableButton presetPreviousButton { "btnPreviousPreset", juce::DrawableButton::ButtonStyle::ImageFitted };
    juce::DrawableButton presetNextButton { "btnNextPreset", juce::DrawableButton::ButtonStyle::ImageFitted };

    juce::ComboBox presetListComboBox { "cmbPresetList" };

    std::unique_ptr<juce::AlertWindow> presetNameChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsPanel)
};
}

#endif //MIDITRANSPOSER_PRESETSPANEL_H
