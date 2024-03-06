#include "PresetManager.h"
#include "params/Params.h"
#include "ProjectInfo.h"

namespace PresetBrowser {

    const juce::String PresetManager::kPresetsExtension = "preset";
    const juce::String PresetManager::kInitPreset = "Init";

    PresetManager::PresetManager(juce::AudioProcessor& p) :
            processor(p),
            presetsPath(juce::File::getSpecialLocation(
                    juce::File::SpecialLocationType::commonDocumentsDirectory)
                             .getChildFile(ProjectInfo::companyName)
                             .getChildFile(ProjectInfo::projectName))
    {
        if (!presetsPath.exists()) {
            if (const auto result = presetsPath.createDirectory(); result.failed()) {
                DBG("Could not create preset directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }
    }

    bool PresetManager::savePreset(const juce::String& inPresetName)
    {
        if (inPresetName.isEmpty() || inPresetName == kInitPreset) {
            return false;
        }
        juce::XmlElement xml("preset");

        auto* xml_params = new juce::XmlElement("params");
        for (auto& param : processor.getParameters()) {
            xml_params->setAttribute(Params::ParamHelper::getParamID(param), param->getValue());
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

    void PresetManager::loadPreset(const juce::String& inPresetName)
    {
        if (inPresetName.isEmpty()) {
            return;
        }
        const auto preset_file = presetsPath.getChildFile(inPresetName + "." + kPresetsExtension);
        if (!preset_file.existsAsFile()) {
            DBG("Preset file " + preset_file.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }

        juce::XmlDocument xml_document { preset_file };
        auto xml = xml_document.getDocumentElementIfTagMatches("preset");
        if (xml != nullptr) {
            auto params = xml->getChildByName("params");
            if (params != nullptr) {
                for (auto& param : processor.getParameters()) {
                    param->setValueNotifyingHost(static_cast<float>(params->getDoubleAttribute(Params::ParamHelper::getParamID(param), param->getValue())));
                }
                currentPreset = inPresetName;
            }
        }
    }

    void PresetManager::deletePreset(const juce::String& inPresetName)
    {
        if (inPresetName.isEmpty() || inPresetName == kInitPreset) {
            return;
        }

        const auto preset_file = presetsPath.getChildFile(inPresetName + "." + kPresetsExtension);
        if (!preset_file.existsAsFile()) {
            DBG("Preset file " + preset_file.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }
        if (preset_file.deleteFile()) {
            DBG("Preset file " + preset_file.getFullPathName() + " deleted");
            // Reload the default preset after the deletion.
            resetPreset();
        } else {
            DBG("Could not delete preset file: " + preset_file.getFullPathName());
            jassertfalse;
        }
    }

    void PresetManager::resetPreset()
    {
        for (auto& param : processor.getParameters()) {
            param->setValueNotifyingHost(param->getDefaultValue());
        }
        currentPreset = kInitPreset;
    }

    juce::StringArray PresetManager::getAllPresets() const {
        juce::StringArray presets;
        const auto fileArray = presetsPath.findChildFiles(
                juce::File::TypesOfFileToFind::findFiles, false, "*." + kPresetsExtension);
        for (const auto& file : fileArray) {
            presets.add(file.getFileNameWithoutExtension());
        }
        return presets;
    }
}