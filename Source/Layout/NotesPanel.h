#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "../PluginProcessor.h"

constexpr auto NB_NOTES = 12;
constexpr auto NB_INTERVALS = 12;

using Track = Grid::TrackInfo;
using Fr = Grid::Fr;

/**
 * This panel holds the 12 interval buttons for a note + a semitone transposition slider.
 */
struct IntervalsPanel : public Component
{
    IntervalsPanel(NoteParam& noteParam)
    {
        intervalsChoices.reserve(NB_INTERVALS);
        for (auto i = 0; i < NB_INTERVALS; i++)
        {
            intervalsChoices.emplace_back(new AttachedComponent<TextButton, ButtonParameterAttachment>(
                *noteParam.intervals[i]->interval, *this,
                [&i](TextButton& button) {
                    button.setButtonText(Notes::intervals[i]);
                    button.setClickingTogglesState(true);
                }
            ));
        }

        transpose = std::make_unique< AttachedComponent<NumericSlider, SliderParameterAttachment> >(
            *noteParam.transpose, *this,
            [](NumericSlider& slider) {
                slider.setUnity("semitone");
                slider.setNormalisableRange({-12, 12, 1});
                slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setTooltip("Choose the number of semitones you want to transpose the note.");
            }
        );
        //resized();
    }

    void paint(Graphics&) override {}

    void resized() override
    {
        Grid grid;

        grid.templateColumns    = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
        grid.templateRows       = { Track(Fr(1)), Track(Fr(1)) };
        grid.alignItems         = Grid::AlignItems::center;
        grid.alignContent       = Grid::AlignContent::center;
        grid.setGap(Grid::Px(10));

        for (auto i: { 1, 3, 4, 5, 6, 8, 10, 11, 0, 2, -1, -1, -1, 7, 9, -1}) {
            if (i > -1) {
                grid.items.add(intervalsChoices[i].get()->component);
            } else {
                grid.items.add(nullptr);
            }
        }

        auto totalBounds = getLocalBounds();
        grid.performLayout (getLocalBounds().reduced(10, 40));

        // Center the transpose slider manually;
        auto width = totalBounds.getWidth();
        auto height = totalBounds.getHeight();
        auto slider_x = width * 5.0 / 16.0;
        auto slider_y = height / 2.0;
        auto slider_width = width / 4.0;
        auto slider_height = height / 2.0;

        // Resize the textbox.
        auto& slider = transpose.get()->component;
        slider.setBounds(Rectangle<int>(slider_x, slider_y, slider_width, slider_height).reduced(0, 20));
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);
    }

    std::unique_ptr<AttachedComponent<NumericSlider, SliderParameterAttachment>> transpose;
    std::vector<std::unique_ptr<AttachedComponent<TextButton, ButtonParameterAttachment>>> intervalsChoices;
};

/**
 * This represents the parameters for a note :
 * - A label
 * - Its transposition
 * - A combobox with some predetermined chords (TODO)
 */
struct NotesHeader : public Component
{
    NotesHeader(int index) 
        : noteIndex(index), noteLabel("noteLabel", Notes::labels[index])
    {
        addMouseListener(this, true);
        noteLabel.setColour(Label::ColourIds::textColourId, Notes::whiteNotes[noteIndex] ? Colours::black : Colours::white);
        noteLabel.setJustificationType(Justification::centredTop);
        addAndMakeVisible(noteLabel);
    }

    void paint(Graphics& g) override
    {
        g.fillAll(Notes::getColour(noteIndex));
        if (isEdited)
        {
            g.setColour (juce::Colours::red);
            auto size = 10;
            g.fillEllipse (getLocalBounds().getWidth() / 2.0 - size / 2.0, getLocalBounds().getHeight() - size * 2.0, size, size);
        }
    }

    void resized() override {
        noteLabel.setBounds(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    }

    void mouseDown(const MouseEvent&) override {
        if (changeNote != nullptr)
            changeNote(noteIndex);
    }

    int noteIndex;
    bool isEdited = false;
    Label noteLabel;
    std::function<void(int index)> changeNote = nullptr;
};

/**
 * The header contains all the note names + their transposition sliders.
 */ 
struct PanelHeader : public Component
{
    PanelHeader()
    {
        for (int i = 0; i < NB_NOTES; i++)
        {
            auto noteHeader = new NotesHeader(i);
            notesHeaders.add(noteHeader);
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

    OwnedArray<NotesHeader> notesHeaders;
};

/**
 * A panel that will contain the differnet parts of the notes configuration.
 */
struct NotesPanel : public Component
{
    NotesPanel(NoteParams& noteParams, UISettings& uiSettings)
    {
        auto index = uiSettings.lastNoteIndex;
        auto noteParam = noteParams.notes[index].get();
        addAndMakeVisible(panelHeader);
        initIntervalsPanel(*noteParam);
        updateNoteEdited(index);
        for (auto& noteHeader: panelHeader.notesHeaders) 
        {
            noteHeader->changeNote = [this, &noteParams, &uiSettings] (int index) 
            { 
                auto noteParam = noteParams.notes[index].get();
                initIntervalsPanel(*noteParam);
                updateNoteEdited(index);
                uiSettings.lastNoteIndex = index;
            };
        }
    }

    void initIntervalsPanel(NoteParam& noteParam)
    {
        intervalsPanel.reset(new IntervalsPanel(noteParam));
        addAndMakeVisible(intervalsPanel.get());
        resized();
    }

    void updateNoteEdited(int index)
    {
        for (auto& noteHeader: panelHeader.notesHeaders) 
        {
            noteHeader->isEdited = (noteHeader->noteIndex == index);
        }
        repaint();
    }

    void paint(Graphics&) override { }

    void resized() override
    {
        Grid grid;
 
        grid.templateColumns    = { Track(Fr(1)) };
        grid.templateRows       = { Track(Fr(1)), Track(Fr(2)) };

        grid.items = { GridItem(panelHeader).withMargin(GridItem::Margin(15)), GridItem(intervalsPanel.get()) };

        grid.performLayout (getLocalBounds());
    }

    PanelHeader panelHeader;
    std::unique_ptr<IntervalsPanel> intervalsPanel = nullptr;
};
