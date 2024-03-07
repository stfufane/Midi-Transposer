#include "PluginProcessor.h"
#include "gui/PluginEditor.h"

//==============================================================================
MidiTransposerAudioProcessor::MidiTransposerAudioProcessor()
    : juce::AudioProcessor (juce::AudioProcessor::BusesProperties().withInput("Input", juce::AudioChannelSet::mono(), true)),
      presetManager(*this)
{ 
    // Add the parameters and listeners from the midi processor.
    midiProcessor.addParameters(*this);
}

MidiTransposerAudioProcessor::~MidiTransposerAudioProcessor() = default;

//==============================================================================
const juce::String MidiTransposerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiTransposerAudioProcessor::acceptsMidi() const { return true; }
bool MidiTransposerAudioProcessor::producesMidi() const { return true; }
bool MidiTransposerAudioProcessor::isMidiEffect() const { return true; }
double MidiTransposerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MidiTransposerAudioProcessor::getNumPrograms() { return 1; }
int MidiTransposerAudioProcessor::getCurrentProgram() { return 0; }
void MidiTransposerAudioProcessor::setCurrentProgram (int) { }
const juce::String MidiTransposerAudioProcessor::getProgramName (int) { return {}; }
void MidiTransposerAudioProcessor::changeProgramName (int, const juce::String&) { }

//==============================================================================
void MidiTransposerAudioProcessor::releaseResources() {}
bool MidiTransposerAudioProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout&) const { return true; }

void MidiTransposerAudioProcessor::prepareToPlay (double sampleRate, int)
{
    midiProcessor.prepareToPlay(sampleRate);
}

void MidiTransposerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    // The real processing is made in the MidiProcessor class.
    midiProcessor.process(midiMessages, buffer.getNumSamples(), getPlayHead());
}

//==============================================================================
bool MidiTransposerAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* MidiTransposerAudioProcessor::createEditor()
{
    return new MidiTransposerAudioProcessorEditor(*this);
}

//==============================================================================
void MidiTransposerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
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

void MidiTransposerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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

void MidiTransposerAudioProcessor::saveEditorSize(int w, int h)
{
    uiSettings.width = w;
    uiSettings.height = h;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiTransposerAudioProcessor();
}
