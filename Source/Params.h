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
}

namespace Notes
{
    static StringArray names { "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B" };
    static StringArray labels { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
    static StringArray chords { "None", "maj", "min", "sus4", "maj7", "min7", "7", "m7b5" };
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
    This represents the structure for a note/chord couple parameters.
    The update function is called on parameter changed to update the mappingNotes vector of the processor.
*/
struct NoteParam : AudioProcessorParameter::Listener
{
    NoteParam(const int i)
        : noteIndex(i), noteName(Notes::names[i]), noteLabel(Notes::labels[i])
    {}

    void addParams(AudioProcessor& p)
    {
        p.addParameter(note = new AudioParameterChoice(noteName + ParamIDs::noteChoice, noteLabel, Notes::names, noteIndex, noteLabel));
        p.addParameter(chord = new AudioParameterChoice(noteName + ParamIDs::chordChoice, noteLabel + ParamIDs::chordChoice, Notes::chords, 0, noteLabel + " chord"));
        note->addListener(this);
        chord->addListener(this);
    }

    void parameterValueChanged(int, float) override
    {
        if (update != nullptr) update();
    }

    void parameterGestureChanged(int, bool) override {}

    int noteIndex;
    String noteName;
    String noteLabel;
    AudioParameterChoice* note = nullptr;
    AudioParameterChoice* chord = nullptr;

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
