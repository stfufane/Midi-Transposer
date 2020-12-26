#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct KeyPosition {
    Image* keyImage;
    int x;
    int y;
    int combo_x;
    int combo_note_y;
    int combo_chord_y;
};

// Each GUI component linked to a parameter is created as an AttachedComponent to manage the attachment in the same object.
// It's automatically made visible when it's initialised. The init parameter function can be used to define the component properties.
template <typename CompType, typename CompAttachment>
class AttachedComponent
{
public:
    AttachedComponent<CompType, CompAttachment>(RangedAudioParameter& param, AudioProcessorEditor& editor, std::function<void(CompType&)> init = nullptr)
    {
        if (init != nullptr) init(component);
        attachment.reset(new CompAttachment(param, component));
        editor.addAndMakeVisible(component);
        attachment->sendInitialUpdate();
    }
private:
    CompType component;
    std::unique_ptr<CompAttachment> attachment;
};

//==============================================================================
/**
*/
class MidiBassPedalChordsAudioProcessorEditor : public AudioProcessorEditor, private Timer
{
public:
    MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor&);
    ~MidiBassPedalChordsAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    MidiBassPedalChordsAudioProcessor& processor;

    Image backgroundImage{ ImageCache::getFromMemory(BinaryData::pk5a_jpg, BinaryData::pk5a_jpgSize) };
    Image whiteKey{ ImageCache::getFromMemory(BinaryData::white_key_png, BinaryData::white_key_pngSize) };
    Image blackKey{ ImageCache::getFromMemory(BinaryData::black_key_png, BinaryData::black_key_pngSize) };

    int currentNotePlayed{ -1 };

    // This vector defines which image is used for each key, 
    // the coordinates of the image and the coordinates of the combobox components.
    const std::vector<KeyPosition> keyPositions {
        { &whiteKey, 41,  401,  35, 616, 648 }, // C
        { &blackKey, 84,  297,  82, 446, 478 }, // C#
        { &whiteKey, 133, 401, 131, 616, 648 }, // D
        { &blackKey, 177, 297, 178, 446, 478 }, // D#
        { &whiteKey, 223, 401, 225, 616, 648 }, // E
        { &whiteKey, 313, 401, 316, 616, 648 }, // F
        { &blackKey, 359, 297, 358, 446, 478 }, // F#
        { &whiteKey, 403, 401, 405, 616, 648 }, // G
        { &blackKey, 449, 297, 449, 446, 478 }, // G#
        { &whiteKey, 494, 401, 498, 616, 648 }, // A
        { &blackKey, 539, 297, 542, 446, 478 }, // A#
        { &whiteKey, 587, 401, 593, 616, 648 }  // B
    };

    std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >   inputChannel;
    std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >   outputChannel;
    std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >   octaveTranspose;
    std::unique_ptr< AttachedComponent<ToggleButton, ButtonParameterAttachment> > bypassChannels;

    std::vector< std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> > > noteChoices;
    std::vector< std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> > > chordChoices;

    TooltipWindow tooltipWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBassPedalChordsAudioProcessorEditor)
};
