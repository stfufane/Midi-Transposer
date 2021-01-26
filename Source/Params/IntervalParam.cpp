#include "Params.h"

/*********************************************
 * IntervalParam
 *********************************************/
IntervalParam::IntervalParam(const String& name, const String& label, int i) 
    : degree(i), noteName(name), noteLabel(label)
{}

void IntervalParam::addParam(juce::AudioProcessor& p)
{
    auto paramId = noteName + ParamIDs::noteInterval + String(degree + 1);
    p.addParameter(interval = new juce::AudioParameterBool(paramId, paramId, false, "Interval " + juce::String(degree + 1) + " for " + noteLabel));
}
