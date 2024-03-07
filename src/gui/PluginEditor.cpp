#include "PluginEditor.h"

//==============================================================================
MidiTransposerAudioProcessorEditor::MidiTransposerAudioProcessorEditor(MidiTransposerAudioProcessor& p)
        : juce::AudioProcessorEditor(&p),
          mLookAndFeel(this),
          mainPanel(p),
          tooltipWindow(mainPanel.getTooltipPanel(), 50)
{
    setResizable(true, true);

    // Retrieve useful data from the processor.
    const auto& uiSettings = p.getUISettings();

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

    addAndMakeVisible(mainPanel);
}

MidiTransposerAudioProcessorEditor::~MidiTransposerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MidiTransposerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
}

void MidiTransposerAudioProcessorEditor::resized()
{
    auto* p = dynamic_cast<MidiTransposerAudioProcessor*>(&processor);
    p->saveEditorSize(getWidth(), getHeight());

    mainPanel.setSize(kWindowWidth, kWindowHeight);
    mainPanel.setTransform(juce::AffineTransform::scale(static_cast<float>(getWidth()) / static_cast<float>(kWindowWidth)));
}
