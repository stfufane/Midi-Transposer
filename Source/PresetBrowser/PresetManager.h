#ifndef MIDIBASSPEDALCHORDS_PRESETMANAGER_H
#define MIDIBASSPEDALCHORDS_PRESETMANAGER_H

#include <JuceHeader.h>

namespace PresetBrowser {

    class PresetManager {
    public:
        PresetManager() = delete;
        explicit PresetManager(AudioProcessor& p);

        /**
         * @brief Saves the current state of params in an XML file
         * @param inPresetName The name of the XML file
         */
        bool savePreset(const juce::String &inPresetName);

        /**
         * @brief Loads an XML file and gets the param values to reset the audio processor.
         * @param inPresetName The name of the XML file to load.
         */
        bool loadPreset(const juce::String &inPresetName);

        /**
         * @brief Resets all the parameters to their default values and set the current preset to Default.
         */
        void resetPreset();

        const juce::File& getPresetPath() const { return presetsPath; }
        const juce::String& getCurrentPreset() const { return currentPreset; }
        void setCurrentPreset(const juce::String& inPreset) { currentPreset = inPreset; }

    private:
        AudioProcessor& processor;

        /**
         * @brief The root folder of the presets
         */
        juce::File presetsPath;

        /**
         * @brief The name of the currently loaded preset
         */
        juce::String currentPreset { "Default" };

    public:
        static const juce::String kPresetsExtension;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManager)
    };

}

#endif //MIDIBASSPEDALCHORDS_PRESETMANAGER_H
