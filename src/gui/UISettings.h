#pragma once

#include "JuceHeader.h"

namespace Gui
{
/**
 * @brief A representation of the last UI settings to restore it when loading the editor
 */
struct UISettings
{
    UISettings() = default;

    // Get it from the plugin state
    explicit UISettings(juce::XmlElement* xml)
    {
        if (xml != nullptr) {
            width = xml->getIntAttribute("width");
            height = xml->getIntAttribute("height");
            lastNoteIndex = xml->getIntAttribute("lastNoteIndex");
            presetName = xml->getStringAttribute("presetName");
        }
    }

    // Build the XML representation to save in plugin state.
    [[nodiscard]] juce::XmlElement* getXml() const
    {
        auto* xml = new juce::XmlElement("UISettings");
        xml->setAttribute("width", width);
        xml->setAttribute("height", height);
        xml->setAttribute("lastNoteIndex", lastNoteIndex);
        xml->setAttribute("presetName", presetName);
        return xml;
    }

    int width = 0;
    int height = 0;
    int lastNoteIndex = 0;
    juce::String presetName;
};

}