#include "PresetsPanel.h"
#include "gui/panels/MainPanel.h"

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
    presetListComboBox.setTextWhenNothingSelected(PresetBrowser::PresetManager::kInitPreset);
    presetListComboBox.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    presetListComboBox.addListener(this);
    updatePresetsList();

    auto new_icon = juce::Drawable::createFromImageData(BinaryData::new_svg, BinaryData::new_svgSize);
    auto save_icon = juce::Drawable::createFromImageData(BinaryData::save_svg, BinaryData::save_svgSize);
    auto copy_icon = juce::Drawable::createFromImageData(BinaryData::copy_svg, BinaryData::copy_svgSize);
    auto delete_icon = juce::Drawable::createFromImageData(BinaryData::delete_svg, BinaryData::delete_svgSize);

    auto previous_icon = juce::Drawable::createFromImageData(BinaryData::previous_svg, BinaryData::previous_svgSize);
    auto next_icon = juce::Drawable::createFromImageData(BinaryData::next_svg, BinaryData::next_svgSize);

    initButton(presetNewButton, &*new_icon, "Create a new preset");
    initButton(presetSaveButton, &*save_icon, "Save the current preset");
    initButton(presetCopyButton, &*copy_icon, "Copy the current preset");
    initButton(presetDeleteButton, &*delete_icon, "Delete the current preset");

    initButton(presetPreviousButton, &*previous_icon, "Load the previous preset");
    initButton(presetNextButton, &*next_icon, "Load the next preset");
}

PresetsPanel::~PresetsPanel()
{
    presetSaveButton.removeListener(this);
    presetNewButton.removeListener(this);
    presetDeleteButton.removeListener(this);
    presetListComboBox.removeListener(this);
}

void PresetsPanel::initButton(juce::DrawableButton& ioButton, juce::Drawable* inDrawable, const juce::String& inTooltip)
{
    addAndMakeVisible(ioButton);
    ioButton.setImages(inDrawable);
    ioButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ioButton.setTooltip(inTooltip);
    ioButton.addListener(this);
}

void PresetsPanel::buttonClicked(juce::Button* button)
{
    if (button == &presetCopyButton) {
        savePreset();
        return;
    }

    const auto& current_preset = presetManager.getCurrentPreset();
    if (button == &presetSaveButton) {
        if (current_preset == PresetBrowser::PresetManager::kInitPreset) {
            savePreset();
        } else {
            presetManager.savePreset(current_preset);
            updatePresetsList();
        }
        return;
    }

    if (button == &presetDeleteButton) {
        presetManager.deletePreset(current_preset);
        updatePresetsList();
        return;
    }

    if (button == &presetNewButton) {
        presetManager.resetPreset();
        presetListComboBox.setSelectedItemIndex(-1, juce::NotificationType::dontSendNotification);
        return;
    }

    if (button == &presetPreviousButton) {
        loadPreset(-1);
        return;
    }

    if (button == &presetNextButton) {
        loadPreset(1);
        return;
    }
}

void PresetsPanel::loadPreset(int offset)
{
    auto new_index = (presetListComboBox.getSelectedItemIndex() + offset);
    // Make the index loop around.
    new_index = new_index < 0 ? presetListComboBox.getNumItems() - 1 : new_index % presetListComboBox.getNumItems();
    presetManager.loadPreset(presetListComboBox.getItemText(new_index));
    presetListComboBox.setSelectedItemIndex(new_index, juce::NotificationType::dontSendNotification);
}

void PresetsPanel::savePreset()
{
        presetNameChooser = std::make_unique<juce::AlertWindow>("Save this preset",
                                                          "Choose the name for your preset.",
                                                                juce::MessageBoxIconType::NoIcon);

        presetNameChooser->addTextEditor("preset", presetManager.getCurrentPreset(), "Preset name :");
        presetNameChooser->addButton("OK", 1, juce::KeyPress(juce::KeyPress::returnKey, 0, 0));
        presetNameChooser->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey, 0, 0));

        presetNameChooser->enterModalState(true, juce::ModalCallbackFunction::create(PresetNameDialogChosen{*this}));
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
    juce::FlexBox file_buttons;
    file_buttons.flexDirection = juce::FlexBox::Direction::row;
    file_buttons.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    file_buttons.items.add(juce::FlexItem(presetNewButton).withFlex(1).withMargin(juce::FlexItem::Margin(0, 5, 0, 5)));
    file_buttons.items.add(juce::FlexItem(presetSaveButton).withFlex(1).withMargin(juce::FlexItem::Margin(0, 5, 0, 5)));
    file_buttons.items.add(juce::FlexItem(presetCopyButton).withFlex(1).withMargin(juce::FlexItem::Margin(0, 5, 0, 5)));
    file_buttons.items.add(juce::FlexItem(presetDeleteButton).withFlex(1).withMargin(juce::FlexItem::Margin(0, 5, 0, 5)));

    juce::FlexBox preset_buttons;
    preset_buttons.flexDirection = juce::FlexBox::Direction::row;
    preset_buttons.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    preset_buttons.items.add(juce::FlexItem(presetPreviousButton).withFlex(1).withMargin(juce::FlexItem::Margin(0, 20, 0, 20)));
    preset_buttons.items.add(juce::FlexItem(presetNextButton).withFlex(1).withMargin(juce::FlexItem::Margin(0, 20, 0, 20)));

    if (auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();

        // Draw the file buttons icons.
        file_buttons.performLayout(juce::Rectangle<int>(0, static_cast<int>(coordinates.mHeaderHeight),
                                                getWidth(), static_cast<int>(coordinates.mButtonHeight))
                                   .reduced(static_cast<int>(coordinates.mMargin) * 2, 0));

        // Calculate the combobox coordinates.
        auto combo_bounds = juce::Rectangle<int>(0, static_cast<int>(coordinates.mHeaderHeight) + static_cast<int>(coordinates.mButtonHeight),
                                         getWidth(), static_cast<int>(static_cast<int>(coordinates.mButtonHeight)))
                                         .reduced(static_cast<int>(coordinates.mMargin) * 2, static_cast<int>(coordinates.mMargin));
        presetListComboBox.setBounds(combo_bounds);

        // Draw the preset navigation buttons just below.
        preset_buttons.performLayout(combo_bounds
            .translated(0, combo_bounds.getHeight() + static_cast<int>(coordinates.mMargin))
            .withHeight(static_cast<int>(coordinates.mButtonHeight)));
    }
}

}