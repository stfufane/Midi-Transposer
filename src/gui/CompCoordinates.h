#ifndef MIDITRANSPOSER_COMPCOORDINATES_H
#define MIDITRANSPOSER_COMPCOORDINATES_H

#include "nlohmann/json.hpp"

namespace Gui
{

struct CompCoordinates {
    // Coordinates of the different panels.
    juce::Rectangle<float> mMidiPanel;
    juce::Rectangle<float> mMidiLabels;
    juce::Rectangle<float> mMidiOct;
    juce::Rectangle<float> mArpPanel;
    juce::Rectangle<float> mArpSwitch;
    juce::Rectangle<float> mArpSyncSwitch;
    juce::Rectangle<float> mPresetsPanel;
    juce::Rectangle<float> mKeysPanel;
    juce::Rectangle<float> mIntervalsPanel;
    juce::Rectangle<float> mIntervalsTransposeLabel;
    juce::Rectangle<float> mIntervalsSlidersLabel;
    juce::Rectangle<float> mTooltipsPanel;
    juce::Rectangle<float> mIntervalsToggle;

    // Some globals to draw different components.
    float mMargin { 1.f }; // The global margin separating the different panels
    float mSwitchMargin { 1.f };
    float mFrameCorner { 1.f }; // The frame's rounded rectangles size
    float mHeaderHeight { 1.f }; // The height of the title blocks
    float mHeaderFontSize { 1.f }; // The font size of the title blocks
    float mLabelFontSize { 1.f }; // The font size of the block labels
    float mMidiInX { 1.f };
    float mMidiOutX { 1.f };
    float mMidiOctX { 1.f };
    float mKnobHeight { 1.f };
    float mToggleHeight { 1.f };
    float mToggleWidth { 1.f };
    float mToggleSyncWidth { 1.f }; 
    float mToggleX { 1.f };
    float mButtonHeight { 1.f };
    float mKeyFontSize { 1.f };
    float mKeyCorner { 1.f };
    float mKeyRatio { 1.f };
    float mKeyOver { 1.f };
    float mIntervalsX { 1.f };
    float mIntervalsH { 1.f };
    float mIntervalKnobW { 1.f };
    float mIntervalsSlidersW { 1.f };
    float mIntervalsLabelCorner { 1.f };
    float mIntervalsLabelFontSize { 1.f };

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
    auto arp_switch = j.at("arp_switch");
    pos.mArpSwitch = { arp_switch.at("x"), arp_switch.at("y"),
                       arp_switch.at("w"), arp_switch.at("h") };
    auto arp_sync_switch = j.at("arp_sync_switch");
    pos.mArpSyncSwitch = { arp_sync_switch.at("x"), arp_sync_switch.at("y"),
                           arp_sync_switch.at("w"), arp_sync_switch.at("h") };
    auto presets_panel = j.at("presets_panel");
    pos.mPresetsPanel = { presets_panel.at("x"), presets_panel.at("y"),
                          presets_panel.at("w"), presets_panel.at("h") };
    auto keys_panel = j.at("keys_panel");
    pos.mKeysPanel = { keys_panel.at("x"), keys_panel.at("y"),
                       keys_panel.at("w"), keys_panel.at("h") };
    auto intervals_panel = j.at("intervals_panel");
    pos.mIntervalsPanel = { intervals_panel.at("x"), intervals_panel.at("y"),
                            intervals_panel.at("w"), intervals_panel.at("h") };
    auto intervals_transpose_label = j.at("intervals_transpose_label");
    pos.mIntervalsTransposeLabel = { intervals_transpose_label.at("x"), intervals_transpose_label.at("y"),
                                     intervals_transpose_label.at("w"), intervals_transpose_label.at("h") };
    auto intervals_sliders_label = j.at("intervals_sliders_label");
    pos.mIntervalsSlidersLabel = { intervals_sliders_label.at("x"), intervals_sliders_label.at("y"),
                                   intervals_sliders_label.at("w"), intervals_sliders_label.at("h") };
    auto tooltips_panel = j.at("tooltips_panel");
    pos.mTooltipsPanel = { tooltips_panel.at("x"), tooltips_panel.at("y"),
                           tooltips_panel.at("w"), tooltips_panel.at("h") };
    auto intervals_toggle = j.at("intervals_toggle");
    pos.mIntervalsToggle = { intervals_toggle.at("x"), intervals_toggle.at("y"),
                             intervals_toggle.at("w"), intervals_toggle.at("h") };

    j.at("margin").get_to(pos.mMargin);
    j.at("switch_margin").get_to(pos.mSwitchMargin);
    j.at("frame_corner").get_to(pos.mFrameCorner);
    j.at("header_height").get_to(pos.mHeaderHeight);
    j.at("header_font_size").get_to(pos.mHeaderFontSize);
    j.at("label_font_size").get_to(pos.mLabelFontSize);
    j.at("midi_in_x").get_to(pos.mMidiInX);
    j.at("midi_out_x").get_to(pos.mMidiOutX);
    j.at("midi_oct_x").get_to(pos.mMidiOctX);
    j.at("knob_height").get_to(pos.mKnobHeight);
    j.at("toggle_height").get_to(pos.mToggleHeight);
    j.at("toggle_width").get_to(pos.mToggleWidth);
    j.at("toggle_sync_width").get_to(pos.mToggleSyncWidth);
    j.at("toggle_x").get_to(pos.mToggleX);
    j.at("button_height").get_to(pos.mButtonHeight);
    j.at("key_font_size").get_to(pos.mKeyFontSize);
    j.at("key_corner").get_to(pos.mKeyCorner);
    j.at("key_ratio").get_to(pos.mKeyRatio);
    j.at("key_over").get_to(pos.mKeyOver);
    j.at("intervals_x").get_to(pos.mIntervalsX);
    j.at("intervals_h").get_to(pos.mIntervalsH);
    j.at("intervals_knob_w").get_to(pos.mIntervalKnobW);
    j.at("intervals_sliders_w").get_to(pos.mIntervalsSlidersW);
    j.at("intervals_label_corner").get_to(pos.mIntervalsLabelCorner);
    j.at("intervals_label_fontsize").get_to(pos.mIntervalsLabelFontSize);
}

}

#endif //MIDITRANSPOSER_COMPCOORDINATES_H
