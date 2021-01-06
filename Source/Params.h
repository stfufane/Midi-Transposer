#pragma once

#include "JuceHeader.h"

namespace ParamIDs
{
    static String inChannel{ "in_channel" };
    static String outChannel{ "out_channel" };
    static String octaveTranspose{ "octave_transpose" };
    static String noteTranspose{ "_noteTranspose" };
    static String noteInterval{ "_interval_" };
}

namespace Notes
{
    static int count { 12 };
    static StringArray names { "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B" };
    static StringArray labels { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
    static StringArray intervals { "m2", "M2", "m3", "M3", "P4", "TT", "P5", "m6", "M6", "m7", "M7", "P8" };
    static std::array<bool, 12> whiteNotes { true, false, true, false, true, true, false, true, false, true, false, true };
    static Colour getColour(int index) { return whiteNotes[index] ? Colours::white : Colours::black; };
}

struct ParamHelper
{
    static juce::String getParamID(juce::AudioProcessorParameter* param)
    {
        if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
            return paramWithID->paramID;

        return param->getName(50);
    }
};

/*
    Simple structure that contains the 4 basic parameters at the top of the plugin.
    Whenever one of them is updated, the parameterChanged method will update the local atomic values for all of them.
*/
struct MidiParams : AudioProcessorParameter::Listener
{
    MidiParams(std::function<void()> lambda) : update(lambda) {}
    ~MidiParams();

    void addParams(AudioProcessor& p);

    void parameterValueChanged(int, float) override;

    void parameterGestureChanged(int, bool) override {}

    std::function<void()> update = nullptr;

    AudioParameterInt* inputChannel = nullptr;
    AudioParameterInt* outputChannel = nullptr;
    AudioParameterInt* octaveTranspose = nullptr;
};

// Forward declarations for readability
struct IntervalParam;
struct NoteParam;

/*
    This structure contains all 12 note/chord couples.
*/
struct NoteParams 
{
    NoteParams();
    void addParams(AudioProcessor& p);

    std::vector<std::unique_ptr<NoteParam>> notes;
};

/*
This represents the structure for a note with its tranposition and selected intervals.
The update function is called on parameter changed to update the mappingNotes vector of the processor.
*/
struct NoteParam : AudioProcessorParameter::Listener
{
    NoteParam(const int i);
    ~NoteParam();

    void addParams(AudioProcessor& p);
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {}

    int noteIndex;
    String noteName;
    String noteLabel;

    AudioParameterInt* transpose = nullptr;
    std::vector<std::unique_ptr<IntervalParam>> intervals;

    std::function<void()> update = nullptr;
};

/**
 * This is just an interval toggle for a specific note and degree.
 */
struct IntervalParam
{
    IntervalParam(NoteParam& noteParam, int i) 
        : degree(i), noteName(noteParam.noteName), noteLabel(noteParam.noteLabel)
    {}

    void addParam(AudioProcessor& p);

    AudioParameterBool* interval;
    int degree;
    String noteName;
    String noteLabel;
};