#include "ArpPanel.h"
#include "gui/panels/MainPanel.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"

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
            slider.setCustomTextLambda([](double value) -> juce::String {
                return juce::String(100. * (.1 + (5. - 5. * value))) + "ms";
            });
            slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                auto text = slider.getTextFromValue(slider.getValue());
                auto bounds = slider.getLocalBounds();
                g.setFont(LnF::getDefaultFont(18.f));
                g.drawText(text, bounds, juce::Justification::centred);
            });
        },
        "Arp Rate Slider", juce::Slider::SliderStyle::RotaryVerticalDrag
    );

    arpSyncRate = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *arpParams.syncRate, *this,
        [](Gui::CustomSlider& slider) {
            double nbDivisions = (double) Notes::divisions.size() - 1;
            slider.setNormalisableRange({ 0, nbDivisions, 1 });
            slider.setCustomTextLambda([](double value) -> juce::String {
                return Notes::divisions[static_cast<size_t>(value)].label;
            });
            slider.setCustomPaintLambda([&slider](juce::Graphics& g) {
                auto text = slider.getTextFromValue(slider.getValue());
                auto bounds = slider.getLocalBounds();
                g.setFont(LnF::getDefaultFont(18.f));
                g.drawText(text, bounds, juce::Justification::centred);
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

}