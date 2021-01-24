#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../Params/Params.h"

constexpr auto NB_NOTES = 12;
constexpr auto NB_INTERVALS = 12;

using Track = Grid::TrackInfo;
using Fr = Grid::Fr;

/**
 * The header with MIDI params
 */
struct HeaderPanel : public Component
{
    HeaderPanel(MidiParams& midiParams);

    void paint(Graphics& g) override;
    void resized() override;

    Image background { ImageCache::getFromMemory(BinaryData::header_jpg, BinaryData::header_jpgSize) };

    std::unique_ptr< AttachedComponent<RotarySlider, SliderParameterAttachment> > inputChannel;
    std::unique_ptr< AttachedComponent<RotarySlider, SliderParameterAttachment> > outputChannel;
    std::unique_ptr< AttachedComponent<RotarySlider, SliderParameterAttachment> > octaveTranspose;
};

/**
 * This panel holds the 12 interval buttons for a note + a semitone transposition slider.
 */
struct IntervalsPanel : public Component
{
    IntervalsPanel(NoteParam& noteParam);
    void paint(Graphics& g) override;
    void resized() override;

    Image buttonsImage { ImageCache::getFromMemory(BinaryData::buttons_png, BinaryData::buttons_pngSize) };
    std::unique_ptr<AttachedComponent<RotarySlider, SliderParameterAttachment>> transpose;
    std::vector<std::unique_ptr<AttachedComponent<IndexedToggleButton, ButtonParameterAttachment>>> intervalsChoices;
};

/**
 * The drawing of a key with an event to display its intervals.
 */
struct NoteKey : public Component
{
    NoteKey(int index, Image* image);

    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent&) override;
    void mouseEnter(const MouseEvent&) override;
    void mouseExit(const MouseEvent&) override;

    int noteIndex;
    bool isEdited = false;
    bool isOver = false;
    Image* keyImage = nullptr;
    std::function<void(int index)> changeNote = nullptr;
};

/**
 * This is the power on/off button on top of each note.
 */
struct NoteMappingToggle : public Component
{
    NoteMappingToggle(int index, NoteParam& noteParam, Image* image);
    void resized() override;

    int noteIndex;
    Image* powerImage = nullptr;
    std::unique_ptr<AttachedComponent<IndexedToggleButton, ButtonParameterAttachment>> mapChoice;
};

/**
 * The header contains all the note names + their transposition sliders.
 */ 
struct KeysPanel : public Component
{
    KeysPanel(NoteParams& noteParams);
    void resized() override;

    OwnedArray<NoteKey> noteKeys;
    OwnedArray<NoteMappingToggle> noteMappingToggles;
    std::array<bool, 12> whiteNotes { true, false, true, false, true, true, false, true, false, true, false, true };

    Image whiteKey { ImageCache::getFromMemory(BinaryData::key_white_sprites_png, BinaryData::key_white_sprites_pngSize) };
    Image blackKey { ImageCache::getFromMemory(BinaryData::key_black_sprites_png, BinaryData::key_black_sprites_pngSize) };
    Image powerImage { ImageCache::getFromMemory(BinaryData::power_button_png, BinaryData::power_button_pngSize) };
};
