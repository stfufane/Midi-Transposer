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

namespace Names
{
    static StringArray notes{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    static StringArray noteLabels{ "C", "C_Sharp", "D", "D_Sharp", "E", "F", "F_Sharp", "G", "G_Sharp", "A", "A_Sharp", "B" };
    static StringArray chords{ "None", "maj", "min", "sus4", "maj7", "min7", "7", "m7b5" };
}

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
        p.addParameter(inputChannel = new AudioParameterInt(ParamIDs::inChannel, "InputChannel", 1, 16, 1, "Input Channel"));
        p.addParameter(outputChannel = new AudioParameterInt(ParamIDs::outChannel, "OutputChannel", 1, 16, 1, "Output Channel"));
        p.addParameter(octaveTranspose = new AudioParameterInt(ParamIDs::octaveTranspose, "TransposeOctaves", -1, 4, 0, "Transpose octaves"));
        p.addParameter(bypassOtherChannels = new AudioParameterBool(ParamIDs::bypassChannels, "BypassOtherChannels", false, "Bypass other channels"));

        inputChannel->addListener(this);
        outputChannel->addListener(this);
        octaveTranspose->addListener(this);
        bypassOtherChannels->addListener(this);
    }

    void parameterValueChanged(int parameterIndex, float newValue) override
    {
        if (update != nullptr) update();
    }

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

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
    NoteParam(const int i, const String& n, const String& l)
        : noteIndex(i), noteName(n), noteLabel(l)
    {}

    void addParams(AudioProcessor& p)
    {
        p.addParameter(note = new AudioParameterChoice(noteName + ParamIDs::noteChoice, noteLabel, Names::notes, noteIndex, noteName));
        p.addParameter(chord = new AudioParameterChoice(noteName + ParamIDs::chordChoice, noteLabel + ParamIDs::chordChoice, Names::chords, 0, noteName + " chord"));
        note->addListener(this);
        chord->addListener(this);
    }

    void parameterValueChanged(int parameterIndex, float newValue) override
    {
        if (update != nullptr) update();
    }

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

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
        for (int i = 0; i < Names::notes.size(); i++) {
            notes.emplace_back(i, Names::notes[i], Names::noteLabels[i]);
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
