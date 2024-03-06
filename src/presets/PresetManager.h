#ifndef MIDITRANSPOSER_PRESETMANAGER_H
#define MIDITRANSPOSER_PRESETMANAGER_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

namespace PresetBrowser {

    class PresetManager {
    public:
        PresetManager() = delete;
        explicit PresetManager(juce::AudioProcessor& p);

        /**
         * @brief Saves the current state of params in an XML file
         * @param inPresetName The name of the XML file
         */
        bool savePreset(const juce::String& inPresetName);

        /**
         * @brief Loads an XML file and gets the param values to reset the audio processor.
         * @param inPresetName The name of the XML file to load.
         */
        void loadPreset(const juce::String& inPresetName);

        /**
         * @brief Deletes a preset from the presets directory
         * @param inPresetName The name of the preset to delete
         */
        void deletePreset(const juce::String& inPresetName);

        /**
         * @brief Resets all the parameters to their default values and set the current preset to Default.
         */
        void resetPreset();

        /**
         * @brief Reads the presets directory to get all the presets
         * @return An array of String to populate the combobox
         */
        [[nodiscard]] juce::StringArray getAllPresets() const;

        [[nodiscard]] const juce::File& getPresetPath() const { return presetsPath; }
        [[nodiscard]] const juce::String& getCurrentPreset() const { return currentPreset; }
        void setCurrentPreset(const juce::String& inPreset) { currentPreset = inPreset; }

    private:
        juce::AudioProcessor& processor;

        /**
         * @brief The root folder of the presets
         */
        juce::File presetsPath;

        /**
         * @brief The name of the currently loaded preset
         */
        juce::String currentPreset;

    public:
        static const juce::String kPresetsExtension;
        static const juce::String kInitPreset;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManager)
    };

}

#endif //MIDITRANSPOSER_PRESETMANAGER_H
