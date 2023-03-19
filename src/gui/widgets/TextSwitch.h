#pragma once

#include "JuceHeader.h"
#include "CustomToggleButton.h"

namespace Gui
{
    class TextSwitch : public Gui::CustomToggleButton
    {
    public:
        TextSwitch() = delete;
        explicit TextSwitch(const juce::String& name, juce::String onText, juce::String offText, float fontSize = 20.f);

        void paint(juce::Graphics& g) override;

        void setOnText(const juce::String& inText) { onText = inText; }
        void setOffText(const juce::String& inText) { offText = inText; }
    private:
        juce::String onText;
        juce::String offText;
        float fontSize = 20.f;
    };
}