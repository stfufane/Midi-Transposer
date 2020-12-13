#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiProcessor.h"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 680;
constexpr auto BACKGROUND_HEIGHT = 612;
constexpr auto COMBO_WIDTH = 65;
constexpr auto COMBO_HEIGHT = 24;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p, APVTS& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts)
{
    inputChannel = std::make_unique< AttachedComponent<ComboBox, APVTS::ComboBoxAttachment> >(
        ParamIDs::inChannel, *this, valueTreeState,
        [](ComboBox& combo) {
            for (int channel = 1; channel <= 16; channel++) {
                combo.addItem(String(channel), channel);
            }
            combo.setBounds(425, 18, COMBO_WIDTH, COMBO_HEIGHT);
            combo.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
        }
    );

    outputChannel = std::make_unique< AttachedComponent<ComboBox, APVTS::ComboBoxAttachment> >(
        ParamIDs::outChannel, *this, valueTreeState,
        [](ComboBox& combo) {
            for (int channel = 1; channel <= 16; channel++) {
                combo.addItem(String(channel), channel);
            }
            combo.setBounds(425, 50, COMBO_WIDTH, COMBO_HEIGHT);
            combo.setTooltip("The transposed events will be routed to this channel.");
        }
    );

    octaveTranspose = std::make_unique< AttachedComponent<ComboBox, APVTS::ComboBoxAttachment> >(
        ParamIDs::octaveTranspose, *this, valueTreeState,
        [](ComboBox& combo) {
            for (int octave = 1; octave <= 6; octave++) {
                combo.addItem(String(octave - 2), octave);
            }
            combo.setTooltip("This will play the root note at its original position and transpose the chord.");
            combo.setBounds(630, 18, COMBO_WIDTH, COMBO_HEIGHT);
        }
    );

    bypassChannels = std::make_unique< AttachedComponent<ToggleButton, APVTS::ButtonAttachment> >(
        ParamIDs::bypassChannels, *this, valueTreeState,
        [](ToggleButton& button) {
            button.setButtonText("Bypass other channels");
            button.setBounds(500, 50, 150, 24);
            button.setColour(ToggleButton::textColourId, Colours::black);
            button.setColour(ToggleButton::tickColourId, Colours::black);
            button.setColour(ToggleButton::tickDisabledColourId, Colours::black);
            button.changeWidthToFitText();
        }
    );

    auto& noteNames = processor.getMidiProcessor().getNoteNames();
    auto& chordNames = processor.getMidiProcessor().getChordNames();
    for (int i = 0; i < noteNames.size(); i++)
    {
        // Create the notes combo box
        String note_id = processor.getMidiProcessor().getNoteNames()[i] + ParamIDs::noteChoice;
        noteChoices.push_back(std::make_unique< AttachedComponent<ComboBox, APVTS::ComboBoxAttachment> >(
            note_id, *this, valueTreeState,
            [&i, &noteNames, this](ComboBox& combo) {
                combo.addItemList(noteNames, i + 1);
                combo.setBounds(keyPositions[i].combo_x, keyPositions[i].combo_note_y, COMBO_WIDTH, COMBO_HEIGHT);
            }
        ));

        // Create the chords combo box
        String chord_id = processor.getMidiProcessor().getNoteNames()[i] + ParamIDs::chordChoice;
        chordChoices.push_back(std::make_unique< AttachedComponent<ComboBox, APVTS::ComboBoxAttachment> >(
            chord_id, *this, valueTreeState,
            [&i, &chordNames, this](ComboBox& combo) {
                combo.addItemList(chordNames, i + 1);
                combo.setBounds(keyPositions[i].combo_x, keyPositions[i].combo_chord_y, COMBO_WIDTH, COMBO_HEIGHT);
            }
        ));
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

    // Draw an overlay on the note that is currently played. It's a transparent image with a glow to highlight the key.
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