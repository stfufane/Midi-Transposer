#include <JuceHeader.h>
#include "Params.h"

/********************************
 * MidiParams
 ********************************/
MidiParams::~MidiParams()
{
    inputChannel->removeListener(this);
    outputChannel->removeListener(this);
    octaveTranspose->removeListener(this);
    bypassOtherChannels->removeListener(this);
}

void MidiParams::addParams(AudioProcessor& p)
{
    p.addParameter(inputChannel = new AudioParameterInt(ParamIDs::inChannel, "Input Channel", 1, 16, 1, "Input Channel"));
    p.addParameter(outputChannel = new AudioParameterInt(ParamIDs::outChannel, "Output Channel", 1, 16, 1, "Output Channel"));
    p.addParameter(octaveTranspose = new AudioParameterInt(ParamIDs::octaveTranspose, "Transpose Octaves", -1, 4, 0, "Transpose Octaves"));
    p.addParameter(bypassOtherChannels = new AudioParameterBool(ParamIDs::bypassChannels, "Bypass Other Channels", false, "Bypass Other Channels"));

    inputChannel->addListener(this);
    outputChannel->addListener(this);
    octaveTranspose->addListener(this);
    bypassOtherChannels->addListener(this);
}

void MidiParams::parameterValueChanged(int, float)
{
    if (update != nullptr) update();
}

/***********************************************
 * NoteParams
 ***********************************************/
NoteParams::NoteParams()
{
    notes.reserve(Notes::count);
    for (int i = 0; i < Notes::count; i++)
        notes.emplace_back(new NoteParam(i));
}

void NoteParams::addParams(AudioProcessor& p)
{
    for (auto& note : notes) {
        note->addParams(p);
    }
}

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
    p.addParameter(transpose = new AudioParameterInt(noteName + ParamIDs::noteTranspose, noteLabel + " transpose", -12, 12, 0, "Transpose semitones"));
    for (auto& interval: intervals) {
        interval->addParam(p);
        interval->interval->addListener(this);
    }
    
    transpose->addListener(this);
}

void NoteParam::parameterValueChanged(int, float)
{
    if (update != nullptr) update();
}

/*********************************************
 * IntervalParam
 *********************************************/
void IntervalParam::addParam(AudioProcessor& p)
{
    auto paramId = noteName + ParamIDs::noteInterval + String(degree + 1);
    p.addParameter(interval = new AudioParameterBool(paramId, paramId, false, "Interval " + String(degree + 1) + " for " + noteLabel));
}
