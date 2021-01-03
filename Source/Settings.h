#pragma once

#include <JuceHeader.h>

/**
 * A representation of the last UI settings to restore it when loading the editor
 */
struct UISettings
{
    UISettings() { }
    // Get it from the plugin state
    UISettings(XmlElement* xml)
    {
        if (xml != nullptr)
        {
            exists = true;
            width = xml->getIntAttribute("width");
            height = xml->getIntAttribute("height");
        }
    }

    // Build the XML representation to save in plugin state.
    XmlElement* getXml() 
    {
        XmlElement* xml = new XmlElement("UISettings");
        xml->setAttribute("width", width);
        xml->setAttribute("height", height);
        return xml;
    }

    bool exists = false;
    int width = 0;
    int height = 0;
};