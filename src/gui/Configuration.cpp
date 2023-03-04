#include "Configuration.h"

#include <fstream>

namespace Gui
{

Configuration::Configuration(juce::Component* rootComponent)
: mRootComponent(rootComponent)
{
    mFileSystemWatcher.addFolder(juce::File(CONFIG_FOLDER));
    mFileSystemWatcher.addListener(this);

    readConfiguration();
}

void Configuration::fileChanged(const juce::File inFile, gin::FileSystemWatcher::FileSystemEvent inEvent)
{
    if (inEvent != gin::FileSystemWatcher::FileSystemEvent::fileUpdated || inFile.getFileName() != "colors.json") {
        return;
    }

    // TODO: check which file is modified.
    std::cout << "File " << inFile.getFileName() << " has been modified.\n";

    readConfiguration();

    for (const auto& listener: mListeners) {
        listener->onColorsChanged(mColors);
    }

    if (mRootComponent) {
        mRootComponent->repaint();
    }
}

void Configuration::readConfiguration() {
    // Read json config
    std::string json_path = std::string(CONFIG_FOLDER) + "/colors.json";
    try {
        std::ifstream f(json_path);
        mColors = nlohmann::json::parse(f);
    } catch (std::exception& e) {
        std::cout << "Failed to read/parse the json file " << json_path << ", error : " << e.what() << "\n\n";
        assert(!mColors.empty());
    }
}

}
