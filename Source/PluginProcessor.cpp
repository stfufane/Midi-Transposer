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
    // Add the parameters and listeners from the midi processor.
    midiProcessor.addParameters(*this);
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
void MidiBassPedalChordsAudioProcessor::setCurrentProgram (int) { }
const String MidiBassPedalChordsAudioProcessor::getProgramName (int) { return {}; }
void MidiBassPedalChordsAudioProcessor::changeProgramName (int, const String&) { }

//==============================================================================
void MidiBassPedalChordsAudioProcessor::prepareToPlay (double, int) {}
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

//==============================================================================
bool MidiBassPedalChordsAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor* MidiBassPedalChordsAudioProcessor::createEditor()
{
    return new MidiBassPedalChordsAudioProcessorEditor(*this);
}

//==============================================================================
void MidiBassPedalChordsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml("PluginState");

    XmlElement* params = new XmlElement("Params");
    for (auto& param : getParameters())
        params->setAttribute(ParamHelper::getParamID(param), param->getValue());

    xml.addChildElement(params);
    xml.addChildElement(uiSettings.getXml());

    copyXmlToBinary(xml, destData);
}

void MidiBassPedalChordsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);

    if (xml != nullptr)
    {
        auto params = xml->getChildByName("Params");
        if (params != nullptr) {
            for (auto& param : getParameters())
                param->setValueNotifyingHost(params->getDoubleAttribute(ParamHelper::getParamID(param), param->getValue()));
        }

        uiSettings = UISettings(xml->getChildByName("UISettings"));
    }
}

void MidiBassPedalChordsAudioProcessor::setEditorSize(int w, int h) 
{
    uiSettings.width = w;
    uiSettings.height = h;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiBassPedalChordsAudioProcessor();
}
