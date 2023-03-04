#pragma once

#include <JuceHeader.h>

namespace ParamIDs
{
    static const juce::String inChannel               { "in_channel" };
    static const juce::String outChannel              { "out_channel" };
    static const juce::String octaveTranspose         { "octave_transpose" };
    static const juce::String noteTranspose           { "_noteTranspose" };
    static const juce::String noteInterval            { "_interval_" };
    static const juce::String mapNote                 { "_mapNote" };
    static const juce::String arpeggiatorActivated    { "arpeggiator_activated" };
    static const juce::String arpeggiatorSync         { "arpeggiator_sync" };
    static const juce::String arpeggiatorSyncRate     { "arpeggiator_sync_rate" };
    static const juce::String arpeggiatorRate         { "arpeggiator_rate" };
}

namespace Notes
{
    static const int count { 12 };
    static const juce::StringArray names      { "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B" };
    static const juce::StringArray labels     { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
    static const juce::StringArray intervals  { "m2", "M2", "m3", "M3", "P4", "TT", "P5", "m6", "M6", "m7", "M7", "P8" };

    struct Division
    {
        juce::String label;
        double division;
    };

    static const std::vector<Notes::Division> divisions {
        { "Whole note", 4.0 },
        { "Half note", 2.0 },
        { "Dotted quarter note", 1.5 },
        { "Quarter note", 1.0 },
        { "Dotted 8th note", 0.75 },
        { "Quarter note triplet", 2.0 / 3.0 },
        { "8th note", 0.5 },
        { "8th note triplet", 1.0 / 3.0 },
        { "16th note", 0.25 }
    };
}

namespace Params
{

struct ParamHelper
{
    static juce::String getParamID(juce::AudioProcessorParameter* param)
    {
        if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
            return paramWithID->paramID;

        return param->getName(50);
    }
};

/**
 * @brief Simple structure that contains the 3 MIDI parameters at the top of the plugin.
*/
struct MidiParams
{
    MidiParams() = default;

    void addParams(juce::AudioProcessor& p);

    juce::AudioParameterInt* inputChannel = nullptr;
    juce::AudioParameterInt* outputChannel = nullptr;
    juce::AudioParameterInt* octaveTranspose = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiParams)
};

/**
 * These are all the parameters related to the arpeggiator
 */
struct ArpeggiatorParams
{
    ArpeggiatorParams() = default;

    void addParams(juce::AudioProcessor& p);

    juce::AudioParameterBool* activated = nullptr;
    juce::AudioParameterBool* synced = nullptr;
    juce::AudioParameterInt* syncRate = nullptr;
    juce::AudioParameterFloat* rate = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpeggiatorParams)
};

/**
 * This is just an interval toggle for a specific note and degree.
 */
struct IntervalParam
{
    IntervalParam() = delete;

    explicit IntervalParam(juce::String name, juce::String label, int i);

    void addParam(juce::AudioProcessor& p);

    juce::AudioParameterBool* interval = nullptr;
    int degree;
    juce::String noteName;
    juce::String noteLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntervalParam)
};

/*
This represents the structure for a note with its transposition and selected intervals.
The update function is called on parameter changed to update the mappingNotes vector of the processor.
*/
struct NoteParam : juce::AudioProcessorParameter::Listener
{
    NoteParam() = delete;

    explicit NoteParam(int i);

    ~NoteParam() override;

    void addParams(juce::AudioProcessor& p);

    void parameterValueChanged(int, float) override;

    void parameterGestureChanged(int, bool) override
    {}

    int noteIndex;
    juce::String noteName;
    juce::String noteLabel;

    juce::AudioParameterBool* mapNote = nullptr;
    juce::AudioParameterInt* transpose = nullptr;
    std::vector<std::unique_ptr<IntervalParam>> intervals;

    std::function<void()> update = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteParam)
};

/*
    This structure contains all 12 note/chord couples.
*/
struct NoteParams
{
    NoteParams();

    void addParams(juce::AudioProcessor& p);

    std::vector<std::unique_ptr<NoteParam>> notes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteParams)
};

}