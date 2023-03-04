#include "PluginProcessor.h"
#include "gui/PluginEditor.h"

//==============================================================================
MidiBassPedalChordsAudioProcessor::MidiBassPedalChordsAudioProcessor()
    : juce::AudioProcessor (juce::AudioProcessor::BusesProperties().withInput("Input", juce::AudioChannelSet::mono(), true)),
      presetManager(*this)
{ 
    // Add the parameters and listeners from the midi processor.
    midiProcessor.addParameters(*this);
}

MidiBassPedalChordsAudioProcessor::~MidiBassPedalChordsAudioProcessor() = default;

//==============================================================================
const juce::String MidiBassPedalChordsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiBassPedalChordsAudioProcessor::acceptsMidi() const { return true; }
bool MidiBassPedalChordsAudioProcessor::producesMidi() const { return true; }
bool MidiBassPedalChordsAudioProcessor::isMidiEffect() const { return true; }
double MidiBassPedalChordsAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MidiBassPedalChordsAudioProcessor::getNumPrograms() { return 1; }
int MidiBassPedalChordsAudioProcessor::getCurrentProgram() { return 0; }
void MidiBassPedalChordsAudioProcessor::setCurrentProgram (int) { }
const juce::String MidiBassPedalChordsAudioProcessor::getProgramName (int) { return {}; }
void MidiBassPedalChordsAudioProcessor::changeProgramName (int, const juce::String&) { }

//==============================================================================
void MidiBassPedalChordsAudioProcessor::releaseResources() {}
bool MidiBassPedalChordsAudioProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout&) const { return true; }

void MidiBassPedalChordsAudioProcessor::prepareToPlay (double sampleRate, int) 
{
    midiProcessor.prepareToPlay(sampleRate);
}

void MidiBassPedalChordsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    // The real processing is made in the MidiProcessor class.
    midiProcessor.process(midiMessages, buffer.getNumSamples(), getPlayHead());
}

//==============================================================================
bool MidiBassPedalChordsAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* MidiBassPedalChordsAudioProcessor::createEditor()
{
    return new MidiBassPedalChordsAudioProcessorEditor(*this);
}

//==============================================================================
void MidiBassPedalChordsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::XmlElement xml("PluginState");

    auto* xml_params = new juce::XmlElement("params");
    for (const auto& param : getParameters()) {
        xml_params->setAttribute(Params::ParamHelper::getParamID(param), param->getValue());
    }
    // Store the name of the current preset.
    uiSettings.presetName = presetManager.getCurrentPreset();

    xml.addChildElement(xml_params);
    xml.addChildElement(uiSettings.getXml());

    copyXmlToBinary(xml, destData);
}

void MidiBassPedalChordsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);

    if (xml != nullptr) {
        auto params = xml->getChildByName("params");
        if (params != nullptr) {
            for (auto& param: getParameters()) {
                param->setValueNotifyingHost(
                        static_cast<float>(params->getDoubleAttribute(Params::ParamHelper::getParamID(param),
                                                                      param->getValue())));
            }
        }

        uiSettings = Gui::UISettings(xml->getChildByName("UISettings"));
        presetManager.setCurrentPreset(uiSettings.presetName);
    }
}

void MidiBassPedalChordsAudioProcessor::saveEditorSize(int w, int h)
{
    uiSettings.width = w;
    uiSettings.height = h;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiBassPedalChordsAudioProcessor();
}
