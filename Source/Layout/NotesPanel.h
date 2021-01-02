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
    IntervalsChoice(int index, MidiBassPedalChordsAudioProcessor& processor) : noteIndex(index)
    {
        buttons.reserve(NB_NOTES);
        for (auto i = 0; i < NB_NOTES; i++)
        {
            auto button = new AttachedComponent<ToggleButton, ButtonParameterAttachment>(
                *processor.midiProcessor.noteParams.notes[noteIndex].intervals[i], *this,
                [this](ToggleButton& button) {
                    button.setColour(ToggleButton::tickColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
                    button.setColour(ToggleButton::tickDisabledColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
                }
            );
            buttons.emplace_back(button);
        }
    }

    void paint(Graphics&) override {}
    void resized() override
    {
        auto height = getLocalBounds().getHeight();
        auto button_width = getLocalBounds().getWidth() / float(NB_NOTES);
        for (size_t i = 0; i < buttons.size(); i++)
            buttons[i].get()->component.setBounds(button_width * i, 0, button_width, height);
    }

    int noteIndex;
    std::vector<std::unique_ptr<AttachedComponent<ToggleButton, ButtonParameterAttachment>>> buttons;
};

/**
 * This represents the parameters for a note :
 * - Its transposition
 * - The list of selected intervals, encapsulated in an IntervalsChoice component
 */
struct NoteLine : Component
{
    NoteLine(int index, MidiBassPedalChordsAudioProcessor& processor) 
        : noteIndex(index), noteLabel("noteLabel", Notes::labels[index]), intervalsChoice(index, processor)
    {
        noteLabel.setColour(Label::ColourIds::textColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
        addAndMakeVisible(noteLabel);

        transpose = std::make_unique< AttachedComponent<Slider, SliderParameterAttachment> >(
            *processor.midiProcessor.noteParams.notes[noteIndex].transpose, *this,
            [this](Slider& slider) {
                slider.setNormalisableRange({-12, 12, 1});
                slider.setColour(Slider::textBoxTextColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
                slider.setColour(Slider::textBoxOutlineColourId, Notes::whiteNotes[noteIndex] ? Colours::white : Colours::black);
                slider.setTooltip("Choose the number of semitones you want to transpose the note.");
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
        auto column_width = getLocalBounds().getWidth() / 10.0;
        noteLabel.setBounds(0, 0, column_width, height);
        transpose.get()->component.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, column_width / 2.0, height);
        transpose.get()->component.setBounds(column_width, 0, column_width * 3, height);
        intervalsChoice.setBounds(column_width * 4, 0, column_width * 6, height);
    }

    int noteIndex;
    Label noteLabel;
    std::unique_ptr<AttachedComponent<Slider, SliderParameterAttachment>> transpose;
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