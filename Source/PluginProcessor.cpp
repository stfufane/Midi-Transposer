/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiTransposerAudioProcessor::MidiTransposerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
    treeState(*this, nullptr, ProjectInfo::projectName, createParameterLayout()),
    midiProcessor(treeState)
{ }

MidiTransposerAudioProcessor::~MidiTransposerAudioProcessor()
{
}

//==============================================================================
const String MidiTransposerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiTransposerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiTransposerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiTransposerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiTransposerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MidiTransposerAudioProcessor::getNumPrograms() { return 1; }
int MidiTransposerAudioProcessor::getCurrentProgram() { return 0; }
void MidiTransposerAudioProcessor::setCurrentProgram (int index) { }
const String MidiTransposerAudioProcessor::getProgramName (int index) { return {}; }
void MidiTransposerAudioProcessor::changeProgramName (int index, const String& newName) { }

//==============================================================================
void MidiTransposerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiTransposerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiTransposerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MidiTransposerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.clear();

    midiProcessor.process(midiMessages);
}

//==============================================================================
bool MidiTransposerAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor* MidiTransposerAudioProcessor::createEditor()
{
    return new MidiTransposerAudioProcessorEditor(*this, treeState);
}

//==============================================================================
void MidiTransposerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MidiTransposerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState::ParameterLayout MidiTransposerAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;

    for (int i = 0; i < MidiProcessor::notes.size(); i++)
    {
        auto transpose = std::make_unique<AudioParameterChoice>(
            MidiProcessor::notes[i] + "_note", MidiProcessor::notes[i], MidiProcessor::notes, i, MidiProcessor::notes[i]
        );
        auto chord = std::make_unique<AudioParameterChoice>(
            MidiProcessor::notes[i] + "_chord", MidiProcessor::notes[i] + " chord", MidiProcessor::chords, 0, MidiProcessor::notes[i] + " chord"
        );
        layout.add(std::move(transpose), std::move(chord));
    }
    auto inChannel = std::make_unique<AudioParameterInt>(IDs::paramInChannel, "Input Channel", 1, 16, 1, "Input Channel"); 
    auto outChannel = std::make_unique<AudioParameterInt>(IDs::paramOutChannel, "Output Channel", 1, 16, 1, "Output Channel");

    auto bypassChannels = std::make_unique<AudioParameterBool>(IDs::bypassOtherChannels, "Bypass other channels", false, "Bypass other channels");

    auto octaveTranspose = std::make_unique<AudioParameterInt>(IDs::octaveTranspose, "Transpose octaves", -1, 4, 0, "Transpose octaves");

    layout.add(std::move(inChannel), std::move(outChannel), std::move(bypassChannels), std::move(octaveTranspose));
    
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiTransposerAudioProcessor();
}
