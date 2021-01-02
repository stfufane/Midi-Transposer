#pragma once

#include "JuceHeader.h"

namespace ParamIDs
{
    static String inChannel{ "in_channel" };
    static String outChannel{ "out_channel" };
    static String octaveTranspose{ "octave_transpose" };
    static String bypassChannels{ "bypass_other_channels" };
    static String noteChoice{ "_note" };
    static String chordChoice{ "_chord" };
    static String noteTranspose{ "_noteTranspose" };
    static String noteInterval{ "_interval_" };
}

namespace Notes
{
    static StringArray names { "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B" };
    static StringArray labels { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
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
    MidiParams(std::function<void()> lambda)
        : update(lambda)
    {}

    void addParams(AudioProcessor& p)
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

    void parameterValueChanged(int, float) override
    {
        if (update != nullptr) update();
    }

    void parameterGestureChanged(int, bool) override {}

    std::function<void()> update = nullptr;

    AudioParameterInt* inputChannel = nullptr;
    AudioParameterInt* outputChannel = nullptr;
    AudioParameterInt* octaveTranspose = nullptr;
    AudioParameterBool* bypassOtherChannels = nullptr;
};

/*
    This represents the structure for a note with its tranposition and selected intervals.
    The update function is called on parameter changed to update the mappingNotes vector of the processor.
*/
struct NoteParam : AudioProcessorParameter::Listener
{
    NoteParam(const int i)
        : noteIndex(i), noteName(Notes::names[i]), noteLabel(Notes::labels[i])
    {
        intervals.reserve(Notes::names.size());
    }

    void addParams(AudioProcessor& p)
    {
        p.addParameter(transpose = new AudioParameterInt(noteName + ParamIDs::noteTranspose, noteLabel + " transpose", -12, 12, 0, "Transpose semitones"));
        for (auto i = 0; i < Notes::names.size(); i++) {
            auto paramId = noteName + ParamIDs::noteInterval + String(i + 1);
            auto interval = new AudioParameterBool(paramId, paramId, false, "Interval " + String(i + 1) + " for " + noteLabel);
            intervals.emplace_back(interval);
            p.addParameter(interval);
            interval->addListener(this);
        }
        transpose->addListener(this);
    }

    void parameterValueChanged(int, float) override
    {
        if (update != nullptr) update();
    }

    void parameterGestureChanged(int, bool) override {}

    int noteIndex;
    String noteName;
    String noteLabel;

    AudioParameterInt* transpose = nullptr;
    std::vector<std::unique_ptr<AudioParameterBool>> intervals;

    std::function<void()> update = nullptr;
};

/*
    This structure contains all 12 note/chord couples.
*/
struct NoteParams 
{
    NoteParams()
    {
        notes.reserve(Notes::names.size());
        for (int i = 0; i < Notes::names.size(); i++) {
            notes.emplace_back(i);
        }
    }

    void addParams(AudioProcessor& p)
    {
        for (auto& note : notes) {
            note.addParams(p);
        }
    }

    std::vector<NoteParam> notes;
};
