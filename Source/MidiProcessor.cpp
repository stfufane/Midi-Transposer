#include "MidiProcessor.h"

MidiProcessor::MidiProcessor()
    : notesMapping(12, std::vector<int>(1, 0))
{}

/*================================================================ */
// PROCESS
/*================================================================ */
void MidiProcessor::prepareToPlay(const double rate)
{
    arp.sampleRate = static_cast<float> (rate);
    arp.time = 0;
    arp.currentNote = 0;
    arp.lastNote.reset();
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
        if (arpeggiatorParams.synced->get())
            arpeggiateSync(numSamples, playHead);
        else
            arpeggiate(numSamples);
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
void MidiProcessor::arpeggiate(const int numSamples)
{
    auto noteDuration = arpeggiatorNoteDuration();
    if ((arp.time + numSamples) >= noteDuration)
    {
        auto offset = jmax (0, jmin ((int) (noteDuration - arp.time), numSamples - 1));

        if (arp.lastNote.note > -1)
        {
            processedMidi.addEvent (juce::MidiMessage::noteOff (arp.lastNote.channel, arp.lastNote.note), offset);
            arp.lastNote.reset();
        }

        if (currentOutputNotesOn.size() > 0)
        {
            arp.currentNote = (arp.currentNote + 1) % currentOutputNotesOn.size();
            arp.lastNote = currentOutputNotesOn[arp.currentNote];
            processedMidi.addEvent(juce::MidiMessage::noteOn(arp.lastNote.channel, arp.lastNote.note, arp.lastNote.velocity), offset);
        }
    }

    arp.time = (arp.time + numSamples) % noteDuration;
}

void MidiProcessor::arpeggiateSync(const int numSamples, juce::AudioPlayHead* playHead)
{
    ignoreUnused(numSamples);
    // TODO https://docs.juce.com/master/structAudioPlayHead_1_1CurrentPositionInfo.html
    // https://forum.juce.com/t/audioplayhead-currentpositioninfo-determining-position-in-bar/19422
    auto division = Notes::durations[arpeggiatorParams.syncRate->get()].division;
    int noteDuration;
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    if (playHead != nullptr && playHead->getCurrentPosition(positionInfo))
    {
        auto bpm = positionInfo.bpm;
        auto samplesPerBeat = arp.sampleRate / (bpm / 60.0f);
        noteDuration = static_cast<int> (std::ceil(samplesPerBeat * division));
    }
    else
    {
        noteDuration = arpeggiatorNoteDuration();
    }
    // Calculer le nombre de PPQ depuis le dernier processBlock. Et comparer ce diff avec la durée de la note en PPQ.
    // Utiliser les paramètres de Arpeggiator::rates pour récupérer la division à utiliser.
    // Déclencher les notes on/off sur les multiples de PPQ correspondants.
    // Utiliser le nombre de samples pour calculer l'offset en fonction des PPQ.
}

int MidiProcessor::arpeggiatorNoteDuration() const
{
    return static_cast<int> (std::ceil (arp.sampleRate * 0.25f * (0.1f + (1.0f - (arpeggiatorParams.rate->get())))));
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
