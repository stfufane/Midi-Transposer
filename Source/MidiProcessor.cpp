#include "MidiProcessor.h"

MidiProcessor::MidiProcessor()
    : mappingNotes(12, std::vector<int>(1, 0))
{}

void MidiProcessor::addParameters(AudioProcessor& p)
{
    midiParams.addParams(p);
    for (auto& noteParam : noteParams.notes) {
        noteParam->addParams(p);
    }

    // Each note param has its own listener and lambda function so only the corresponding note
    // is updated in the mappingNotes vector. It also avoids testing paramID in parameterChanged method.
    for (auto& noteParam : noteParams.notes) {
        noteParam->update = [this, &noteParam]() { updateNoteMapping(*noteParam); };
    }

    initParameters();
}

void MidiProcessor::process(MidiBuffer& midiMessages)
{
    processedMidi.clear();
    for (const auto metadata: midiMessages)
    {
        const auto m = metadata.getMessage();
        // Only notes on and off from input channel are processed, the rest is passed through.
        if (m.getChannel() == inputChannel)
        {
            if (m.isNoteOnOrOff())
            {
                mapNote(m.getNoteNumber(), m.getVelocity(), m.isNoteOn(), metadata.samplePosition);
            }
            else
            {
                processedMidi.addEvent(m, metadata.samplePosition);
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
void MidiProcessor::mapNote(const int note, const uint8 velocity, const bool noteOn, const int samplePosition)
{
    if (noteOn)
    {
        // Add the note to the vector of current notes played.
        currentInputNotesOn.push_back(note);

        // If the note changed, turn off the previous notes before adding the new ones.
        if (note != lastNoteOn && lastNoteOn > -1)
        {
            stopCurrentNotes(velocity, samplePosition);
        }
        // Play the received note.
        playMappedNotes(note, velocity, samplePosition);
    }
    else 
    {
        // For every note off, remove the received note from the vector of current notes held.
        removeHeldNote(note);
        
        // Turn off the corresponding notes for the current note off if it's the same as the last played note.
        // Otherwise it means the released note was not active so we don't need to do anything (case of multiple notes held)
        if (note == lastNoteOn) 
        {
            stopCurrentNotes(velocity, samplePosition);

            // If there were still some notes held, play the last one.
            if (currentInputNotesOn.size() > 0) {
                // Then play the last note from the vector of active input notes.
                playMappedNotes(currentInputNotesOn.back(), velocity, samplePosition);
            }
            else {
                // No note is currently played.
                lastNoteOn = -1;
            }
        }
    }
}

void MidiProcessor::playMappedNotes(const int note, const uint8 velocity, const int samplePosition)
{
    lastNoteOn = note;
    // First clear the output notes vector to replace its values.
    currentOutputNotesOn.clear();

    const int baseNote = lastNoteOn % 12;
    // If there's an octave transpose, add the root note at its original height.
    if (octaveTranspose != 0) {
        const int noteToPlay = lastNoteOn + mappingNotes[baseNote][0];
        playNote(noteToPlay, velocity, samplePosition);
    }
    // Then add all the notes from the mapping vector at the transposed height.
    for (const auto mappedNote: mappingNotes[baseNote]) {
        const int noteToPlay = lastNoteOn + mappedNote + (octaveTranspose * 12);
        playNote(noteToPlay, velocity, samplePosition);
    }
    currentNoteOutputChannel = outputChannel;
}

void MidiProcessor::playNote(const int note, const uint8 velocity, const int samplePosition)
{
    if (note >= 0 && note < 128) {
        processedMidi.addEvent(MidiMessage::noteOn(outputChannel, note, velocity), samplePosition);
        currentOutputNotesOn.push_back(note);
    }
}

void MidiProcessor::stopCurrentNotes(const uint8 velocity, const int samplePosition)
{
    for (const auto& currentNote: currentOutputNotesOn) {
        processedMidi.addEvent(MidiMessage::noteOff(currentNoteOutputChannel, currentNote, velocity), samplePosition);
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

void MidiProcessor::initParameters()
{
    updateMidiParams();
    
    for (auto& noteParam: noteParams.notes)
    {
        updateNoteMapping(*noteParam);
    }
}

void MidiProcessor::updateMidiParams()
{
    inputChannel = midiParams.inputChannel->get();
    outputChannel = midiParams.outputChannel->get();
    octaveTranspose = midiParams.octaveTranspose->get();
}

// This method is called by the lambda associated to every noteParam when one of the note parameters is updated.
void MidiProcessor::updateNoteMapping(const NoteParam& noteParam)
{
    std::vector<int> new_mapping;
    auto transpose = noteParam.transpose->get();
    // Add the transposed value first
    new_mapping.push_back(transpose);

    // Then add the selected intervals.
    for (size_t i = 0; i < noteParam.intervals.size(); i++)
        if (noteParam.intervals[i]->interval->get())
            new_mapping.push_back(transpose + (i + 1));
    
    // Finally, replace the old mapping.
    mappingNotes[noteParam.noteIndex].swap(new_mapping);
}
