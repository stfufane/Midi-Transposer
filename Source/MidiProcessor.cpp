/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 15 Apr 2020 7:12:57pm
    Author:  Stfufane

  ==============================================================================
*/

#include "MidiProcessor.h"

MidiProcessor::MidiProcessor()
    : mappingNotes(12, std::vector<int>(1, 0))
{
    updateParameters();
}

void MidiProcessor::process(MidiBuffer& midiMessages)
{
    MidiBuffer processedMidi;
    bool matchingChannel; 

    for (const auto metadata: midiMessages)
    {
        const auto m = metadata.getMessage();
        matchingChannel = (m.getChannel() == inputChannel);
        // Only notes on and off from input channel are processed, the rest is passed through.
        if (matchingChannel && m.isNoteOnOrOff())
        {
            mapNote(m.getNoteNumber(), m.getVelocity(), m.isNoteOn(), metadata.samplePosition, processedMidi);
        }
        else 
        {
            if (!bypassOtherChannels || matchingChannel)
                processedMidi.addEvent(m, metadata.samplePosition);
        }
    }
    midiMessages.swapWith(processedMidi);
}

/*
    There are many cases to take in account here.
    We want the input to be monophonic so we need to know which was the last played note and 
    if there are some notes still on to be played when the last one is released.
*/
void MidiProcessor::mapNote(const int note, const juce::uint8 velocity, const bool noteOn, const int samplePosition, MidiBuffer& processedMidi)
{
    if (noteOn)
    {
        // Add the note to the vector of current notes played.
        currentInputNotesOn.push_back(note);

        // If the note changed, turn off the previous notes before adding the new ones.
        if (note != lastNoteOn && lastNoteOn > -1)
        {
            stopCurrentNotes(velocity, samplePosition, processedMidi);
        }
        // Play the received note.
        playMappedNotes(note, velocity, samplePosition, processedMidi);
    }
    else 
    {
        // For every note off, remove the received note from the vector of current notes held.
        removeHeldNote(note);
        
        // Turn off the corresponding notes for the current note off if it's the same as the last played note.
        // Otherwise it means the released note was not active so we don't need to do anything (case of multiple notes held)
        if (note == lastNoteOn) 
        {
            stopCurrentNotes(velocity, samplePosition, processedMidi);

            // If there were still some notes held, play the last one.
            if (currentInputNotesOn.size() > 0) {
                // Then play the last note from the vector of active input notes.
                playMappedNotes(currentInputNotesOn.back(), velocity, samplePosition, processedMidi);
            }
            else {
                // No note is currently played.
                lastNoteOn = -1;
            }
        }
    }
}

void MidiProcessor::playMappedNotes(const int note, const juce::uint8 velocity, const int samplePosition, MidiBuffer& processedMidi)
{
    int baseNote, noteToPlay;
    lastNoteOn = note;
    // First clear the output notes vector to replace its values.
    currentOutputNotesOn.clear();

    baseNote = lastNoteOn % 12;
    for (int i = 0; i < mappingNotes[baseNote].size(); i++) {
        noteToPlay = lastNoteOn + mappingNotes[baseNote][i];
        if (noteToPlay >= 0 && noteToPlay < 128) {
            processedMidi.addEvent(MidiMessage::noteOn(outputChannel, noteToPlay, velocity), samplePosition);
            currentOutputNotesOn.push_back(noteToPlay);
        }
    }
    currentNoteOutputChannel = outputChannel;
}

void MidiProcessor::stopCurrentNotes(const juce::uint8 velocity, const int samplePosition, MidiBuffer& processedMidi)
{
    for (int i = 0; i < currentOutputNotesOn.size(); i++) {
        processedMidi.addEvent(MidiMessage::noteOff(currentNoteOutputChannel, currentOutputNotesOn[i], velocity), samplePosition);
    }
}

void MidiProcessor::removeHeldNote(const int note)
{
    for (auto it = currentInputNotesOn.begin(); it != currentInputNotesOn.end(); ++it)
    {
        if (*it == note)
        {
            currentInputNotesOn.erase(it);
            break;
        }
    }
}

void MidiProcessor::parameterChanged(const String& parameterID, float newValue)
{
    DBG(parameterID);
    DBG(newValue);
}

void MidiProcessor::updateParameters()
{
    /* inputChannel = inputChannelParameter->get();
    outputChannel = outputChannelParameter->get();
    bypassOtherChannels = bypassOtherChannelsParameter->get();
    
    octaveTranspose = octaveTransposeParameter->get();
    for (int i = 0; i < notes.size(); i++)
    {
        setMappedNotes(i, noteParameters[i]->getIndex(), chordParameters[i]->getIndex());
    } */
}

void MidiProcessor::setMappedNotes(const int from_note, const int to_note, const int chord)
{
    // First clear the mapping values.
    mappingNotes[from_note].clear();

    // Add the root note in every case.
    mappingNotes[from_note].push_back(to_note - from_note);

    // If there's a chord, add its note + the octave transposition.
    size_t chord_size = chordIntervals[chord].size();
    if (chord_size > 1)
    {
        for (int i = 0; i < chord_size; i++)
        {
            mappingNotes[from_note].push_back(to_note - from_note + chordIntervals[chord][i] + (octaveTranspose * 12));
        }
    } 
}

AudioProcessorValueTreeState::ParameterLayout MidiProcessor::getParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;
    std::vector<std::unique_ptr<RangedAudioParameter>> layout_params;

    for (int i = 0; i < notes.size(); i++)
    {
        layout_params.emplace_back(new AudioParameterChoice(notes[i] + "_note", notes[i], notes, i, notes[i]));
        layout_params.emplace_back(new AudioParameterChoice(notes[i] + "_chord", notes[i] + " chord", chords, 0, notes[i] + " chord"));
    }

    layout_params.emplace_back(new AudioParameterInt("in_channel", "Input Channel", 1, 16, 1, "Input Channel"));
    layout_params.emplace_back(new AudioParameterInt("out_channel", "Output Channel", 1, 16, 1, "Output Channel"));
    layout_params.emplace_back(new AudioParameterBool("bypass_other_channels", "Bypass other channels", false, "Bypass other channels"));
    layout_params.emplace_back(new AudioParameterInt("octave_transpose", "Transpose octaves", -1, 4, 0, "Transpose octaves"));

    for (auto& param : layout_params)
        params.push_back(param.get());

    layout.add(layout_params.begin(), layout_params.end());

    return layout;
}

void MidiProcessor::registerListeners(AudioProcessorValueTreeState& treeState)
{
    for (auto& param : params) {
        treeState.addParameterListener(param->paramID, this);
    }
}

int MidiProcessor::getLastNoteOn()
{
    return lastNoteOn;
}