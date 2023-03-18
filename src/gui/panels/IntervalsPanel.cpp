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

    mapChoice = std::make_unique<AttachedComponent<Gui::TextSwitch, juce::ButtonParameterAttachment>>(
        *noteParam.mapNote, *this,
        [this](Gui::TextSwitch& b) {
            b.onStateChange = [this]() {
                resized();
            };
            b.setCustomTooltipLambda([&b]() {
                return juce::String(b.getToggleState() ? "Dea" : "A") + "ctivate the transposition of this note.";
            });
        },
        "Note Transpose " + noteParam.noteName + " toggle", "ON", "OFF"
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
                                                          .reduced(coordinates.mMargin, coordinates.mMargin * 5.f)
                                                          .toNearestInt());

        auto& map_choice_button = mapChoice->getComponent();
        map_choice_button.setBounds(juce::Rectangle<float>(0.f, 0.f,
                                                           static_cast<float>(getWidth()) * .1f,
                                                           static_cast<float>(getHeight()) * .2f)
                                                           .toNearestInt());

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

        for (auto& interval: intervalsChoices) {
            fb.items.add(juce::FlexItem(interval->getComponent()).withFlex(1));
        }
        fb.performLayout(juce::Rectangle<float>(coordinates.mIntervalsX + coordinates.mIntervalKnobW, 0.f,
                                                               coordinates.mIntervalsSlidersW,
                                                               static_cast<float>(getHeight()))
                                                               .reduced(coordinates.mMargin));
    }
}

}