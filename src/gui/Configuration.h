#ifndef MIDIBASSPEDALCHORDS_CONFIGURATION_H
#define MIDIBASSPEDALCHORDS_CONFIGURATION_H

#include "JuceHeader.h"
#include "nlohmann/json.hpp"
#include "gin/utilities/gin_filesystemwatcher.h"

namespace Gui
{

class Configuration : public gin::FileSystemWatcher::Listener
{
public:
    explicit Configuration(juce::Component* rootComponent);
    ~Configuration() override;

    /**
     * @brief Declare several callbacks, one for each kind of config file
     */
    class Listener {
    public:
        // Auto add listener to the config
        explicit Listener(Configuration* config) : mConfig(config) {
            mConfig->addListener(this);
        }

        // Auto remove at destruction
        virtual ~Listener() {
            mConfig->removeListener(this);
        }

        virtual void onColorsChanged(const nlohmann::json&) {}
        virtual void onPositionsChanged(const nlohmann::json&) {}

    private:
        Configuration* mConfig;
    };

    enum class ConfigurationType {
        eColors,
        ePositions,
    };

    void addListener(Listener* inListener) { mListeners.insert(inListener); }
    void removeListener(Listener* inListener) { mListeners.erase(inListener); }

    /**
     * @brief Triggered by the filewatcher when a file in the listened folder has been modified
     */
    void fileChanged(juce::File, gin::FileSystemWatcher::FileSystemEvent) override;

    [[nodiscard]] const nlohmann::json& getColors() const { return mColors; }

private:
    static const std::unordered_map<std::string, ConfigurationType> kConfigFiles;

    void readAllConfigurations();

    void readConfiguration(ConfigurationType conf_type, const std::string& filename);
    void notifyListeners(ConfigurationType conf_type);

    gin::FileSystemWatcher mFileSystemWatcher;

    /**
     * @brief Reference to the editor, necessary to trigger repaint after a config change.
     */
    juce::Component* mRootComponent = nullptr;

    /**
     * @brief Keep track of the listeners to notify when changes have been made
     */
    std::set<Listener*> mListeners;

    nlohmann::json mColors;
    nlohmann::json mPositions;
};

}

#endif //MIDIBASSPEDALCHORDS_CONFIGURATION_H
