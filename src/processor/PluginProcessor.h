#pragma once

#include "JuceHeader.h"
#include "processor/MidiProcessor.h"
#include "gui/UISettings.h"
#include "presets/PresetManager.h"

/**
 * 
*/
class MidiBassPedalChordsAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiBassPedalChordsAudioProcessor();
    ~MidiBassPedalChordsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    MidiProcessor& getMidiProcessor() { return midiProcessor; }
    Gui::UISettings& getUISettings() { return uiSettings; }
    PresetBrowser::PresetManager& getPresetManager() { return presetManager; }
    void saveEditorSize(int w, int h);
private:
    Gui::UISettings uiSettings;
    PresetBrowser::PresetManager presetManager;
    MidiProcessor midiProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiBassPedalChordsAudioProcessor)
};
