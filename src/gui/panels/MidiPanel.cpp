#include "MidiPanel.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"
#include "gui/panels/MainPanel.h"

namespace Gui
{

MidiPanel::MidiPanel(MidiTransposerAudioProcessor& p)
    : juce::Component("Midi Panel")
{
    auto& midiParams = p.getMidiProcessor().getMidiParams();

    inputChannel = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *midiParams.inputChannel, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("Only the events coming from this channel will be transposed. The rest will pass through.");
            slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                auto text = juce::String(slider.getValue());
                g.setFont(LnF::getDefaultFont(26.f));
                g.drawText(text, slider.getLocalBounds(), juce::Justification::centred);
            });
        },
        "Midi Input Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    outputChannel = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *midiParams.outputChannel, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({0, 16, 1});
            slider.setTooltip("The transposed events will be routed to this channel.");
            slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                auto text = juce::String(slider.getValue());
                g.setFont(LnF::getDefaultFont(26.f));
                g.drawText(text, slider.getLocalBounds(), juce::Justification::centred);
            });
        },
        "Midi Output Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    octaveTranspose = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *midiParams.octaveTranspose, *this,
        [](Gui::CustomSlider& slider) {
            slider.setNormalisableRange({-1, 4, 1});
            slider.setTooltip("This will play the root note at its original position and transpose the chord.");
            slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                const auto value = slider.getValue();
                auto text = (value > 0 ? "+" : "") + juce::String(value);
                g.setFont(LnF::getDefaultFont(26.f));
                g.drawText(text, slider.getLocalBounds(), juce::Justification::centred);
            });
        },
        "Octave Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );
}

void MidiPanel::resized()
{
    using juce::operator""_px;
    using juce::operator""_fr;

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateRows    = { Track (1_fr) };
    grid.templateColumns = { Track (1_fr), Track (1_fr), Track (1_fr) };

    grid.alignContent    = juce::Grid::AlignContent::center;
    grid.justifyContent  = juce::Grid::JustifyContent::center;
    grid.alignItems      = juce::Grid::AlignItems::center;
    grid.justifyItems    = juce::Grid::JustifyItems::center;

    grid.columnGap = 2_px;
    grid.rowGap = 2_px;

    grid.items = {
        juce::GridItem(inputChannel->getComponent()),
        juce::GridItem(outputChannel->getComponent()),
        juce::GridItem(octaveTranspose->getComponent())
    };

    if (auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();
        grid.performLayout(juce::Rectangle<int>(0, static_cast<int>(coordinates.mHeaderHeight),
                                                getWidth(), static_cast<int>(coordinates.mKnobHeight))
                                   .reduced(static_cast<int>(coordinates.mMargin)));
    }
}

}