#include "ArpPanel.h"
#include "gui/panels/MainPanel.h"
#include "gui/lookandfeel/BaseLookAndFeel.h"

namespace Gui
{

ArpPanel::ArpPanel(MidiTransposerAudioProcessor& p)
    : juce::Component("Arp Panel")
{
    const auto& arpParams = p.getMidiProcessor().getArpeggiatorParams();

    arpActivated = std::make_unique< AttachedComponent<Gui::TextSwitch, juce::ButtonParameterAttachment> >(
        *arpParams.activated, *this,
        [](Gui::TextSwitch& button) {
            button.setCustomTooltipLambda([&button]() -> juce::String {
                return juce::String(button.getToggleState() ? "Dea" : "A") + "ctivate the arpeggiator";
            });
        },
        "Arp Activation Toggle", "ON", "OFF", 14.f
    );

    arpRate = std::make_unique< AttachedComponent<Gui::CustomSlider, juce::SliderParameterAttachment> >(
        *arpParams.rate, *this,
        [](Gui::CustomSlider& slider) {
            slider.setRange(0., 1.0, 0.01);
            slider.setNumDecimalPlacesToDisplay(0);
            slider.setCustomTextLambda([](const double value) -> juce::String {
                return juce::String(1000. / (100. * (.1 + (5. - 5. * value))), 1) + "Hz";
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
            double nbDivisions = static_cast<double>(Notes::divisions.size()) - 1;
            slider.setNormalisableRange({ 0, nbDivisions, 1 });
            slider.setCustomTextLambda([](const double value) -> juce::String {
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

    arpSynced = std::make_unique< AttachedComponent<Gui::TextSwitch, juce::ButtonParameterAttachment> >(
        *arpParams.synced, *this,
        [this](Gui::TextSwitch& button) {
            button.onStateChange = [this]() {
                resized();
            };
            button.setCustomTooltipLambda([&button]() -> juce::String {
                return !button.getToggleState() ? "Set the arpeggiator rate to tempo sync" : "Set the arpeggiator rate to an arbitrary value";
            });
        },
        "Arp Sync Toggle", "SYNC", "FREE", 14.f
    );
}

void ArpPanel::resized()
{
    arpRate->getComponent().setVisible(!arpSynced->getComponent().getToggleState());
    arpSyncRate->getComponent().setVisible(arpSynced->getComponent().getToggleState());

    if (auto* main_panel = findParentComponentOfClass<MainPanel>(); main_panel) {
        const auto& coordinates = main_panel->getCoordinates();
        auto knob_bounds = juce::Rectangle<float>(0, coordinates.mHeaderHeight,
                                                static_cast<float>(getWidth()), coordinates.mKnobHeight)
                .reduced(coordinates.mMargin).toNearestInt();
        arpRate->getComponent().setBounds(knob_bounds);
        arpSyncRate->getComponent().setBounds(knob_bounds);

        arpActivated->getComponent().setBounds(coordinates.mArpSwitch.toNearestInt());
        arpSynced->getComponent().setBounds(coordinates.mArpSyncSwitch.toNearestInt());
    }
}

}