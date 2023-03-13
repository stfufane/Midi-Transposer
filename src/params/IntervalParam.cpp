#include "Params.h"

namespace Params
{
/*********************************************
 * IntervalParam
 *********************************************/
IntervalParam::IntervalParam(std::string name, std::string label, int i)
        : degree(i), noteName(std::move(name)), noteLabel(std::move(label))
{}

void IntervalParam::addParam(juce::AudioProcessor& p)
{
    const auto paramId = juce::String(noteName) + ParamIDs::noteInterval + juce::String(degree + 1);
    p.addParameter(interval = new juce::AudioParameterInt(paramId, paramId, -12, 12, 0,
                                                           juce::AudioParameterIntAttributes().withLabel(
                                                           "Interval " + juce::String(degree + 1) + " for " +
                                                           noteLabel)));
}

}