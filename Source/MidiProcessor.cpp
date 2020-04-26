/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 15 Apr 2020 7:12:57pm
    Author:  Stfufane

  ==============================================================================
*/

#include "MidiProcessor.h"

StringArray MidiProcessor::midiChannels{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
StringArray MidiProcessor::notes{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
StringArray MidiProcessor::chords{ "None", "maj", "min", "sus4", "maj7", "min7", "7", "m7b5" };

MidiProcessor::MidiProcessor(AudioProcessorValueTreeState& stateToUse)
    : mapping_notes(128, std::vector<int>(1, 0)),
      noteParameters(notes.size()),
      chordParameters(notes.size()),
      state(stateToUse)
{
    for (int i = 0; i < 128; i++) {
        mapping_notes[i][0] = i;
    }

    state.state.addListener(this);

    inputChannelParameter = state.getRawParameterValue(IDs::paramInChannel);
    outputChannelParameter = state.getRawParameterValue(IDs::paramOutChannel);

    updateMapping();
}

void MidiProcessor::process(MidiBuffer& midiMessages)
{
    MidiBuffer processedMidi;
    int time;
    MidiMessage m;
    for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
        if (m.getChannel() == inputChannel) {
            if (m.isNoteOnOrOff()) {
                mapNote(m.getNoteNumber(), m.getVelocity(), m.isNoteOn(), time, processedMidi);
            }
            else {
                processedMidi.addEvent(m, time);
            }
        }
    }
    midiMessages.swapWith(processedMidi);
}

void MidiProcessor::mapNote(int note, juce::uint8 velocity, bool noteOn, int time, MidiBuffer& processedMidi)
{
    MidiMessage m;
    for (int i = 0; i < mapping_notes[note].size(); i++) {
        if (noteOn) {
            m = MidiMessage::noteOn(outputChannel, mapping_notes[note][i], velocity);
        }
        else {
            m = MidiMessage::noteOff(outputChannel, mapping_notes[note][i], velocity);
        }
        processedMidi.addEvent(m, time);
    }
}

void MidiProcessor::updateMapping()
{
    inputChannel = roundToInt(inputChannelParameter->load()) + 1;
    outputChannel = roundToInt(outputChannelParameter->load()) + 1;

    for (int i = 0; i < notes.size(); i++)
    {
        noteParameters[i] = dynamic_cast<AudioParameterChoice*>(state.getParameter(notes[i] + "_note"));
        chordParameters[i] = dynamic_cast<AudioParameterChoice*>(state.getParameter(notes[i] + "_chord"));
        addMappedNotes(i, noteParameters[i]->getIndex(), chordParameters[i]->getCurrentChoiceName());
    }
}

void MidiProcessor::addMappedNotes(const int from_note, const int to_note, const String chord)
{
    int note, mapped_note, new_size = 1;
    if (chord == "maj" || chord == "min" || chord == "sus4") {
        new_size = 3;
    }
    else if (chord == "Maj7" || chord == "m7" || chord == "7" || chord == "m7b5") {
        new_size = 4;
    }

    for (int i = 0; i <= 10; i++) {
        note = i * 12 + from_note;
        mapped_note = i * 12 + to_note;
        if (note < 128 && mapped_note < 128) {
            // Redimensionne le tableau de mapping
            mapping_notes[note].resize(new_size);

            // Définit la note racine
            mapping_notes[note][0] = mapped_note;

            // Si aucun accord paramétré on s'arrête là.
            if (chord == "None") {
                continue;
            }

            // Définit si on est sur une tierce majeure, mineure ou une quarte
            if (chord == "maj" || chord == "Maj7" || chord == "7") {
                mapping_notes[note][1] = mapped_note + 4;
            }
            else if (chord == "m7" || chord == "min") {
                mapping_notes[note][1] = mapped_note + 3;
            }
            else if (chord == "sus4") {
                mapping_notes[note][1] = mapped_note + 5;
            }

            // La quinte dans tous les cas sauf m7b5
            if (chord == "m7b5") {
                mapping_notes[note][2] = mapped_note + 6;
            }
            else {
                mapping_notes[note][2] = mapped_note + 7;
            }

            // 7eme majeure ou mineure.
            if (chord == "Maj7") {
                mapping_notes[note][3] = mapped_note + 11;
            }
            else if (chord == "m7" || chord == "7") {
                mapping_notes[note][3] = mapped_note + 10;
            }
        }
    }

}

void MidiProcessor::addMappingParameters(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    for (int i = 0; i < notes.size(); i++)
    {
        auto transpose = std::make_unique<AudioParameterChoice>(notes[i] + "_note", notes[i], notes, i);
        auto chord = std::make_unique<AudioParameterChoice>(notes[i] + "_chord", notes[i], chords, 0);
        layout.add(std::move(transpose), std::move(chord));
    }
}

void MidiProcessor::addChannelParameters(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto inChannel = std::make_unique<AudioParameterChoice>(IDs::paramInChannel, "Input Channel", midiChannels, 0);
    auto outChannel = std::make_unique<AudioParameterChoice>(IDs::paramOutChannel, "Output Channel", midiChannels, 0);

    layout.add(std::move(inChannel), std::move(outChannel));
}

void MidiProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyChanged, const Identifier& property)
{
    if (property.toString() == "value")
        updateMapping();
}