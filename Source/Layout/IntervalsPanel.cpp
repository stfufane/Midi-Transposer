#include "Panels.h"

IntervalsPanel::IntervalsPanel(NoteParam& noteParam)
{
    intervalsChoices.reserve(NB_INTERVALS);
    for (auto i = 0; i < NB_INTERVALS; i++)
    {
        intervalsChoices.emplace_back(new AttachedComponent<IndexedToggleButton, ButtonParameterAttachment>(
            *noteParam.intervals[i]->interval, *this, 
            [this, &i](IndexedToggleButton& button) {
                button.setImage(&buttonsImage, i, NB_INTERVALS);
            }
        ));
    }

    transpose = std::make_unique< AttachedComponent<RotarySlider, SliderParameterAttachment> >(
        *noteParam.transpose, *this,
        [](RotarySlider& slider) {
            slider.setNormalisableRange({-12, 12, 1});
            slider.setTooltip("Choose the number of semitones you want to transpose the note.");
        }
    );
}

void IntervalsPanel::paint(Graphics& g)
{
    g.fillAll(Colours::black);
}

void IntervalsPanel::resized()
{
    Grid grid;

    grid.templateColumns    = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
    grid.templateRows       = { Track(Fr(1)), Track(Fr(1)) };
    grid.alignItems         = Grid::AlignItems::center;
    grid.alignContent       = Grid::AlignContent::center;
    grid.setGap(Grid::Px(5));

    for (auto i: { 1, 3, 4, 5, 6, 8, 10, 11, 0, 2, -1, -1, -1, 7, 9, -1}) {
        if (i > -1) {
            grid.items.add(intervalsChoices[i].get()->component);
        } else {
            grid.items.add(nullptr);
        }
    }

    // auto totalBounds = getLocalBounds();
    grid.performLayout (getLocalBounds().reduced(getLocalBounds().getWidth() / 10.0f, getLocalBounds().getHeight() / 10.0f));

    // Center the transpose slider manually;
    /* 
    auto width = totalBounds.getWidth();
    auto height = totalBounds.getHeight();
    auto slider_x = width * 5.0 / 16.0;
    auto slider_y = height / 2.0;
    auto slider_width = width / 4.0;
    auto slider_height = height / 2.0;

    // Resize the textbox.
    auto& slider = transpose.get()->component;
    slider.setBounds(Rectangle<int>(slider_x, slider_y, slider_width, slider_height)); 
    */
}