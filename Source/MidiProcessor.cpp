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

    for (int i = 0; i < notes.size(); i++)
    {
        noteParameters[i] = dynamic_cast<AudioParameterChoice*>(state.getParameter(notes[i] + "_note"));
        chordParameters[i] = dynamic_cast<AudioParameterChoice*>(state.getParameter(notes[i] + "_chord"));
    }

    updateMapping();
}

void MidiProcessor::process(MidiBuffer& midiMessages)
{
    MidiBuffer processedMidi;
    int time;
    MidiMessage m;
    for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
        if (m.getChannel() == inputChannel) 
        {
            if (m.isNoteOnOrOff()) 
            {
                mapNote(m.getNoteNumber(), m.getVelocity(), m.isNoteOn(), time, processedMidi);
            }
            else 
            {
                processedMidi.addEvent(m, time);
            }
        }
    }
    midiMessages.swapWith(processedMidi);
}

/*
    There are many cases to take in account here.
    We want the input to be monophonic so we need to know which was the last played note and 
    if there are some notes still on to be played when the last one is released.
*/
void MidiProcessor::mapNote(int note, juce::uint8 velocity, bool noteOn, int time, MidiBuffer& processedMidi)
{
    MidiMessage m;
    int baseNote;

    // If the note changed, turn off the previous notes before adding the new ones.
    if (noteOn)
    {
        // Add the note to the vector of current notes played.
        currentNotesOn.push_back(note);
        if (note != lastNoteOn && lastNoteOn > -1)
        {
            baseNote = lastNoteOn % 12;
            for (int i = 0; i < mappingNotes[baseNote].size(); i++) {
                processedMidi.addEvent(MidiMessage::noteOff(outputChannel, lastNoteOn + mappingNotes[baseNote][i], velocity), time);
            }
        }
        lastNoteOn = note;

        // Loop on the corresponding mapping for the played note.
        baseNote = note % 12;
        for (int i = 0; i < mappingNotes[baseNote].size(); i++) {
            processedMidi.addEvent(MidiMessage::noteOn(outputChannel, note + mappingNotes[baseNote][i], velocity), time);
        }
    }
    else 
    {
        for (auto it = currentNotesOn.begin(); it != currentNotesOn.end();)
        {
            if (*it == note)
            {
                currentNotesOn.erase(it);
                break;
            }
            ++it;
        }
        
        // Turn off the corresponding notes for the current note off if it's the same as the last played note.
        // Otherwise it means the released note was not active so we don't need to do anything.
        if (note == lastNoteOn) 
        {
            baseNote = note % 12;
            for (int i = 0; i < mappingNotes[baseNote].size(); i++) {
                processedMidi.addEvent(MidiMessage::noteOff(outputChannel, note + mappingNotes[baseNote][i], velocity), time);
            }
        }

        // If there were still some notes on, play the last one.
        if (note == lastNoteOn && currentNotesOn.size() > 0) {
            lastNoteOn = currentNotesOn.back();
            baseNote = lastNoteOn % 12;
            for (int i = 0; i < mappingNotes[baseNote].size(); i++) {
                processedMidi.addEvent(MidiMessage::noteOn(outputChannel, lastNoteOn + mappingNotes[baseNote][i], velocity), time);
            }
        }
    }
}

void MidiProcessor::updateMapping()
{
    inputChannel = inputChannelParameter->get();
    outputChannel = outputChannelParameter->get();

    for (int i = 0; i < notes.size(); i++)
    {
        addMappedNotes(i, noteParameters[i]->getIndex(), chordParameters[i]->getIndex());
    }
}

void MidiProcessor::addMappedNotes(const int from_note, const int to_note, const int chord)
{
    // Redimensionne le tableau de mapping
    mappingNotes[from_note].resize(chordIntervals[chord].size());
    for (int i = 0; i < mappingNotes[from_note].size(); i++)
    {
        mappingNotes[from_note][i] = to_note - from_note + chordIntervals[chord][i];
    }
}

void MidiProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyChanged, const Identifier& property)
{
    if (property.toString() == "value")
        updateMapping();
}