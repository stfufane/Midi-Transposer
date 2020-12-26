#include "PluginEditor.h"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 680;
constexpr auto BACKGROUND_HEIGHT = 612;
constexpr auto COMBO_WIDTH = 65;
constexpr auto NOTE_COMBO_WIDTH = 85;
constexpr auto COMBO_HEIGHT = 24;

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    inputChannel = std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
        *processor.midiProcessor.midiParams.inputChannel, *this,
        [](ComboBox& combo) {
            for (int channel = 1; channel <= 16; channel++) {
                combo.addItem(String(channel), channel);
            }
            combo.setBounds(425, 18, COMBO_WIDTH, COMBO_HEIGHT);
            combo.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
        }
    );

    outputChannel = std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
        *processor.midiProcessor.midiParams.outputChannel, *this,
        [](ComboBox& combo) {
            for (int channel = 1; channel <= 16; channel++) {
                combo.addItem(String(channel), channel);
            }
            combo.setBounds(425, 50, COMBO_WIDTH, COMBO_HEIGHT);
            combo.setTooltip("The transposed events will be routed to this channel.");
        }
    );

    octaveTranspose = std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
        *processor.midiProcessor.midiParams.octaveTranspose, *this,
        [](ComboBox& combo) {
            for (int octave = 1; octave <= 6; octave++) {
                combo.addItem(String(octave - 2), octave);
            }
            combo.setTooltip("This will play the root note at its original position and transpose the chord.");
            combo.setBounds(630, 18, COMBO_WIDTH, COMBO_HEIGHT);
        }
    );

    bypassChannels = std::make_unique< AttachedComponent<ToggleButton, ButtonParameterAttachment> >(
        *processor.midiProcessor.midiParams.bypassOtherChannels, *this,
        [](ToggleButton& button) {
            button.setButtonText("Bypass other channels");
            button.setBounds(500, 50, 150, 24);
            button.setColour(ToggleButton::textColourId, Colours::black);
            button.setColour(ToggleButton::tickColourId, Colours::black);
            button.setColour(ToggleButton::tickDisabledColourId, Colours::black);
            button.changeWidthToFitText();
        }
    );

    for (int i = 0; i < Names::notes.size(); i++)
    {
        // Create the notes combo box
        noteChoices.push_back(std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
            *processor.midiProcessor.noteParams.notes[i].note, *this,
            [&i, this](ComboBox& combo) {
                combo.addItemList(Names::noteLabels, i + 1);
                combo.setBounds(keyPositions[i].combo_x, keyPositions[i].combo_note_y, NOTE_COMBO_WIDTH, COMBO_HEIGHT);
            }
        ));

        // Create the chords combo box
        chordChoices.push_back(std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
            *processor.midiProcessor.noteParams.notes[i].chord, *this,
            [&i, this](ComboBox& combo) {
                combo.addItemList(Names::chords, i + 1);
                combo.setBounds(keyPositions[i].combo_x, keyPositions[i].combo_chord_y, NOTE_COMBO_WIDTH, COMBO_HEIGHT);
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
    int lastNotePlayed = processor.midiProcessor.getLastNoteOn() % 12;
    if (lastNotePlayed != currentNotePlayed) {
        repaint();
        currentNotePlayed = lastNotePlayed;
    }
}