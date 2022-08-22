#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../Params/Params.h"
#include "../PluginProcessor.h"

constexpr auto NB_NOTES = 12;
constexpr auto NB_INTERVALS = 12;

using Track = juce::Grid::TrackInfo;
using Fr = juce::Grid::Fr;

/**
 * The header with MIDI params
 */
struct HeaderPanel : public juce::Component
{
    HeaderPanel() = delete;
    explicit HeaderPanel(MidiBassPedalChordsAudioProcessor& p);

    void resized() override;

    juce::Label lblInputChannel    { "lblInputChannel",    "Input Channel" };
    juce::Label lblOutputChannel   { "lblOutputChannel",   "Output Channel" };
    juce::Label lblOctaveTranspose { "lblOctaveTranspose", "Transposition" };

    std::unique_ptr< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> > inputChannel;
    std::unique_ptr< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> > outputChannel;
    std::unique_ptr< AttachedComponent<OctaveSlider, juce::SliderParameterAttachment> > octaveTranspose;

    juce::Label lblArpRate { "lblArpRate", "Rate" };

    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpActivated;
    std::unique_ptr< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> > arpSynced;
    std::unique_ptr< AttachedComponent<SyncRateSlider, juce::SliderParameterAttachment> > arpSyncRate;
    std::unique_ptr< AttachedComponent<HorizontalSlider, juce::SliderParameterAttachment> > arpRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderPanel)
};

/**
 * @brief This panel holds the 12 interval buttons for a note + a semitone transposition slider.
 */
struct IntervalsPanel : public juce::Component
{
    IntervalsPanel() = delete;
    explicit IntervalsPanel(NoteParam& noteParam);
    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Image buttonsImage { juce::ImageCache::getFromMemory(BinaryData::buttons_png, BinaryData::buttons_pngSize) };

    std::unique_ptr<AttachedComponent<SemitoneSlider, juce::SliderParameterAttachment>> transpose;
    std::vector<std::unique_ptr<AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>>> intervalsChoices;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntervalsPanel)
};

/**
 * @brief The drawing of a key with an event to display its intervals.
 */
struct NoteKey : public juce::Component
{
    NoteKey() = delete;
    explicit NoteKey(int index, Image* image);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;

    int noteIndex;
    bool isEdited = false;
    bool isOver = false;
    juce::Image* keyImage = nullptr;
    std::function<void(int index)> changeNote = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteKey)
};

/**
 * @brief This is the power on/off button on top of each note.
 */
struct NoteMappingToggle : public juce::Component
{
    NoteMappingToggle() = delete;
    explicit NoteMappingToggle(int index, NoteParam& noteParam, juce::Image* image);
    void resized() override;

    int noteIndex;
    juce::Image* powerImage = nullptr;
    std::unique_ptr<AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>> mapChoice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteMappingToggle)
};

/**
 * @brief The header contains all the note names + their transposition sliders.
 */ 
struct KeysPanel : public juce::Component
{
    KeysPanel() = delete;
    explicit KeysPanel(NoteParams& noteParams);
    void resized() override;

    juce::OwnedArray<NoteKey> noteKeys;
    juce::OwnedArray<NoteMappingToggle> noteMappingToggles;
    std::array<bool, 12> whiteNotes { true, false, true, false, true, true, false, true, false, true, false, true };

    juce::Image whiteKey { juce::ImageCache::getFromMemory(BinaryData::key_white_sprites_png, BinaryData::key_white_sprites_pngSize) };
    juce::Image blackKey { juce::ImageCache::getFromMemory(BinaryData::key_black_sprites_png, BinaryData::key_black_sprites_pngSize) };
    juce::Image powerImage { juce::ImageCache::getFromMemory(BinaryData::power_button_png, BinaryData::power_button_pngSize) };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeysPanel)
};
