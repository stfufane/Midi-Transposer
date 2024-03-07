#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <string_view>

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
    static constexpr int count { 12 };
    static constexpr std::array<std::string_view, count> names  { "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B" };
    static constexpr std::array<std::string_view, count> labels { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

    static constexpr std::array<bool, count> whiteNotes { true, false, true, false, true, true, false, true, false, true, false, true };

    struct Division
    {
        std::string label;
        double division;
    };

    static const std::vector<Notes::Division> divisions {
        { "1/1", 4.0 },
        { "1/2", 2.0 },
        { "1/4.d", 1.5 },
        { "1/4", 1.0 },
        { "1/8d", 0.75 },
        { "1/4.t", 2.0 / 3.0 },
        { "1/8", 0.5 },
        { "1/8.t", 1.0 / 3.0 },
        { "1/16", 0.25 }
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

    explicit IntervalParam(std::string name, std::string label, int i);

    void addParam(juce::AudioProcessor& p);

    juce::AudioParameterInt* interval = nullptr;
    int degree;
    std::string noteName;
    std::string noteLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntervalParam)
};

/*
This represents the structure for a note with its transposition and selected intervals.
The update function is called on parameter changed to update the mappingNotes vector of the processor.
*/
struct NoteParam final : juce::AudioProcessorParameter::Listener
{
    NoteParam() = delete;

    explicit NoteParam(int index);

    ~NoteParam() override;

    void addParams(juce::AudioProcessor& p);

    void parameterValueChanged(int, float) override;

    void parameterGestureChanged(int, bool) override
    {}

    int noteIndex;
    std::string noteName;
    std::string noteLabel;

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