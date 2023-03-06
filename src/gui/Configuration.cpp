#include "Configuration.h"

#include <fstream>

namespace Gui
{

const std::unordered_map<std::string, Configuration::ConfigurationType> Configuration::kConfigFiles =
{
    { "colors.json", ConfigurationType::eColors },
    { "positions.json", ConfigurationType::ePositions }
};

Configuration::Configuration(juce::Component* rootComponent)
: mRootComponent(rootComponent)
{
#if _DEBUG
    // Do not start watching folders in release mode
    mFileSystemWatcher.addFolder(juce::File(CONFIG_FOLDER));
#endif
    mFileSystemWatcher.addListener(this);

    readAllConfigurations();
}

Configuration::~Configuration()
{
    mFileSystemWatcher.removeListener(this);
}

void Configuration::fileChanged(const juce::File inFile, gin::FileSystemWatcher::FileSystemEvent inEvent)
{
    if (inEvent != gin::FileSystemWatcher::FileSystemEvent::fileUpdated) {
        return;
    }

    // Check which file is modified.
    const auto filename = inFile.getFileName().toStdString();
    auto found = kConfigFiles.find(filename);
    if (found == kConfigFiles.cend()) {
        return;
    }

    auto conf_type = found->second;

    readConfiguration(conf_type, filename);
    notifyListeners(conf_type);

    if (mRootComponent) {
        mRootComponent->repaint();
    }
}

void Configuration::readAllConfigurations()
{
    for (const auto& [filename, conf_type]: kConfigFiles) {
        readConfiguration(conf_type, filename);
    }
}

void Configuration::readConfiguration(ConfigurationType conf_type, const std::string& filename) {
    // Read json config
    std::string json_path = std::string(CONFIG_FOLDER) + "/" + filename;
    try {
        std::ifstream f(json_path);
        switch (conf_type) {
            case ConfigurationType::eColors:
                mColors = nlohmann::json::parse(f);
                break;
            case ConfigurationType::ePositions:
                mPositions = nlohmann::json::parse(f);
                break;
            default:
                break;
        }
    } catch (std::exception& e) {
        std::cout << "Failed to read/parse the json file " << json_path << ", error : " << e.what() << "\n\n";
        return;
    }
}

void Configuration::notifyListeners(ConfigurationType conf_type)
{
    for (auto* listener: mListeners) {
        switch (conf_type) {
            case ConfigurationType::eColors:
                listener->onColorsChanged(mColors);
                break;
            case ConfigurationType::ePositions:
                listener->onPositionsChanged(mPositions);
                break;
            default:
                break;
        }
    }
}

}
