#include "Params.h"

void ArpeggiatorParams::addParams(juce::AudioProcessor& p)
{
    p.addParameter(activated = new juce::AudioParameterBool(ParamIDs::arpeggiatorActivated,
                                                            "Arpeggiator activated",
                                                            false,
                                                            juce::AudioParameterBoolAttributes().withLabel("Activated")));
    p.addParameter(synced    = new juce::AudioParameterBool(ParamIDs::arpeggiatorSync,
                                                            "Arpeggiator Sync",
                                                            false,
                                                            juce::AudioParameterBoolAttributes().withLabel("Sync the arpeggiator with the host")));
    p.addParameter(syncRate  = new juce::AudioParameterInt(ParamIDs::arpeggiatorSyncRate,
                                                           "Arpeggiator Rate Sync",
                                                           0, (int)Notes::divisions.size() - 1, 2,
                                                           juce::AudioParameterIntAttributes().withLabel("Rate")));
    p.addParameter(rate      = new juce::AudioParameterFloat(ParamIDs::arpeggiatorRate,
                                                             "Arpeggiator Rate",
                                                             juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
                                                             juce::AudioParameterFloatAttributes().withLabel("Rate")));
}
