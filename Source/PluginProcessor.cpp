#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiBassPedalChordsAudioProcessor::MidiBassPedalChordsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties())
#else
    : 
#endif
{ 
    // This is called in the constructor body cause both midiProcessor and treeState have to be initialized
    // before it's called.
    midiProcessor.registerListeners(treeState);
}

MidiBassPedalChordsAudioProcessor::~MidiBassPedalChordsAudioProcessor() {}

//==============================================================================
const String MidiBassPedalChordsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiBassPedalChordsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiBassPedalChordsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiBassPedalChordsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiBassPedalChordsAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MidiBassPedalChordsAudioProcessor::getNumPrograms() { return 1; }
int MidiBassPedalChordsAudioProcessor::getCurrentProgram() { return 0; }
void MidiBassPedalChordsAudioProcessor::setCurrentProgram (int index) { }
const String MidiBassPedalChordsAudioProcessor::getProgramName (int index) { return {}; }
void MidiBassPedalChordsAudioProcessor::changeProgramName (int index, const String& newName) { }

//==============================================================================
void MidiBassPedalChordsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}
void MidiBassPedalChordsAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiBassPedalChordsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MidiBassPedalChordsAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.clear();
    // The real processing is made in the MidiProcessor class.
    midiProcessor.process(midiMessages);
}

int MidiBassPedalChordsAudioProcessor::getCurrentNotePlayed()
{
    return midiProcessor.getLastNoteOn();
}

//==============================================================================
bool MidiBassPedalChordsAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor* MidiBassPedalChordsAudioProcessor::createEditor()
{
    return new MidiBassPedalChordsAudioProcessorEditor(*this, treeState);
}

//==============================================================================
void MidiBassPedalChordsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MidiBassPedalChordsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiBassPedalChordsAudioProcessor();
}
