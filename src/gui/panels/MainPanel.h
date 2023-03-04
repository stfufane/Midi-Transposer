#ifndef MIDIBASSPEDALCHORDS_MAINPANEL_H
#define MIDIBASSPEDALCHORDS_MAINPANEL_H

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/panels/KeysPanel.h"
#include "gui/panels/IntervalsPanel.h"
#include "gui/panels/PresetsPanel.h"
#include "gui/panels/SettingsPanel.h"

namespace Gui
{

class MainPanel : public juce::Component
{
public:
    MainPanel() = delete;
    explicit MainPanel(MidiBassPedalChordsAudioProcessor& p);

    void resized() override;

    juce::Component* getTooltipPanel() { return &tooltipPanel; }

private:
    /**
     * @brief Resets the intervals panel with the currently edited note.
     * @param noteParam the note that is edited.
     */
    void initIntervalsPanel(NoteParam& noteParam);

    /**
     * @brief Notifies all the note keys to tell which one is edited and update its color.
     * @param index the index of the edited note. -1 means there's no note currently edited.
     */
    void updateNoteEdited(int index);

    /**
     * @brief Contains the name of the current preset + buttons to load/save/reset presets
     */
    PresetsPanel presetsPanel;

    /**
     * @brief Contains the midi settings and the arpeggiator settings.
     */
    SettingsPanel settingsPanel;

    /**
     * @brief Fills the empty space when not editing a key. Contains some instruction.
     */
    juce::Label dummyPanel { "Dummy Panel", "Click on a key to configure the associated chord" };

    /**
     * @brief Contains the twelve keys that trigger the display of the intervals panel.
     */
    KeysPanel keysPanel;

    /**
     * @brief Contains toggle buttons to activate the intervals for a certain key.
     */
    std::unique_ptr<IntervalsPanel> intervalsPanel = nullptr;

    /**
     * @brief Container that will be put at the bottom to display all the tooltips at the same place.
     */
    juce::Component tooltipPanel { "tooltipPanel" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
};

}

#endif //MIDIBASSPEDALCHORDS_MAINPANEL_H
