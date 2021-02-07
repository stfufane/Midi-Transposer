#include "Panels.h"

IntervalsPanel::IntervalsPanel(NoteParam& noteParam)
{
    intervalsChoices.reserve(NB_INTERVALS);
    for (auto i = 0; i < NB_INTERVALS; i++)
    {
        intervalsChoices.emplace_back(new AttachedComponent<IndexedToggleButton, juce::ButtonParameterAttachment>(
            *noteParam.intervals[i]->interval, *this, 
            [this, &i](IndexedToggleButton& button) {
                button.setImage(&buttonsImage, i, NB_INTERVALS);
            }
        ));
    }

    transpose = std::make_unique< AttachedComponent<SemitoneSlider, juce::SliderParameterAttachment> >(
        *noteParam.transpose, *this,
        [](SemitoneSlider& slider) {
            slider.setNormalisableRange({-12, 12, 1});
            slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
            slider.setTooltip("Choose the number of semitones you want to transpose the note.");
        }
    );
}

void IntervalsPanel::paint(juce::Graphics&) {}

void IntervalsPanel::resized()
{
    juce::Grid grid;

    grid.templateColumns    = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
    grid.templateRows       = { Track(Fr(1)), Track(Fr(1)) };
    grid.alignItems         = juce::Grid::AlignItems::center;
    grid.alignContent       = juce::Grid::AlignContent::center;
    grid.setGap(juce::Grid::Px(5));

    for (auto i: { 1, 3, 4, 5, 6, 8, 10, 11, 0, 2, -1, -1, -1, 7, 9, -1}) {
        if (i > -1) {
            grid.items.add(intervalsChoices[i].get()->component);
        } else {
            grid.items.add(nullptr);
        }
    }

    auto totalBounds = getLocalBounds();
    auto xMargin = totalBounds.getWidth() / 10.0f;
    auto yMargin = totalBounds.getHeight() / 10.0f;
    auto gridBounds = totalBounds.reduced(xMargin, yMargin);
    grid.performLayout (gridBounds);

    // Center the transpose slider manually;
    auto width = gridBounds.getWidth();
    auto height = totalBounds.getHeight();
    auto sliderX = xMargin + width * 5.0f / 16.0f;
    auto sliderY = height / 2.0f;
    auto sliderWidth = width / 4.0f;
    auto sliderHeight = ( height - yMargin * 2.0f ) / 2.0f;

    // Resize the textbox.
    auto& slider = transpose.get()->component;
    slider.setBounds(juce::Rectangle<int>(sliderX, sliderY, sliderWidth, sliderHeight)); 
}