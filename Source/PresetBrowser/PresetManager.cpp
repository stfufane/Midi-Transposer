#include "PresetManager.h"
#include "../Params/Params.h"

namespace PresetBrowser {

    const juce::String PresetManager::kPresetsExtension = "preset";

    PresetManager::PresetManager(AudioProcessor& p) :
            processor(p),
            presetsPath(File::getSpecialLocation(
                File::SpecialLocationType::commonDocumentsDirectory)
                             .getChildFile(ProjectInfo::companyName)
                             .getChildFile(ProjectInfo::projectName))
    {
        if (!presetsPath.exists()) {
            const auto result = presetsPath.createDirectory();
            if (result.failed()) {
                DBG("Could not create preset directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }
    }

    bool PresetManager::savePreset(const String &inPresetName)
    {
        if (inPresetName.isEmpty()) {
            return false;
        }
        juce::XmlElement xml("preset");

        auto* xml_params = new juce::XmlElement("Params");
        for (auto& param : processor.getParameters()) {
            xml_params->setAttribute(ParamHelper::getParamID(param), param->getValue());
        }

        xml.addChildElement(xml_params);
        const auto preset_file = presetsPath.getChildFile(inPresetName + "." + kPresetsExtension);
        if (!xml.writeTo(preset_file)) {
            DBG("Could not create preset file: " + preset_file.getFullPathName());
            jassertfalse;
            return false;
        }
        currentPreset = inPresetName;
        return true;
    }

    bool PresetManager::loadPreset(const String &inPresetName)
    {
        if (inPresetName.isEmpty()) {
            return false;
        }
        const auto preset_file = presetsPath.getChildFile(inPresetName + "." + kPresetsExtension);
        if (!preset_file.existsAsFile()) {
            DBG("Preset file " + preset_file.getFullPathName() + " does not exist");
            jassertfalse;
            return false;
        }

        XmlDocument xml_document { preset_file };
        auto xml = xml_document.getDocumentElementIfTagMatches("preset");
        if (xml != nullptr) {
            auto params = xml->getChildByName("Params");
            if (params != nullptr) {
                for (auto& param : processor.getParameters()) {
                    param->setValueNotifyingHost(static_cast<float>(params->getDoubleAttribute(ParamHelper::getParamID(param), param->getValue())));
                }
                currentPreset = inPresetName;
                return true;
            }
        }
        return false;
    }

    void PresetManager::resetPreset()
    {
        for (auto& param : processor.getParameters()) {
            param->setValueNotifyingHost(param->getDefaultValue());
        }
        currentPreset = "Default";
    }
}