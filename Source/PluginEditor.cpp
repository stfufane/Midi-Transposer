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
    for (int c = 1; c <= 16; c++) {
        inputChannelChoice.addItem(std::to_string(c), c);
        outputChannelChoice.addItem(std::to_string(c), c);
    }

    inputChannelChoice.setText("Input Channel");
    inputChannelChoice.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
    inputChannelChoice.setBounds(518, 22, COMBO_WIDTH, COMBO_HEIGHT);
    addAndMakeVisible(inputChannelChoice);
    inputChannelAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, IDs::paramInChannel, inputChannelChoice));

    outputChannelChoice.setText("Output Channel");
    outputChannelChoice.setTooltip("The transposed events will be routed to this channel.");
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
    
    Image background = ImageCache::getFromMemory(BinaryData::pk5a_jpg, BinaryData::pk5a_jpgSize);
    g.drawImage(background, 0, 0, WINDOW_WIDTH, BACKGROUND_HEIGHT, 0, 0, background.getWidth(), background.getHeight());
    
    g.setColour(Colours::white);
    g.fillRoundedRectangle(383.0f, 11.0f, 409.0f, 75.0f, 2.0f);
    
    g.setColour(Colours::black);
    g.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
    g.drawText("Input Midi Channel", 390, 22, 130, 24, Justification::left, true);

    g.setColour(Colours::black);
    g.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
    g.drawText("Output Midi Channel", 390, 54, 130, 24, Justification::left, true);
}

void MidiTransposerAudioProcessorEditor::resized() {}
