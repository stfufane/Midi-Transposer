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
    presetListComboBox.setTextWhenNothingSelected("Default");
    presetListComboBox.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    presetListComboBox.addListener(this);
    updatePresetsList();

    auto save_icon = juce::Drawable::createFromImageData(BinaryData::save_png, BinaryData::save_pngSize);
    initButton(presetSaveButton, &*save_icon);
    initButton(presetResetButton, &*save_icon);
    initButton(presetDeleteButton, &*save_icon);

}

PresetsPanel::~PresetsPanel()
{
    presetSaveButton.removeListener(this);
    presetResetButton.removeListener(this);
    presetDeleteButton.removeListener(this);
    presetListComboBox.removeListener(this);
}

void PresetsPanel::initButton(juce::DrawableButton& ioButton, juce::Drawable* inDrawable)
{
    addAndMakeVisible(ioButton);
    // ioButton.setButtonText(inText);
    ioButton.setImages(inDrawable);
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
    using juce::operator""_px;
    using juce::operator""_fr;

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateRows    = { Track (1_fr) };
    grid.templateColumns = { Track (1_fr), Track (1_fr), Track (1_fr) };

    grid.alignContent    = juce::Grid::AlignContent::center;
    grid.justifyContent  = juce::Grid::JustifyContent::center;
    grid.alignItems      = juce::Grid::AlignItems::center;
    grid.justifyItems    = juce::Grid::JustifyItems::center;

    grid.columnGap = 3_px;

    grid.items = {
        juce::GridItem(presetResetButton),
        juce::GridItem(presetSaveButton),
        juce::GridItem(presetDeleteButton)
    };

    if (auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();
        grid.performLayout(juce::Rectangle<int>(0, static_cast<int>(coordinates.mHeaderHeight),
                                                getWidth(), static_cast<int>(coordinates.mButtonHeight))
                                   .reduced(static_cast<int>(coordinates.mMargin) * 2, 0));
        auto combo_bounds = juce::Rectangle<int>(0, static_cast<int>(coordinates.mHeaderHeight) + static_cast<int>(coordinates.mButtonHeight),
                                         getWidth(), static_cast<int>(static_cast<int>(coordinates.mButtonHeight)))
                                         .reduced(static_cast<int>(coordinates.mMargin) * 2, static_cast<int>(coordinates.mMargin));
        presetListComboBox.setBounds(combo_bounds);
    }
}

}