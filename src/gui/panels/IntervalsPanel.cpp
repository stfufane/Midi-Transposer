#include "IntervalsPanel.h"

namespace Gui
{

IntervalsPanel::IntervalsPanel(Params::NoteParam& noteParam)
    : juce::Component("Intervals Panel " + noteParam.noteName ),
      mLookAndFeel(this)
{
    setLookAndFeel(&mLookAndFeel);

    intervalsChoices.reserve(kNbIntervals);
    for (auto i = 0; i < kNbIntervals; i++) {
        intervalsChoices.emplace_back(new AttachedComponent<Gui::IndexedToggleButton, juce::ButtonParameterAttachment>(
            *noteParam.intervals[static_cast<size_t>(i)]->interval, *this,
            [this, &i](Gui::IndexedToggleButton& button) {
                button.setImage(&buttonsImage, i, kNbIntervals);
            }
        ));
    }

    transpose = std::make_unique<AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *noteParam.transpose, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({ -12, 12, 1 });
            slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
            slider.setTooltip("Choose the number of semitones you want to transpose the note.");
            slider.setCustomTextLambda([](double value) -> juce::String {
                return juce::String(value) + " semitone" + (std::abs(value) > 1 ? "s" : "");
            });
        },
        // Extra constructor parameters
        "Note Transpose " + noteParam.noteName, juce::Slider::SliderStyle::LinearBarVertical
    );
}

IntervalsPanel::~IntervalsPanel()
{
    setLookAndFeel(nullptr);
}

void IntervalsPanel::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using juce::operator""_fr;

    grid.templateColumns = {Track(1_fr), Track(1_fr), Track(1_fr),
                            Track(1_fr), Track(1_fr), Track(1_fr),
                            Track(1_fr), Track(1_fr)};
    grid.templateRows = { Track(1_fr), Track(1_fr) };
    grid.alignItems = juce::Grid::AlignItems::center;
    grid.alignContent = juce::Grid::AlignContent::center;
    grid.setGap(juce::Grid::Px(5));

    for (auto i: {1, 3, 4, 5, 6, 8, 10, 11, 0, 2, -1, -1, -1, 7, 9, -1}) {
        if (i > -1) {
            grid.items.add(intervalsChoices[static_cast<size_t>(i)]->getComponent());
        } else {
            grid.items.add(nullptr);
        }
    }

    auto totalBounds = getLocalBounds();
    auto xMargin = totalBounds.getWidth() / 10.0f;
    auto yMargin = totalBounds.getHeight() / 10.0f;
    auto gridBounds = totalBounds.reduced(static_cast<int>(xMargin), static_cast<int>(yMargin));
    grid.performLayout(gridBounds);

    // Center the transpose slider manually;
    auto width = gridBounds.getWidth();
    auto height = totalBounds.getHeight();
    auto sliderX = xMargin + width * 5.0f / 16.0f;
    auto sliderY = height / 2.0f;
    auto sliderWidth = width / 4.0f;
    auto sliderHeight = (height - yMargin * 2.0f) / 2.0f;

    // Resize the textbox.
    auto& slider = transpose->getComponent();
    slider.setBounds(static_cast<int>(sliderX), static_cast<int>(sliderY),
                     static_cast<int>(sliderWidth), static_cast<int>(sliderHeight));
}

}