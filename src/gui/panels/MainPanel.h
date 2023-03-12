#ifndef MIDIBASSPEDALCHORDS_MAINPANEL_H
#define MIDIBASSPEDALCHORDS_MAINPANEL_H

#include "JuceHeader.h"
#include "processor/PluginProcessor.h"
#include "gui/panels/KeysPanel.h"
#include "gui/panels/IntervalsPanel.h"
#include "gui/panels/PresetsPanel.h"
#include "gui/panels/MidiPanel.h"
#include "gui/panels/ArpPanel.h"

namespace Gui
{

struct CompCoordinates {
    // Coordinates of the different panels.
    juce::Rectangle<float> mMidiPanel;
    juce::Rectangle<float> mMidiLabels;
    juce::Rectangle<float> mMidiOct;
    juce::Rectangle<float> mArpPanel;
    juce::Rectangle<float> mArpLabel;
    juce::Rectangle<float> mPresetsPanel;
    juce::Rectangle<float> mKeysPanel;
    juce::Rectangle<float> mIntervalsPanel;
    juce::Rectangle<float> mTooltipsPanel;

    // Some globals to draw different components.
    float mMargin { 1.f };
    float mFrameCorner { 1.f };
    float mHeaderHeight { 1.f };
    float mHeaderFontSize { 1.f };
    float mLabelFontSize { 1.f };
    float mMidiInX { 1.f };
    float mMidiOutX { 1.f };
    float mMidiOctX { 1.f };
    float mKnobHeight { 1.f };
    float mButtonHeight { 1.f };
    float mPresetsRatio { 1.f };
    float mKeyFontSize { 1.f };
    float mKeyCorner { 1.f };
    float mKeyRatio { 1.f };
    float mKeyOver { 1.f };

    static std::string getFileName() { return "positions.json"; }
};

inline void from_json(const nlohmann::json& j, CompCoordinates& pos)
{
    auto midi_panel = j.at("midi_panel");
    pos.mMidiPanel = { midi_panel.at("x"), midi_panel.at("y"),
                       midi_panel.at("w"), midi_panel.at("h") };
    auto midi_labels = j.at("midi_labels");
    pos.mMidiLabels = { midi_labels.at("x"), midi_labels.at("y"),
                        midi_labels.at("w"), midi_labels.at("h") };
    auto midi_oct = j.at("midi_oct");
    pos.mMidiOct = { midi_oct.at("x"), midi_oct.at("y"),
                     midi_oct.at("w"), midi_oct.at("h") };
    auto arp_panel = j.at("arp_panel");
    pos.mArpPanel = { arp_panel.at("x"), arp_panel.at("y"),
                       arp_panel.at("w"), arp_panel.at("h") };
    auto arp_label = j.at("arp_label");
    pos.mArpLabel = { arp_label.at("x"), arp_label.at("y"),
                      arp_label.at("w"), arp_label.at("h") };
    auto presets_panel = j.at("presets_panel");
    pos.mPresetsPanel = { presets_panel.at("x"), presets_panel.at("y"),
                          presets_panel.at("w"), presets_panel.at("h") };
    auto keys_panel = j.at("keys_panel");
    pos.mKeysPanel = { keys_panel.at("x"), keys_panel.at("y"),
                       keys_panel.at("w"), keys_panel.at("h") };
    auto intervals_panel = j.at("intervals_panel");
    pos.mIntervalsPanel = { intervals_panel.at("x"), intervals_panel.at("y"),
                            intervals_panel.at("w"), intervals_panel.at("h") };
    auto tooltips_panel = j.at("tooltips_panel");
    pos.mTooltipsPanel = { tooltips_panel.at("x"), tooltips_panel.at("y"),
                           tooltips_panel.at("w"), tooltips_panel.at("h") };

    j.at("margin").get_to(pos.mMargin);
    j.at("frame_corner").get_to(pos.mFrameCorner);
    j.at("header_height").get_to(pos.mHeaderHeight);
    j.at("header_font_size").get_to(pos.mHeaderFontSize);
    j.at("label_font_size").get_to(pos.mLabelFontSize);
    j.at("midi_in_x").get_to(pos.mMidiInX);
    j.at("midi_out_x").get_to(pos.mMidiOutX);
    j.at("midi_oct_x").get_to(pos.mMidiOctX);
    j.at("knob_height").get_to(pos.mKnobHeight);
    j.at("button_height").get_to(pos.mButtonHeight);
    j.at("presets_ratio").get_to(pos.mPresetsRatio);
    j.at("key_font_size").get_to(pos.mKeyFontSize);
    j.at("key_corner").get_to(pos.mKeyCorner);
    j.at("key_ratio").get_to(pos.mKeyRatio);
    j.at("key_over").get_to(pos.mKeyOver);
}

class MainPanel : public juce::Component, public Configuration<CompCoordinates>::Listener<CompCoordinates>
{
public:
    MainPanel() = delete;
    explicit MainPanel(MidiBassPedalChordsAudioProcessor& p);
    ~MainPanel() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    juce::Component* getTooltipPanel() { return &tooltipPanel; }

    [[nodiscard]] const CompCoordinates& getCoordinates() const { return mConfiguration.getData(); }

private:
    Configuration<CompCoordinates> mConfiguration;

    void onConfigChanged(const CompCoordinates& positions) override;

    /**
     * @brief Resets the intervals panel with the currently edited note.
     * @param noteParam the note that is edited.
     */
    void initIntervalsPanel(Params::NoteParam& noteParam);

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
     * @brief Contains the midi settings.
     */
    MidiPanel midiPanel;

    /**
     * @brief Contains the arpeggiator settings.
     */
    ArpPanel arpPanel;

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
