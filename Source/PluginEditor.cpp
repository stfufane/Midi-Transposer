/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiProcessor.h"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 680;
constexpr auto BACKGROUND_HEIGHT = 612;
constexpr auto COMBO_WIDTH = 65;
constexpr auto COMBO_HEIGHT = 24;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts),
    inputChannel("in_channel", "Only the events coming from this channel will be transposed. The rest will pass through.", vts),
    outputChannel("out_channel", "The transposed events will be routed to this channel.", vts)
{
    backgroundImage = ImageCache::getFromMemory(BinaryData::pk5a_jpg, BinaryData::pk5a_jpgSize);
    whiteKey = ImageCache::getFromMemory(BinaryData::white_key_png, BinaryData::white_key_pngSize);
    blackKey = ImageCache::getFromMemory(BinaryData::black_key_png, BinaryData::black_key_pngSize);

    for (int c = 1; c <= 16; c++) {
        inputChannel.component.addItem(String(c), c);
        outputChannel.component.addItem(String(c), c);
    }

    inputChannel.component.setBounds(425, 18, COMBO_WIDTH, COMBO_HEIGHT);
    outputChannel.component.setBounds(425, 50, COMBO_WIDTH, COMBO_HEIGHT);

    bypassChannels.setButtonText("Bypass other channels");
    bypassChannels.setBounds(500, 50, 150, 24);
    bypassChannels.setColour(ToggleButton::textColourId, Colours::black);
    bypassChannels.setColour(ToggleButton::tickColourId, Colours::black);
    bypassChannels.setColour(ToggleButton::tickDisabledColourId, Colours::black);
    addAndMakeVisible(bypassChannels);
    bypassChannels.changeWidthToFitText();
    bypassChannelsAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "bypass_other_channels", bypassChannels));

    for (int o = 1; o <= 6; o++) {
        octaveTransposeChoice.addItem(String(o - 2), o);
    }
    octaveTransposeChoice.setTooltip("This will play the root note at its original position and transpose the chord.");
    octaveTransposeChoice.setBounds(630, 18, COMBO_WIDTH, COMBO_HEIGHT);
    addAndMakeVisible(octaveTransposeChoice);
    octraveTransposeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "octave_transpose", octaveTransposeChoice));

    std::vector<int> combo_x{ 47, 94, 138, 185, 232, 323, 365, 412, 455, 503, 547, 595 };
    std::vector<int> combo_note_y{ 616, 446, 616, 446, 616, 616, 446, 616, 446, 616, 446, 616 };
    std::vector<int> combo_chord_y{ 648, 478, 648, 478, 648, 648, 478, 648, 478, 648, 478, 648 };

    for (int i = 0; i < processor.getMidiProcessor().getNotes().size(); i++)
    {
        // Create the notes combo box
        String note_id = processor.getMidiProcessor().getNotes()[i] + "_note";
        noteChoices.push_back(std::make_unique<ComboBox>(note_id));
        noteChoices.back()->addItemList(processor.getMidiProcessor().getNotes(), i + 1);
        noteChoices.back()->setBounds(combo_x[i], combo_note_y[i], COMBO_WIDTH, COMBO_HEIGHT);
        addAndMakeVisible(noteChoices.back().get());
        // Attach the combobox to the tree state
        noteChoicesAttachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, note_id, *noteChoices.back())
        );

        // Create the chords combo box
        String chord_id = processor.getMidiProcessor().getNotes()[i] + "_chord";
        chordChoices.push_back(std::make_unique<ComboBox>(chord_id));
        chordChoices.back()->addItemList(processor.getMidiProcessor().getChords(), i + 1);
        chordChoices.back()->setBounds(combo_x[i], combo_chord_y[i], COMBO_WIDTH, COMBO_HEIGHT);
        addAndMakeVisible(chordChoices.back().get());
        // Attach the combobox to the tree state
        chordChoicesAttachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, chord_id, *chordChoices.back())
        );
    }

    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setResizable(false, false);

    startTimerHz(20);
}

MidiBassPedalChordsAudioProcessorEditor::~MidiBassPedalChordsAudioProcessorEditor() { }

//==============================================================================
void MidiBassPedalChordsAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colour(0xffffffff));
    
    g.drawImage(backgroundImage, 0, 0, WINDOW_WIDTH, BACKGROUND_HEIGHT, 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());

    if (currentNotePlayed > -1)
    {
        g.drawImageAt(*keyPositions[currentNotePlayed].keyImage, keyPositions[currentNotePlayed].x, keyPositions[currentNotePlayed].y, false);
    }
    
    g.setColour(Colours::white);
    g.fillRoundedRectangle(278.0f, 9.0f, 515.0f, 71.0f, 2.0f);
    
    g.setColour(Colours::black);
    g.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
    g.drawText("Input Midi Channel", 290, 18, 130, 24, Justification::left, true);
    g.drawText("Output Midi Channel", 290, 50, 130, 24, Justification::left, true);
    g.drawText("Octave transpose", 500, 18, 120, 24, Justification::left, true);
}

void MidiBassPedalChordsAudioProcessorEditor::resized() {}

void MidiBassPedalChordsAudioProcessorEditor::timerCallback()
{
    int lastNotePlayed = processor.getCurrentNotePlayed() % 12;
    if (lastNotePlayed != currentNotePlayed) {
        repaint();
        currentNotePlayed = lastNotePlayed;
    }
}