#include "MidiProcessor.h"

MidiProcessor::MidiProcessor()
    : notesMapping(12, std::vector<int>(1, 0))
{}

/*================================================================ */
// PROCESS
/*================================================================ */
void MidiProcessor::prepareToPlay(const double rate)
{
    arp.reset();
    arp.sampleRate = static_cast<float> (rate);
}

void MidiProcessor::process(juce::MidiBuffer& midiMessages, const int numSamples, juce::AudioPlayHead* playHead)
{
    processedMidi.clear();

    /**
     * This will filter notes on/off and keep other messages.
     * In arpeggiator mode it will just calculate the chord notes to play
     */
    for (const auto metadata: midiMessages)
    {
        const auto m = metadata.getMessage();
        // Only notes on and off from input channel are processed, the rest is passed through.
        const auto inputChannel = midiParams.inputChannel->get();
        if (inputChannel == 0 || m.getChannel() == inputChannel)
        {
            if (m.isNoteOnOrOff())
                mapNote(m, metadata.samplePosition);
            else
                processedMidi.addEvent(m, metadata.samplePosition);
        }
    }

    /**
     * If arpeggiator is activated, we need to calculate which note will be sent at which time.
     */
    if (arpeggiatorParams.activated->get())
    {
        processArpeggiator(numSamples, playHead);
    }

    midiMessages.swapWith(processedMidi);
}

/*================================================================ */
// CHORD MAPPING
/*================================================================ */
/*
    There are many cases to take in account here.
    We want the input to be monophonic so we need to know which was the last played note and 
    if there are some notes still on to be played when the last one is released.
*/
void MidiProcessor::mapNote(const juce::MidiMessage& m, const int samplePosition)
{
    // The output channel will be the original one or the one defined by the parameter knob.
    const auto outputChannel = midiParams.outputChannel->get();
    const auto channel = (outputChannel == 0) ? m.getChannel() : outputChannel;
    const NoteState noteState { m.getNoteNumber(), channel, m.getVelocity() };
    if (m.isNoteOn())
    {
        // Add the note to the vector of current notes played.
        currentInputNotesOn.push_back(noteState);

        // If the note changed, turn off the previous notes before adding the new ones.
        if (noteState.note != lastNoteOn.note && lastNoteOn.note > -1)
            stopCurrentNotes(noteState.velocity, samplePosition);
        
        // Play the received note.
        playMappedNotes(noteState, samplePosition);
    }
    else 
    {
        // For every note off, remove the received note from the vector of current notes held.
        removeHeldNote(noteState.note);
        
        // Turn off the corresponding notes for the current note off if it's the same as the last played note.
        // Otherwise it means the released note was not active so we don't need to do anything (case of multiple notes held)
        if (noteState.note == lastNoteOn.note) 
        {
            stopCurrentNotes(noteState.velocity, samplePosition);

            // If there were still some notes held, play the last one.
            if (currentInputNotesOn.size() > 0)
                playMappedNotes(currentInputNotesOn.back(), samplePosition);
            else 
            {
                // No note is currently played.
                lastNoteOn.reset();
                if (arp.currentNote.note != -1) 
                {
                    processedMidi.addEvent(juce::MidiMessage::noteOff(arp.currentNote.channel, arp.currentNote.note, noteState.velocity), samplePosition);
                }
                arp.reset();
                currentOutputNotesOn.clear();
            }
        }
    }
}

void MidiProcessor::playMappedNotes(const NoteState& noteState, const int samplePosition)
{
    // First recalculate the output notes vector.
    auto mappedNotes = getMappedNotes(noteState);
    currentOutputNotesOn.swap(mappedNotes);
    playCurrentNotes(samplePosition);
    lastNoteOn = noteState;
}

void MidiProcessor::playCurrentNotes(const int samplePosition)
{
    // Nothing to do here if arpeggiator is on, it has its own loop to play the notes.
    if (arpeggiatorParams.activated->get()) return;

    for (const auto noteState: currentOutputNotesOn)
        if (noteState.note >= 0 && noteState.note < 128) 
            processedMidi.addEvent(juce::MidiMessage::noteOn(noteState.channel, noteState.note, noteState.velocity), samplePosition);
}

void MidiProcessor::stopCurrentNotes(const uint8 velocity, const int samplePosition)
{
    if (arpeggiatorParams.activated->get()) return;

    for (const auto& noteState: currentOutputNotesOn) 
        processedMidi.addEvent(juce::MidiMessage::noteOff(noteState.channel, noteState.note, velocity), samplePosition);
}

void MidiProcessor::removeHeldNote(const int note)
{
    for (auto it = currentInputNotesOn.begin(); it != currentInputNotesOn.end(); ++it)
    {
        if (it->note == note)
        {
            currentInputNotesOn.erase(it);
            break;
        }
    }
}

std::vector<MidiProcessor::NoteState> MidiProcessor::getMappedNotes(const NoteState& noteState) const
{
    std::vector<NoteState> mappedNotes;
    const int baseNote = noteState.note % 12;
    const auto octaveTranspose = midiParams.octaveTranspose->get();

    // If there's an octave transpose, add the root note at its original height.
    if (octaveTranspose != 0)
        mappedNotes.push_back(noteState);

    // Then add all the notes from the mapping vector at the transposed height.
    for (const auto noteMapping: notesMapping[baseNote])
        mappedNotes.push_back({noteState.note + noteMapping + (octaveTranspose * 12), noteState.channel, noteState.velocity});

    return mappedNotes;
}

/*================================================================ */
// ARPEGGIATOR
/*================================================================ */
void MidiProcessor::processArpeggiator(const int numSamples, juce::AudioPlayHead* playHead)
{
    bool hasPositionInfo{ false };
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    if (playHead != nullptr)
        hasPositionInfo = playHead->getCurrentPosition(positionInfo);

    if (arpeggiatorParams.synced->get() && hasPositionInfo && positionInfo.isPlaying)
        arpeggiateSync(numSamples, positionInfo);
    else
        arpeggiate(numSamples, positionInfo);
}

int MidiProcessor::getArpeggiatorNoteDuration(const juce::AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    if (arpeggiatorParams.synced->get() && positionInfo.bpm != 0.)
    {
        arp.division = Notes::divisions[arpeggiatorParams.syncRate->get()].division;
        auto bpm = positionInfo.bpm;
        auto samplesPerBeat = arp.sampleRate / (bpm / 60.);
        return static_cast<int> (std::ceil(samplesPerBeat * arp.division));
    }
    return static_cast<int> (std::ceil (arp.sampleRate * .25 * (.1 + (1. - (arpeggiatorParams.rate->get())))));
}

// This is a simple arpeggiator function that does not care about transport timing because it's not in a playing context.
void MidiProcessor::arpeggiate(const int numSamples, const juce::AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    auto noteDuration = getArpeggiatorNoteDuration(positionInfo);
    if ((arp.time + numSamples) >= noteDuration)
    {
        auto offset = jmax (0, jmin ((int) (noteDuration - arp.time), numSamples - 1));
        playArpeggiatorNote(offset);
    }

    arp.time = (arp.time + numSamples) % noteDuration;
}

// The synced arpeggiator is used when there's a track playing in a DAW, and it requires a few calculations to snap to the grid.
void MidiProcessor::arpeggiateSync(const int numSamples, const juce::AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    auto beatPosition = positionInfo.ppqPosition;
    auto samplesPerBeat = arp.sampleRate / (positionInfo.bpm / 60.);
    auto beatsPerBlock = numSamples / samplesPerBeat;

    // Reset the position calculation if the division has changed.
    if (arp.division != Notes::divisions[arpeggiatorParams.syncRate->get()].division)
        arp.lastBeatPosition = 0.;

    // Update the current division from parameter
    arp.division = Notes::divisions[arpeggiatorParams.syncRate->get()].division;

    // We need to get the current quarter note and see what's the next candidate position to snap to the current time division.
    double nextBeatPosition { 0. };
    if (arp.lastBeatPosition == 0.)
    {
        int nb_divisions = 1;
        while (nextBeatPosition == 0.)
        {
            auto nextDivision = std::floor(beatPosition) + (nb_divisions * arp.division);
            if (nextDivision >= beatPosition)
                nextBeatPosition = nextDivision;

            nb_divisions ++;
        }
    }
    else
        nextBeatPosition = arp.lastBeatPosition + arp.division;

    // The next "snapping" time division occurs in this block! We need to calculate the offset here and play the note.
    if (nextBeatPosition <= (beatPosition + beatsPerBlock))
    {
        auto offset = static_cast<int> ( ((nextBeatPosition - beatPosition) / beatsPerBlock) * numSamples );
        playArpeggiatorNote(offset);
        
        arp.lastBeatPosition = nextBeatPosition;
    }
}

void MidiProcessor::playArpeggiatorNote(const int offset)
{
    if (arp.currentNote.note > -1)
    {
        processedMidi.addEvent (juce::MidiMessage::noteOff (arp.currentNote.channel, arp.currentNote.note), offset);
        arp.currentNote.reset();
    }

    if (currentOutputNotesOn.size() > 0)
    {
        arp.currentNote = currentOutputNotesOn[arp.currentIndex];
        processedMidi.addEvent(juce::MidiMessage::noteOn(arp.currentNote.channel, arp.currentNote.note, arp.currentNote.velocity), offset);
        arp.currentIndex = (arp.currentIndex + 1) % currentOutputNotesOn.size();
    }
}

/*================================================================ */
// PARAMETERS
/*================================================================ */
MidiParams& MidiProcessor::getMidiParams()
{
    return midiParams;
}

NoteParams& MidiProcessor::getNoteParams()
{
    return noteParams;
}

ArpeggiatorParams& MidiProcessor::getArpeggiatorParams()
{
    return arpeggiatorParams;
}

void MidiProcessor::addParameters(juce::AudioProcessor& p)
{
    midiParams.addParams(p);
    arpeggiatorParams.addParams(p);
    for (auto& noteParam : noteParams.notes) {
        noteParam->addParams(p);
    }

    // Each note param has its own listener and lambda function so only the corresponding note
    // is updated in the notesMapping vector. It also avoids testing paramID in parameterChanged method.
    for (auto& noteParam : noteParams.notes) {
        noteParam->update = [this, &noteParam]() { updateNoteMapping(*noteParam); };
    }

    initParameters();
}

void MidiProcessor::initParameters()
{    
    for (auto& noteParam: noteParams.notes)
    {
        updateNoteMapping(*noteParam);
    }
}

// This method is called by the lambda associated to every noteParam when one of the note parameters is updated.
void MidiProcessor::updateNoteMapping(const NoteParam& noteParam)
{
    std::vector<int> new_mapping;
    // There's a toggle button on each note saying if it should be mapped or not.
    // If not we just use the default value, which is the base note without transposition.
    auto mapNote = noteParam.mapNote->get();
    if (mapNote)
    {
        auto transpose = noteParam.transpose->get();
        // Add the transposed value first, it's the root note.
        new_mapping.push_back(transpose);

        // Then add the selected intervals.
        for (size_t i = 0; i < noteParam.intervals.size(); i++)
            if (noteParam.intervals[i]->interval->get())
                new_mapping.push_back(transpose + (i + 1));
    }
    else
    {
        new_mapping.push_back(0);
    }
    
    // Finally, replace the old mapping.
    notesMapping[noteParam.noteIndex].swap(new_mapping);

    // If arpeggiator is on, we can update the chord directly.
    if (arpeggiatorParams.activated->get() && lastNoteOn.note != -1)
    {
        auto mappedNotes = getMappedNotes(lastNoteOn);
        currentOutputNotesOn.swap(mappedNotes);
    }
}
