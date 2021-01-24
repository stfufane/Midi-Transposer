#include "Params.h"

/*********************************************
 * IntervalParam
 *********************************************/
IntervalParam::IntervalParam(NoteParam& noteParam, int i) 
    : degree(i), noteName(noteParam.noteName), noteLabel(noteParam.noteLabel)
{}

void IntervalParam::addParam(juce::AudioProcessor& p)
{
    auto paramId = noteName + ParamIDs::noteInterval + String(degree + 1);
    p.addParameter(interval = new juce::AudioParameterBool(paramId, paramId, false, "Interval " + juce::String(degree + 1) + " for " + noteLabel));
}
