#include "PresetsPanel.h"

namespace Gui
{

/**
 * @brief A callback struct used by the AlertWindow to validate the user input.
 */
struct PresetNameDialogChosen
{
    void operator()(int result) const noexcept
    {
        panel.validatePresetSave(result);
    }

    PresetsPanel& panel;
};

PresetsPanel::PresetsPanel(PresetBrowser::PresetManager& pm)
        : juce::Component("Presets Panel"),
          presetManager(pm)
{
    addAndMakeVisible(presetListComboBox);
    presetListComboBox.setTextWhenNothingSelected("Default");
    presetListComboBox.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    presetListComboBox.addListener(this);
    updatePresetsList();

    initButton(presetSaveButton, "Save");
    initButton(presetResetButton, "Reset");
}

PresetsPanel::~PresetsPanel()
{
    presetSaveButton.removeListener(this);
    presetResetButton.removeListener(this);
    presetListComboBox.removeListener(this);
}

void PresetsPanel::initButton(juce::Button& ioButton, const juce::String& inText)
{
    addAndMakeVisible(ioButton);
    ioButton.setButtonText(inText);
    ioButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ioButton.addListener(this);
}

void PresetsPanel::buttonClicked(juce::Button* button)
{
    if (button == &presetSaveButton) {
        presetNameChooser = std::make_unique<juce::AlertWindow>("Save this preset",
                                                          "Choose the name for your preset.",
                                                                juce::MessageBoxIconType::NoIcon);

        presetNameChooser->addTextEditor("preset", presetManager.getCurrentPreset(), "Preset name :");
        presetNameChooser->addButton("OK", 1, juce::KeyPress(juce::KeyPress::returnKey, 0, 0));
        presetNameChooser->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey, 0, 0));

        presetNameChooser->enterModalState(true, juce::ModalCallbackFunction::create(PresetNameDialogChosen{*this}));
        return;
    }

    if (button == &presetResetButton) {
        presetManager.resetPreset();
        presetListComboBox.setSelectedItemIndex(-1, juce::NotificationType::dontSendNotification);
    }
}

void PresetsPanel::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetListComboBox) {
        presetManager.loadPreset(presetListComboBox.getItemText(presetListComboBox.getSelectedItemIndex()));
    }
}

void PresetsPanel::updatePresetsList()
{
    presetListComboBox.clear(juce::dontSendNotification);
    const auto allPresets = presetManager.getAllPresets();
    const auto currentPreset = presetManager.getCurrentPreset();
    presetListComboBox.addItemList(allPresets, 1);
    presetListComboBox.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
}

void PresetsPanel::validatePresetSave(int result)
{
    presetNameChooser->exitModalState(result);
    presetNameChooser->setVisible(false);

    if (result == 0) {
        return;
    }

    auto preset_name = presetNameChooser->getTextEditorContents("preset");
    //TODO: Some input validation.
    if (presetManager.savePreset(preset_name)) {
        updatePresetsList();
    }
}

void PresetsPanel::resized()
{
    const auto button_width = getWidth() / 6;
    const auto height = getHeight() / 2;
    const auto y = getHeight() / 4;

    presetListComboBox.setBounds(button_width, y, button_width * 2, height);
    presetSaveButton.setBounds(juce::Rectangle<int>(button_width * 3, y, button_width, height).reduced(button_width / 4, 0));
    presetResetButton.setBounds(juce::Rectangle<int>(button_width * 4, y, button_width, height).reduced(button_width / 4, 0));
}

}