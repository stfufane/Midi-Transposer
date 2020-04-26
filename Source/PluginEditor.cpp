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
MidiTransposerAudioProcessorEditor::MidiTransposerAudioProcessorEditor(MidiTransposerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts)
{
    inputChannelChoice.setText("Input Channel");
    inputChannelChoice.addItemList(MidiProcessor::midiChannels, 1);
    inputChannelChoice.setBounds(518, 22, COMBO_WIDTH, COMBO_HEIGHT);
    addAndMakeVisible(inputChannelChoice);
    inputChannelAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, IDs::paramInChannel, inputChannelChoice));

    outputChannelChoice.setText("Output Channel");
    outputChannelChoice.addItemList(MidiProcessor::midiChannels, 1);
    outputChannelChoice.setBounds(518, 54, COMBO_WIDTH, COMBO_HEIGHT);
    addAndMakeVisible(outputChannelChoice);
    outputChannelAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, IDs::paramOutChannel, outputChannelChoice));

    std::vector<int> combo_x{ 47, 94, 138, 185, 232, 323, 365, 412, 455, 503, 547, 595 };
    std::vector<int> combo_note_y{ 616, 446, 616, 446, 616, 616, 446, 616, 446, 616, 446, 616 };
    std::vector<int> combo_chord_y{ 648, 478, 648, 478, 648, 648, 478, 648, 478, 648, 478, 648 };

    String note_id, chord_id;
    for (int i = 0; i < MidiProcessor::notes.size(); i++)
    {
        // Create the notes combo box
        note_id = MidiProcessor::notes[i] + "_note";
        noteChoices.push_back(std::make_unique<ComboBox>(note_id));
        noteChoices.back()->setText(MidiProcessor::notes[i] + " Transpose");
        noteChoices.back()->addItemList(MidiProcessor::notes, i + 1);
        noteChoices.back()->setBounds(combo_x[i], combo_note_y[i], COMBO_WIDTH, COMBO_HEIGHT);
        addAndMakeVisible(noteChoices.back().get());
        // Attach the combobox to the tree state
        noteChoicesAttachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, note_id, *noteChoices.back())
        );

        // Create the chords combo box
        chord_id = MidiProcessor::notes[i] + "_chord";
        chordChoices.push_back(std::make_unique<ComboBox>(chord_id));
        chordChoices.back()->setText(MidiProcessor::notes[i] + " Chord");
        chordChoices.back()->addItemList(MidiProcessor::chords, i + 1);
        chordChoices.back()->setBounds(combo_x[i], combo_chord_y[i], COMBO_WIDTH, COMBO_HEIGHT);
        addAndMakeVisible(chordChoices.back().get());
        // Attach the combobox to the tree state
        chordChoicesAttachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, chord_id, *chordChoices.back())
        );
    }

    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setResizable(false, false);
}

MidiTransposerAudioProcessorEditor::~MidiTransposerAudioProcessorEditor() { }

//==============================================================================
void MidiTransposerAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colour(0xffffffff));

    {
        int x = 0, y = 0;
        Image background = ImageCache::getFromMemory(BinaryData::pk5a_jpg, BinaryData::pk5a_jpgSize);
        g.drawImage(background, x, y, WINDOW_WIDTH, BACKGROUND_HEIGHT, x, y, background.getWidth(), background.getHeight());
    }

    {
        float x = 383.0f, y = 11.0f, width = 409.0f, height = 75.0f;
        Colour fillColour = Colours::white;
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        int x = 385, y = 22, width = 130, height = 24;
        String text(TRANS("Old Midi Channel"));
        Colour fillColour = Colours::black;
        g.setColour(fillColour);
        g.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, Justification::centred, true);
    }

    {
        int x = 385, y = 54, width = 130, height = 24;
        String text(TRANS("New Midi Channel"));
        Colour fillColour = Colours::black;
        g.setColour(fillColour);
        g.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, Justification::centred, true);
    }
}

void MidiTransposerAudioProcessorEditor::resized() {}
