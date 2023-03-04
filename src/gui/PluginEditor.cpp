#include "PluginEditor.h"

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), 
      mainPanel(p),
      tooltipWindow(mainPanel.getTooltipPanel(), 50)
{
    setResizable(true, true);

    // Retrieve useful data from the processor.
    auto& uiSettings = p.getUISettings();

    // Restore the last size if it exists.
    if (uiSettings.width != 0) {
        setSize(uiSettings.width, uiSettings.height);
    } else {
        setSize(kWindowWidth, kWindowHeight);
    }
    setResizeLimits(kWindowWidth, kWindowHeight,
                    static_cast<int>(kWindowWidth * kMaxResize), static_cast<int>(kWindowHeight * kMaxResize));
    getConstrainer()->setFixedAspectRatio(kWindowRatio);

    setLookAndFeel(&mLookAndFeel);
    tooltipWindow.setLookAndFeel(&mLookAndFeel);

    mFileSystemWatcher.addFolder(juce::File(CONFIG_FOLDER));
    mFileSystemWatcher.addListener(this);

    addAndMakeVisible(mainPanel);
}

MidiBassPedalChordsAudioProcessorEditor::~MidiBassPedalChordsAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MidiBassPedalChordsAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
}

void MidiBassPedalChordsAudioProcessorEditor::resized()
{
    auto* p = dynamic_cast<MidiBassPedalChordsAudioProcessor*>(&processor);
    p->saveEditorSize(getWidth(), getHeight());

    mainPanel.setSize(getWidth(), getHeight());
}

void MidiBassPedalChordsAudioProcessorEditor::fileChanged(const juce::File inFile, gin::FileSystemWatcher::FileSystemEvent inEvent)
{
    if (inEvent != gin::FileSystemWatcher::FileSystemEvent::fileUpdated || inFile.getFileName() != "colors.json") {
        return;
    }
    
    std::cout << "File " << inFile.getFileName() << " has been modified.\n";
    mLookAndFeel.resetConfiguration();
    repaint();
}