#ifndef MIDIBASSPEDALCHORDS_CONFIGURATION_HPP
#define MIDIBASSPEDALCHORDS_CONFIGURATION_HPP

#include "JuceHeader.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "gin/utilities/gin_filesystemwatcher.h"

namespace Gui
{
/**
 * @brief A helper class to listen to changes on a certain configuration file and trigger actions when it's modified
 * @tparam Data a struct type that can be serialized from json.
 * It also has to implement a getFileName static method that returns a std::string
 */
template<class Data>
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
    template<typename T>
    class Listener {
    public:
        /**
         * @brief Send the updated json and the type of config that has been updated.
         */
        virtual void onConfigChanged(const T&) {}
    };

    void addListener(Listener<Data>* inListener) { mListeners.insert(inListener); }
    void removeListener(Listener<Data>* inListener) { mListeners.erase(inListener); }

    /**
     * @brief Triggered by the file-watcher when a file in the listened folder has been modified
     */
    void fileChanged(const juce::File inFile, gin::FileSystemWatcher::FileSystemEvent inEvent) override
    {
        // Only interested in modified files
        if (inEvent != gin::FileSystemWatcher::FileSystemEvent::fileUpdated) {
            return;
        }

        // Check if the right file is modified
        if (inFile.getFileName().toStdString() != Data::getFileName()) {
            return;
        }

        if (reloadConfiguration()) {
            notifyListeners();
            if (mRootComponent) {
                mRootComponent->repaint();
            }
        }
    }

    [[nodiscard]] const Data& getData() const { return mData; }

private:
    bool reloadConfiguration()
    {
        // Read json config
        std::string json_path = std::string(CONFIG_FOLDER) + "/" + Data::getFileName();
        try {
            std::ifstream f(json_path);
            mJson = nlohmann::json::parse(f);
            mData = mJson;
            return true;
        } catch (std::exception& e) {
            std::cout << "Failed to read/parse the json file " << json_path << ", error : " << e.what() << "\n\n";
            return false;
        }
    }

    void notifyListeners()
    {
        for (auto* listener: mListeners) {
            listener->onConfigChanged(mData);
        }
    }

    /**
     * @brief A background thread that will listen to changes on the wanted file
     */
    gin::FileSystemWatcher mFileSystemWatcher;

    /**
     * @brief Reference to the component holding the configuration, necessary to trigger repaint after a config change.
     */
    juce::Component* mRootComponent = nullptr;

    /**
     * @brief Keep track of the listeners to notify when changes have been made
     */
    std::set<Listener<Data>*> mListeners;

    /**
     * @brief The data representation of the configuration json file
     */
    nlohmann::json mJson;

    /**
     * @brief The type of data that will be retrieved from json
     */
    Data mData;
};

}

#endif //MIDIBASSPEDALCHORDS_CONFIGURATION_HPP
