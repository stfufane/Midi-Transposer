#include "Params.h"

/*********************************************
 * IntervalParam
 *********************************************/
IntervalParam::IntervalParam(juce::String name, juce::String label, int i)
    : degree(i), noteName(std::move(name)), noteLabel(std::move(label))
{}

void IntervalParam::addParam(juce::AudioProcessor& p)
{
    const auto paramId = noteName + ParamIDs::noteInterval + juce::String(degree + 1);
    p.addParameter(interval = new juce::AudioParameterBool(paramId,
                                                           paramId,
                                                           false,
                                                           juce::AudioParameterBoolAttributes().withLabel("Interval " + juce::String(degree + 1) + " for " + noteLabel)));
}
