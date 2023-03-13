#include "Params.h"

namespace Params
{
/***********************************************
 * NoteParam
 ***********************************************/
NoteParam::NoteParam(const int index)
        : noteIndex(index), noteName(Notes::names[static_cast<size_t>(index)]), noteLabel(Notes::labels[index])
{
    intervals.reserve(Notes::names.size());
    for (int i = 0; i < Notes::count; i++) {
        intervals.emplace_back(new IntervalParam(noteName, noteLabel, i));
    }
}

NoteParam::~NoteParam()
{
    transpose->removeListener(this);
    for (auto& interval: intervals) {
        interval->interval->removeListener(this);
    }
}

void NoteParam::addParams(juce::AudioProcessor& p)
{
    p.addParameter(mapNote = new juce::AudioParameterBool(juce::String(noteName) + ParamIDs::mapNote,
                                                          "Map " + noteLabel, true,
                                                          juce::AudioParameterBoolAttributes().withLabel(
                                                                  "Map " + noteLabel)));
    p.addParameter(transpose = new juce::AudioParameterInt(juce::String(noteName) + ParamIDs::noteTranspose,
                                                           noteLabel + " transpose",
                                                           -12, 12, 0,
                                                           juce::AudioParameterIntAttributes().withLabel(
                                                                   "Transpose semitones")));
    for (auto& interval: intervals) {
        interval->addParam(p);
        interval->interval->addListener(this);
    }

    mapNote->addListener(this);
    transpose->addListener(this);
}

void NoteParam::parameterValueChanged(int, float)
{
    if (update != nullptr) {
        update();
    }
}

}