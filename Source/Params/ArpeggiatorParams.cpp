#include "Params.h"

ArpeggiatorParams::~ArpeggiatorParams()
{
    activated->removeListener(this);
}

void ArpeggiatorParams::addParams(juce::AudioProcessor& p)
{
    p.addParameter(activated = new juce::AudioParameterBool(ParamIDs::arpeggiatorActivated, "Arpeggiator activated", true, "Activated"));
    p.addParameter(synced    = new juce::AudioParameterBool(ParamIDs::arpeggiatorSync, "Arpeggiator Sync", false, "Sync the arpeggiator with the host"));
    p.addParameter(rate      = new juce::AudioParameterFloat(ParamIDs::arpeggiatorRate, "Arpeggiator rate", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f, "Rate"));

    activated->addListener(this);
}

void ArpeggiatorParams::parameterValueChanged(int, float)
{
    if (update != nullptr) update();
}