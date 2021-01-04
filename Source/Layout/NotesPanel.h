#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../PluginProcessor.h"

constexpr auto NB_NOTES = 12;

/**
 * This will contain all the interval switches for a given note.
 */
struct IntervalsChoice : public Component
{
    IntervalsChoice(int index, MidiBassPedalChordsAudioProcessor& processor) : noteIndex(index)
    {
        buttons.reserve(NB_NOTES);
        for (auto i = 0; i < NB_NOTES; i++)
        {
            auto button = new AttachedComponent<TextButton, ButtonParameterAttachment>(
                *processor.midiProcessor.noteParams.notes[noteIndex]->intervals[i]->interval, *this,
                [&i](TextButton& button) {
                    button.setButtonText(Notes::intervals[i]);
                    button.setClickingTogglesState(true);
                }
            );
            buttons.emplace_back(button);
        }
    }

    void paint(Graphics&) override {}
    void resized() override
    {
        auto width = getLocalBounds().getWidth() * 0.8;
        auto x = width * 0.1;
        auto height = getLocalBounds().getHeight() / float(NB_NOTES);
        for (size_t i = 0; i < buttons.size(); i++)
            buttons[i].get()->component.setBounds(x, height * i, width, height);
    }

    int noteIndex;
    std::vector<std::unique_ptr<AttachedComponent<TextButton, ButtonParameterAttachment>>> buttons;
};

struct IntervalsPanel : public Component
{
    IntervalsPanel(MidiBassPedalChordsAudioProcessor& processor)
    {
        intervalsChoices.reserve(NB_NOTES);
        for (auto i = 0; i < NB_NOTES; i++)
        {
            auto intervalsChoice = new IntervalsChoice(i, processor);
            intervalsChoices.emplace_back(intervalsChoice);
            addAndMakeVisible(intervalsChoice);
        }
    }

    void paint(Graphics& g) override 
    {
        g.fillAll(juce::Colours::white);
    }

    void resized() override
    {
        auto width = getLocalBounds().getWidth() / float(NB_NOTES);
        auto height = getLocalBounds().getHeight();
        for (auto& intervalsChoice: intervalsChoices)
            intervalsChoice->setBounds(width * intervalsChoice->noteIndex, 0, width, height);
    }

    std::vector<std::unique_ptr<IntervalsChoice>> intervalsChoices;
};

/**
 * This represents the parameters for a note :
 * - A label
 * - Its transposition
 * - A combobox with some predetermined chords (TODO)
 */
struct NotesHeader : public Component
{
    NotesHeader(int index, MidiBassPedalChordsAudioProcessor& processor) 
        : noteIndex(index), noteLabel("noteLabel", Notes::labels[index])
    {
        noteLabel.setColour(Label::ColourIds::textColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
        noteLabel.setJustificationType(Justification::centredTop);
        addAndMakeVisible(noteLabel);

        chordChoice.addItemList({"maj", "min", "maj7"}, 1);
        addAndMakeVisible(chordChoice);

        transpose = std::make_unique< AttachedComponent<SemitoneSlider, SliderParameterAttachment> >(
            *processor.midiProcessor.noteParams.notes[noteIndex]->transpose, *this,
            [this](SemitoneSlider& slider) {
                slider.setNormalisableRange({-12, 12, 1});
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setColour(Slider::textBoxTextColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
                slider.setColour(Slider::textBoxOutlineColourId, Notes::whiteNotes[noteIndex] ? Colours::white : Colours::black);
                slider.setTooltip("Choose the number of semitones you want to transpose the note.");
            }
        );
    }

    void paint(Graphics& g) override
    {
        g.fillAll(Notes::getColour(noteIndex));
    }

    void resized() override
    {
        Grid grid;
        using Track = Grid::TrackInfo;
        using Fr = Grid::Fr;
 
        grid.templateRows       = { Track(Fr(1)), Track(Fr(3)), Track(Fr(1)) };
        grid.templateColumns    = { Track(Fr(1)) };

        grid.items = { GridItem(noteLabel), GridItem(transpose.get()->component), GridItem(chordChoice) };

        grid.performLayout (getLocalBounds());

        // Resize the textbox.
        auto& slider = transpose.get()->component;
        auto slider_width = slider.getLocalBounds().getWidth();
        auto slider_height = slider.getLocalBounds().getHeight();
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, slider_width, slider_height / 3.0);
    }

    int noteIndex;
    Label noteLabel;
    std::unique_ptr<AttachedComponent<SemitoneSlider, SliderParameterAttachment>> transpose;
    ComboBox chordChoice;
};

/**
 * The header contains all the note names + their transposition sliders.
 */ 
struct PanelHeader : public Component
{
    PanelHeader(MidiBassPedalChordsAudioProcessor& processor)
    {
        notesHeaders.reserve(NB_NOTES);
        for (int i = 0; i < NB_NOTES; i++)
        {
            auto noteHeader = new NotesHeader(i, processor);
            notesHeaders.emplace_back(noteHeader);
            addAndMakeVisible(noteHeader);
        }
    }

    void paint(Graphics&) override { }

    void resized() override
    {
        auto width = getLocalBounds().getWidth() / float(NB_NOTES);
        auto height = getLocalBounds().getHeight();
        for (auto& noteHeader: notesHeaders)
            noteHeader->setBounds(width * noteHeader->noteIndex, 0, width, height);
        
    }

    std::vector<std::unique_ptr<NotesHeader>> notesHeaders;
};

/**
 * A panel that will contain the differnet parts of the notes configuration.
 */
struct NotesPanel : public Component
{
    NotesPanel(MidiBassPedalChordsAudioProcessor& processor) : panelHeader(processor), intervalsPanel(processor)
    {
        addAndMakeVisible(panelHeader);
        addAndMakeVisible(intervalsPanel);
    }

    void paint(Graphics&) override { }

    void resized() override
    {
        Grid grid;
        using Track = Grid::TrackInfo;
        using Fr = Grid::Fr;
 
        grid.templateColumns    = { Track (Fr (1)) };
        grid.templateRows       = { Track (Fr (2)), Track (Fr (8)) };

        grid.items = { GridItem(panelHeader), GridItem(intervalsPanel) };

        grid.performLayout (getLocalBounds());
    }

    PanelHeader panelHeader;
    IntervalsPanel intervalsPanel;
};
