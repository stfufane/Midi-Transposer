#include "Panels.h"

PresetsPanel::PresetsPanel(PresetBrowser::PresetManager& pm)
: presetManager(pm)
{
    addAndMakeVisible(presetNameLabel);
    presetNameLabel.setText(presetManager.getCurrentPreset(), juce::NotificationType::dontSendNotification);

    initButton(presetSaveButton, "Save");
    initButton(presetLoadButton, "Load");
    initButton(presetResetButton, "Reset");
}

PresetsPanel::~PresetsPanel()
{
    presetSaveButton.removeListener(this);
    presetLoadButton.removeListener(this);
    presetResetButton.removeListener(this);
}

void PresetsPanel::initButton(juce::Button &ioButton, const juce::String &inText)
{
    addAndMakeVisible(ioButton);
    ioButton.setButtonText(inText);
    ioButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ioButton.addListener(this);
}

void PresetsPanel::buttonClicked(juce::Button *button)
{
    if (button == &presetLoadButton) {
        fileChooser = std::make_unique<FileChooser>(
                "Choose the preset you want to load",
                presetManager.getPresetPath(),
                "*." + PresetBrowser::PresetManager::kPresetsExtension
        );

        fileChooser->launchAsync(FileBrowserComponent::openMode, [&](const FileChooser &chooser) {
            const auto result_file = chooser.getResult();
            const auto preset_name = result_file.getFileNameWithoutExtension();
            if (presetManager.loadPreset(preset_name)) {
                presetNameLabel.setText(preset_name, juce::NotificationType::dontSendNotification);
            }
        });
        return;
    }

    if (button == &presetSaveButton) {
        presetNameChooser = std::make_unique<AlertWindow> ("Save this preset",
                                                          "Choose the name for your preset.",
                                                          MessageBoxIconType::NoIcon);

        presetNameChooser->addTextEditor ("preset", "My cool preset", "Preset name :");
        presetNameChooser->addButton ("OK",     1, KeyPress (KeyPress::returnKey, 0, 0));
        presetNameChooser->addButton ("Cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));

        presetNameChooser->enterModalState (true, ModalCallbackFunction::create (PresetNameDialogChosen { *this }));
        return;
    }

    if (button == &presetResetButton) {
        presetManager.resetPreset();
        presetNameLabel.setText(presetManager.getCurrentPreset(), juce::NotificationType::dontSendNotification);
    }
}

void PresetsPanel::validatePresetSave(int result)
{
    presetNameChooser->exitModalState (result);
    presetNameChooser->setVisible (false);

    if (result == 0) {
        return;
    }

    auto preset_name = presetNameChooser->getTextEditorContents ("preset");
    //TODO: Some input validation.
    if (presetManager.savePreset(preset_name)) {
        presetNameLabel.setText(preset_name, juce::NotificationType::dontSendNotification);
    }
}

void PresetsPanel::resized()
{
    auto button_width = getWidth() / 5;
    const auto height = getHeight() / 2;
    const auto y = getHeight() / 4;
    presetSaveButton.setBounds(0, y, button_width, height);
    presetLoadButton.setBounds(button_width, y, button_width, height);
    presetResetButton.setBounds(button_width * 2, y, button_width, height);
    presetNameLabel.setBounds(button_width * 3, 0, button_width * 2, getHeight());
}