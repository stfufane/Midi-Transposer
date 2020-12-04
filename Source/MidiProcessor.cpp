/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 15 Apr 2020 7:12:57pm
    Author:  Stfufane

  ==============================================================================
*/

#include "MidiProcessor.h"

StringArray MidiProcessor::notes{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
StringArray MidiProcessor::chords{ "None", "maj", "min", "sus4", "maj7", "min7", "7", "m7b5" };

MidiProcessor::MidiProcessor(AudioProcessorValueTreeState& stateToUse)
    : mappingNotes(12, std::vector<int>(1, 0)),
      noteParameters(notes.size()),
      chordParameters(notes.size()),
      state(stateToUse)
{
    state.state.addListener(this);

    inputChannelParameter = dynamic_cast<AudioParameterInt*>(state.getParameter(IDs::paramInChannel));
    outputChannelParameter = dynamic_cast<AudioParameterInt*>(state.getParameter(IDs::paramOutChannel));
    bypassOtherChannelsParameter = dynamic_cast<AudioParameterBool*>(state.getParameter(IDs::bypassOtherChannels));

    for (int i = 0; i < notes.size(); i++)
    {
        noteParameters[i] = dynamic_cast<AudioParameterChoice*>(state.getParameter(notes[i] + "_note"));
        chordParameters[i] = dynamic_cast<AudioParameterChoice*>(state.getParameter(notes[i] + "_chord"));
    }

    octaveTransposeParameter = dynamic_cast<AudioParameterInt*>(state.getParameter(IDs::octaveTranspose));

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
void MidiProcessor::mapNote(const int note, const juce::uint8 velocity, const bool noteOn, const int time, MidiBuffer& processedMidi)
{
    if (noteOn)
    {
        // Add the note to the vector of current notes played.
        currentInputNotesOn.push_back(note);

        // If the note changed, turn off the previous notes before adding the new ones.
        if (note != lastNoteOn && lastNoteOn > -1)
        {
            stopCurrentNotes(velocity, time, processedMidi);
        }
        // Play the received note.
        playMappedNotes(note, velocity, time, processedMidi);
    }
    else 
    {
        // For every note off, remove the received note from the vector of current notes held.
        removeHeldNote(note);
        
        // Turn off the corresponding notes for the current note off if it's the same as the last played note.
        // Otherwise it means the released note was not active so we don't need to do anything (case of multiple notes held)
        if (note == lastNoteOn) 
        {
            stopCurrentNotes(velocity, time, processedMidi);

            // If there were still some notes held, play the last one.
            if (currentInputNotesOn.size() > 0) {
                // Then play the last note from the vector of active input notes.
                playMappedNotes(currentInputNotesOn.back(), velocity, time, processedMidi);
            }
            else {
                // No note is currently played.
                lastNoteOn = -1;
            }
        }
    }
}

void MidiProcessor::playMappedNotes(const int note, const juce::uint8 velocity, const int time, MidiBuffer& processedMidi)
{
    int baseNote, noteToPlay;
    lastNoteOn = note;
    // First clear the output notes vector to replace its values.
    currentOutputNotesOn.clear();

    baseNote = lastNoteOn % 12;
    for (int i = 0; i < mappingNotes[baseNote].size(); i++) {
        noteToPlay = lastNoteOn + mappingNotes[baseNote][i];
        if (noteToPlay >= 0 && noteToPlay < 128) {
            processedMidi.addEvent(MidiMessage::noteOn(outputChannel, noteToPlay, velocity), time);
            currentOutputNotesOn.push_back(noteToPlay);
        }
    }
    currentNoteOutputChannel = outputChannel;
}

void MidiProcessor::stopCurrentNotes(const juce::uint8 velocity, const int time, MidiBuffer& processedMidi)
{
    for (int i = 0; i < currentOutputNotesOn.size(); i++) {
        processedMidi.addEvent(MidiMessage::noteOff(currentNoteOutputChannel, currentOutputNotesOn[i], velocity), time);
    }
}

void MidiProcessor::removeHeldNote(const int note)
{
    for (auto it = currentInputNotesOn.begin(); it != currentInputNotesOn.end();)
    {
        if (*it == note)
        {
            currentInputNotesOn.erase(it);
            break;
        }
        ++it;
    }
}

void MidiProcessor::updateParameters()
{
    inputChannel = inputChannelParameter->get();
    outputChannel = outputChannelParameter->get();
    bypassOtherChannels = bypassOtherChannelsParameter->get();
    
    octaveTranspose = octaveTransposeParameter->get();
    for (int i = 0; i < notes.size(); i++)
    {
        setMappedNotes(i, noteParameters[i]->getIndex(), chordParameters[i]->getIndex());
    }
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

void MidiProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyChanged, const Identifier& property)
{
    if (property.toString() == "value")
        updateParameters();
}

int MidiProcessor::getLastNoteOn()
{
    return lastNoteOn;
}