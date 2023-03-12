#include "ArpPanel.h"
#include "MainPanel.h"

namespace Gui
{

ArpPanel::ArpPanel(MidiBassPedalChordsAudioProcessor& p)
    : juce::Component("Arp Panel")
{
    auto& arpParams = p.getMidiProcessor().getArpeggiatorParams();

    arpActivated = std::make_unique< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> >(
            *arpParams.activated, *this,
            [](juce::ToggleButton& button) {
                button.setButtonText("Arpeggiator");
            },
            "Arp Activation Toggle"
    );

    arpRate = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
            *arpParams.rate, *this,
            [](Gui::CustomSlider& slider) {
                slider.setRange(0., 1.0, 0.01);
                slider.setNumDecimalPlacesToDisplay(0);
                slider.setPopupDisplayEnabled(true, true, nullptr);
                slider.setCustomTextLambda([](double value) -> juce::String {
                    return juce::String(250. * (.1 + (1. - value))) + " ms";
                });
            },
            "Arp Rate Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    arpSyncRate = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
            *arpParams.syncRate, *this,
            [](Gui::CustomSlider& slider) {
                double nbDivisions = (double) Notes::divisions.size() - 1;
                slider.setNormalisableRange({ 0, nbDivisions, 1 });
                slider.setPopupDisplayEnabled(true, true, nullptr);
                slider.setCustomTextLambda([](double value) -> juce::String {
                    return Notes::divisions[static_cast<size_t>(value)].label;
                });
            },
            "Arp Sync Rate Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    arpSynced = std::make_unique< AttachedComponent<juce::ToggleButton, juce::ButtonParameterAttachment> >(
            *arpParams.synced, *this,
            [this](juce::ToggleButton& button) {
                button.setButtonText("Tempo Sync");
                button.onStateChange = [this]() {
                    resized();
                };
            },
            "Arp Sync Toggle"
    );
}

void ArpPanel::resized()
{
    arpRate->getComponent().setVisible(!arpSynced->getComponent().getToggleState());
    arpSyncRate->getComponent().setVisible(arpSynced->getComponent().getToggleState());

    if (auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();
        auto knob_bounds = juce::Rectangle<int>(0, static_cast<int>(coordinates.mHeaderHeight),
                                                getWidth(), static_cast<int>(coordinates.mKnobHeight))
                .reduced(static_cast<int>(coordinates.mMargin));
        arpRate->getComponent().setBounds(knob_bounds);
        arpSyncRate->getComponent().setBounds(knob_bounds);
    }
}

void ArpPanel::paint(juce::Graphics& g)
{
    const auto width = static_cast<float>(getWidth());
    const auto height = static_cast<float>(getHeight());

    if (auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();

        g.setColour(findColour(juce::Label::ColourIds::backgroundColourId));
        g.drawRoundedRectangle(juce::Rectangle<float>(0.f, 0.f, width, height).reduced(coordinates.mMargin),
                               coordinates.mFrameCorner, 1.f);
        const auto header_coordinates = juce::Rectangle<float>(0.f, 0.f,
                                                               width, coordinates.mHeaderHeight).reduced(coordinates.mMargin * 2.f);
        g.fillRoundedRectangle(header_coordinates, coordinates.mFrameCorner / 2.f);
        g.setColour(findColour(juce::Label::ColourIds::textColourId));
        g.setFont(LnF::getDefaultFont(coordinates.mHeaderFontSize));
        g.drawText("ARP", header_coordinates, juce::Justification::centred);
    }
}

}