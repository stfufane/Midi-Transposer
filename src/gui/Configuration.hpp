#ifndef MIDITRANSPOSER_CONFIGURATION_HPP
#define MIDITRANSPOSER_CONFIGURATION_HPP

#include <fstream>
#include "nlohmann/json.hpp"
#include "gin/gin.h"

namespace Gui
{
/**
 * @brief A helper class to listen to changes on a certain configuration file and trigger actions when it's modified
 * @tparam Data a struct type that can be serialized from json.
 * It also has to implement a getFileName static method that returns a std::string
 */
template<class Data>
class Configuration final : public gin::FileSystemWatcher::Listener
{
public:
    Configuration() = delete;

    explicit Configuration(std::string watchedFolder, juce::Component* rootComponent) :
        mWatchedFolder(std::move(watchedFolder)), mRootComponent(rootComponent)
    {
#if _DEBUG
        // Do not start watching folders in release mode
        mFileSystemWatcher.addFolder(juce::File(mWatchedFolder));
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
        virtual ~Listener() = default;

        /**
         * @brief Send the updated data
         */
        virtual void onConfigChanged(const T&) {}
    };

    void addListener(Listener<Data>* inListener) { mListeners.insert(inListener); }
    void removeListener(Listener<Data>* inListener) { mListeners.erase(inListener); }

    /**
     * @brief Triggered by the file-watcher when a file in the listened folder has been modified
     */
    void fileChanged(const juce::File& inFile, gin::FileSystemWatcher::FileSystemEvent inEvent) override
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
                juce::MessageManager::callAsync([&]() { mRootComponent->repaint(); });
            }
        }
    }

    [[nodiscard]] const Data& getData() const { return mData; }

private:
    bool reloadConfiguration()
    {
        // Read json config
        const std::string json_path = mWatchedFolder + "/" + Data::getFileName();
        try {
            std::ifstream f(json_path);
            auto j = nlohmann::json::parse(f);
            mData = j;
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
     * @brief The folder that contains the file to watch.
     */
    std::string mWatchedFolder;

    /**
     * @brief Reference to the component holding the configuration, necessary to trigger repaint after a config change.
     */
    juce::Component* mRootComponent = nullptr;

    /**
     * @brief Keep track of the listeners to notify when changes have been made
     */
    std::set<Listener<Data>*> mListeners;

    /**
     * @brief The type of data that will be retrieved from json
     */
    Data mData;
};

}

#endif //MIDITRANSPOSER_CONFIGURATION_HPP
