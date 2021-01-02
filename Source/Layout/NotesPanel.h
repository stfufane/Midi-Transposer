#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../PluginProcessor.h"

constexpr auto NB_NOTES = 12;

/**
 * This will contain all the interval switches for a given note.
 */
struct IntervalsChoice : Component
{
    IntervalsChoice(int index, MidiBassPedalChordsAudioProcessor&) : noteIndex(index)
    {
        buttons.reserve(NB_NOTES);
        for (auto i = 0; i < NB_NOTES; i++)
        {
            auto button = new ToggleButton();
            buttons.emplace_back(button);
            button->setBounds(25 * i, 0, 25, 25);
            button->setColour(ToggleButton::tickColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
            button->setColour(ToggleButton::tickDisabledColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
            addAndMakeVisible(button);
        }
    }

    void paint(Graphics&) override {}
    void resized() override
    {
        auto height = getLocalBounds().getHeight();
        auto button_width = getLocalBounds().getWidth() / float(NB_NOTES);
        for (size_t i = 0; i < buttons.size(); i++)
            buttons[i]->setBounds(button_width * i, 0, button_width, height);
    }

    int noteIndex;
    std::vector<std::unique_ptr<ToggleButton>> buttons;
};

/**
 * This represents the parameters for a note :
 * - Its transposition
 * - The list of selected intervals, encapsulated in an IntervalsChoice component
 */
struct NoteLine : Component
{
    NoteLine(int index, MidiBassPedalChordsAudioProcessor& processor) : noteIndex(index), intervalsChoice(index, processor)
    {
        // Create the notes combo box
        noteChoice = std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
            *processor.midiProcessor.noteParams.notes[noteIndex].note, *this,
            [this](ComboBox& combo) {
                combo.addItemList(Notes::labels, noteIndex + 1);
            }
        );

        // Create the chords combo box
        chordChoice = std::make_unique< AttachedComponent<ComboBox, ComboBoxParameterAttachment> >(
            *processor.midiProcessor.noteParams.notes[noteIndex].chord, *this,
            [this](ComboBox& combo) {
                combo.addItemList(Notes::chords, noteIndex + 1);
            }
        );

        addAndMakeVisible(intervalsChoice);
    }

    void paint(Graphics& g) override
    {
        g.fillAll(Notes::getColour(noteIndex));
    }

    void resized() override
    {
        auto height = getLocalBounds().getHeight();
        auto column_width = getLocalBounds().getWidth() / 5.0;
        intervalsChoice.setBounds(column_width * 2, 0, column_width * 3, height);
        noteChoice->component.setBounds(0, 0, column_width, height);
        chordChoice->component.setBounds(column_width, 0, column_width, height);
    }

    int noteIndex;
    std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> > noteChoice;
    std::unique_ptr< AttachedComponent<ComboBox, ComboBoxParameterAttachment> > chordChoice;
    IntervalsChoice intervalsChoice;
};

/**
 * A panel that will contain the lines for each midi note.
 */
struct NotesPanel : public Component
{
    NotesPanel(MidiBassPedalChordsAudioProcessor& processor)
    {
        noteLines.reserve(NB_NOTES);
        for (int i = 0; i < NB_NOTES; i++)
        {
            auto noteLine = new NoteLine(i, processor);
            noteLines.emplace_back(noteLine);
            addAndMakeVisible(noteLine);
        }
    }

    void paint(Graphics& g) override
    {
        g.fillAll(juce::Colours::beige);
    }

    void resized() override
    {
        auto line_height = getLocalBounds().getHeight() / float(NB_NOTES);
        for (auto& noteLine: noteLines)
            noteLine->setBounds(0, line_height * noteLine->noteIndex, getLocalBounds().getWidth(), line_height);
    }

    std::vector<std::unique_ptr<NoteLine>> noteLines;
};