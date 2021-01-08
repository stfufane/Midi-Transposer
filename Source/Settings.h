#pragma once

#include <JuceHeader.h>

/**
 * A representation of the last UI settings to restore it when loading the editor
 */
struct UISettings
{
    UISettings() = default;
    // Get it from the plugin state
    UISettings(XmlElement* xml)
    {
        if (xml != nullptr)
        {
            width = xml->getIntAttribute("width");
            height = xml->getIntAttribute("height");
            lastNoteIndex = xml->getIntAttribute("lastNoteIndex");
        }
    }

    // Build the XML representation to save in plugin state.
    XmlElement* getXml() 
    {
        XmlElement* xml = new XmlElement("UISettings");
        xml->setAttribute("width", width);
        xml->setAttribute("height", height);
        xml->setAttribute("lastNoteIndex", lastNoteIndex);
        return xml;
    }

    int width = 0;
    int height = 0;
    int lastNoteIndex = 0;
};