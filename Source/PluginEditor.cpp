#include "PluginEditor.h"

//==============================================================================
MidiBassPedalChordsAudioProcessorEditor::MidiBassPedalChordsAudioProcessorEditor(MidiBassPedalChordsAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), 
      mainPanel(p)
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
    setResizeLimits(kWindowWidth, kWindowHeight, kWindowWidth * kMaxResize, kWindowHeight * kMaxResize);
    getConstrainer()->setFixedAspectRatio(kWindowRatio);

    addAndMakeVisible(mainPanel);
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

