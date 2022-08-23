#ifndef MIDIBASSPEDALCHORDS_MAINPANEL_H
#define MIDIBASSPEDALCHORDS_MAINPANEL_H

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "KeysPanel.h"
#include "IntervalsPanel.h"
#include "PresetsPanel.h"

namespace Gui
{

struct MainPanel : public juce::Component
{
    MainPanel() = delete;
    explicit MainPanel(MidiBassPedalChordsAudioProcessor& p);

    void resized() override;

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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
};

}

#endif //MIDIBASSPEDALCHORDS_MAINPANEL_H
