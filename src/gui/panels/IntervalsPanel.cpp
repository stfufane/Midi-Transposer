#include "IntervalsPanel.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"

namespace Gui
{

IntervalsPanel::IntervalsPanel(Params::NoteParam& noteParam)
    : juce::Component("Intervals Panel " + noteParam.noteName)
{
    intervalsChoices.reserve(kNbIntervals);
    for (auto i = 0; i < kNbIntervals; i++) {
        intervalsChoices.emplace_back(new AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment>(
            *noteParam.intervals[static_cast<size_t>(i)]->interval, *this,
            [](Gui::CustomSlider& slider) {
                slider.setNormalisableRange({ -12, 12, 1 });
                slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                    const auto value = slider.getValue();
                    auto text = (value > 0 ? "+" : "") + juce::String(value);
                    g.setFont(LnF::getDefaultFont(20.f));
                    g.setColour(juce::Colours::white);
                    g.drawText(text, slider.getLocalBounds().withY(slider.getHeight()), juce::Justification::centred);
                });
            },
            // Extra constructor params
            "Interval transpose " + noteParam.noteName + "_" + std::to_string(i), juce::Slider::SliderStyle::LinearVertical
        ));
    }

    transpose = std::make_unique<AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *noteParam.transpose, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({ -12, 12, 1 });
            slider.setTooltip("Choose the number of semitones you want to transpose the note.");
            slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                const auto value = slider.getValue();
                auto text = (value > 0 ? "+" : "") + juce::String(value);
                g.setFont(LnF::getDefaultFont(26.f));
                g.drawText(text, slider.getLocalBounds(), juce::Justification::centred);
            });
        },
        // Extra constructor parameters
        "Note Transpose " + noteParam.noteName, juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    mapChoice = std::make_unique<AttachedComponent<Gui::IndexedToggleButton, juce::ButtonParameterAttachment>>(
        *noteParam.mapNote, *this,
        [](Gui::IndexedToggleButton& button) {
            // TODO something :)
        }
    );
}

void IntervalsPanel::resized()
{
    if (auto* main_panel = findParentComponentOfClass<Gui::MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();

        auto& transpose_slider = transpose->getComponent();
        transpose_slider.setBounds(juce::Rectangle<float>(coordinates.mIntervalsX, 0.f,
                                                          coordinates.mIntervalKnobW,
                                                          coordinates.mIntervalsH)
                                                          .reduced(coordinates.mMargin, coordinates.mMargin * 2.f)
                                                          .toNearestInt());

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

        for (auto& interval: intervalsChoices) {
            fb.items.add(juce::FlexItem(interval->getComponent()).withFlex(1));
        }
        fb.performLayout(juce::Rectangle<float>(coordinates.mIntervalsX + coordinates.mIntervalKnobW, 0.f,
                                                               coordinates.mIntervalsSlidersW,
                                                               coordinates.mIntervalsH)
                                                               .reduced(coordinates.mMargin));
    }
}

}