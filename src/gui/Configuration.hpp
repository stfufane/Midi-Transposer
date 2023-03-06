#ifndef MIDIBASSPEDALCHORDS_CONFIGURATION_HPP
#define MIDIBASSPEDALCHORDS_CONFIGURATION_HPP

#include "JuceHeader.h"
#include <fstream>
#include <map>
#include "nlohmann/json.hpp"
#include "gin/utilities/gin_filesystemwatcher.h"

namespace Gui
{

enum class ConfigurationType {
    eColors,
    ePositions,
};

const std::map<ConfigurationType, std::string> kConfigFiles =
{
    { ConfigurationType::eColors, "colors.json" },
    { ConfigurationType::ePositions, "positions.json" }
};

/**
 * @brief A helper class to listen to changes on a certain configuration file and trigger actions when it's modified
 * @tparam conf_type The type of file to watch
 */
template<ConfigurationType conf_type>
class Configuration : public gin::FileSystemWatcher::Listener
{
public:
    Configuration() = delete;

    explicit Configuration(juce::Component* rootComponent) : mRootComponent(rootComponent)
    {
#if _DEBUG
        // Do not start watching folders in release mode
        mFileSystemWatcher.addFolder(juce::File(CONFIG_FOLDER));
#endif
        mFileSystemWatcher.addListener(this);

        reloadConfiguration();
    }

    ~Configuration() override
    {
        mFileSystemWatcher.removeListener(this);
    }

    /**
     * @brief Declare a simple listener type for classes that use the dynamic config
     */
    class Listener {
    public:
        /**
         * @brief Send the updated json and the type of config that has been updated.
         */
        virtual void onConfigChanged(const nlohmann::json&, ConfigurationType) {}
    };

    void addListener(Listener* inListener) { mListeners.insert(inListener); }
    void removeListener(Listener* inListener) { mListeners.erase(inListener); }

    /**
     * @brief Triggered by the filewatcher when a file in the listened folder has been modified
     */
    void fileChanged(const juce::File inFile, gin::FileSystemWatcher::FileSystemEvent inEvent) override
    {
        if (inEvent != gin::FileSystemWatcher::FileSystemEvent::fileUpdated) {
            return;
        }

        // Check if the right file is modified
        if (inFile.getFileName().toStdString() != kConfigFiles.at(conf_type)) {
            return;
        }

        if (reloadConfiguration()) {
            notifyListeners();
        }

        if (mRootComponent) {
            mRootComponent->repaint();
        }
    }

    [[nodiscard]] const nlohmann::json& getJson() const { return mJson; }

private:
    bool reloadConfiguration()
    {
        // Read json config
        std::string json_path = std::string(CONFIG_FOLDER) + "/" + kConfigFiles.at(conf_type);
        try {
            std::ifstream f(json_path);
            mJson = nlohmann::json::parse(f);
            return true;
        } catch (std::exception& e) {
            std::cout << "Failed to read/parse the json file " << json_path << ", error : " << e.what() << "\n\n";
            return false;
        }
    }

    void notifyListeners()
    {
        for (auto* listener: mListeners) {
            listener->onConfigChanged(mJson, conf_type);
        }
    }

    /**
     * @brief A background thread that will listen to changes on the wanted file
     */
    gin::FileSystemWatcher mFileSystemWatcher;

    /**
     * @brief Reference to the editor, necessary to trigger repaint after a config change.
     */
    juce::Component* mRootComponent = nullptr;

    /**
     * @brief Keep track of the listeners to notify when changes have been made
     */
    std::set<Listener*> mListeners;

    /**
     * @brief The data representation of the configuration json file
     */
    nlohmann::json mJson;
};

}

#endif //MIDIBASSPEDALCHORDS_CONFIGURATION_HPP
