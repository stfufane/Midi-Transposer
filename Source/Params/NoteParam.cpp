#include "Params.h"

/***********************************************
 * NoteParam
 ***********************************************/ 
NoteParam::NoteParam(const int i)
    : noteIndex(i), noteName(Notes::names[i]), noteLabel(Notes::labels[i])
{
    intervals.reserve(Notes::names.size());
    for (int i = 0; i < Notes::count; i++)
        intervals.emplace_back(new IntervalParam(*this, i));
}

NoteParam::~NoteParam()
{
    transpose->removeListener(this);
    for (auto& interval: intervals)
        interval->interval->removeListener(this);
}

void NoteParam::addParams(AudioProcessor& p)
{
    p.addParameter(mapNote = new juce::AudioParameterBool(noteName + ParamIDs::mapNote, "Map " + noteLabel, true, "Map " + noteLabel));
    p.addParameter(transpose = new juce::AudioParameterInt(noteName + ParamIDs::noteTranspose, noteLabel + " transpose", -12, 12, 0, "Transpose semitones"));
    for (auto& interval: intervals) {
        interval->addParam(p);
        interval->interval->addListener(this);
    }
    
    mapNote->addListener(this);
    transpose->addListener(this);
}

void NoteParam::parameterValueChanged(int, float)
{
    if (update != nullptr) update();
}