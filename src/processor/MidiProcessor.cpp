#include "MidiProcessor.h"

MidiProcessor::MidiProcessor()
        : notesMapping(12, std::set<int>({ 0 })) {}

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
    const auto inputChannel = midiParams.inputChannel->get();
    for (const auto& metadata: midiMessages) {
        const auto& m = metadata.getMessage();
        // Only notes on and off from input channel are processed, the rest is passed through.
        if (inputChannel == 0 || m.getChannel() == inputChannel) {
            if (m.isNoteOnOrOff()) {
                mapNote(m, metadata.samplePosition);
            } else {
                processedMidi.addEvent(m, metadata.samplePosition);
            }
        }
    }

    /**
     * If arpeggiator is activated, we need to calculate which note will be sent at which time.
     */
    if (arpeggiatorParams.activated->get()) {
        processArpeggiator(numSamples, playHead);
    }

    midiMessages.swapWith(processedMidi);
}

/*================================================================ */
// CHORD MAPPING
/*================================================================ */
/*
    There are many cases to take in account here.
    We want the input to be monophonic, so we need to know which was the last played note and
    if there are some notes still on to be played when the last one is released.
*/
void MidiProcessor::mapNote(const juce::MidiMessage& m, const int samplePosition)
{
    // The output channel will be the original one or the one defined by the parameter knob.
    const auto outputChannel = midiParams.outputChannel->get();
    const auto channel = (outputChannel == 0) ? m.getChannel() : outputChannel;
    const NoteState noteState {m.getNoteNumber(), channel, m.getVelocity() };
    if (m.isNoteOn()) {
        // Add the note to the vector of current notes played.
        currentInputNotesOn.push_back(noteState);

        // If the note changed, turn off the previous notes before adding the new ones.
        if (noteState.note != lastNoteOn.note && lastNoteOn.note > -1) {
            stopCurrentNotes(noteState.velocity, samplePosition);
        }

        // Play the received note.
        playMappedNotes(noteState, samplePosition);
    } else {
        // For every note off, remove the received note from the vector of current notes held.
        removeHeldNote(noteState.note);

        // Turn off the corresponding notes for the current note off if it's the same as the last played note.
        // Otherwise, it means the released note was not active, so we don't need to do anything (case of multiple notes held)
        if (noteState.note == lastNoteOn.note) {
            stopCurrentNotes(noteState.velocity, samplePosition);

            // If there were still some notes held, play the last one.
            if (!currentInputNotesOn.empty()) {
                playMappedNotes(currentInputNotesOn.back(), samplePosition);
            } else {
                // No note is currently played.
                lastNoteOn.reset();
                if (arp.currentNote.note != -1) {
                    processedMidi.addEvent(juce::MidiMessage::noteOff(arp.currentNote.channel, arp.currentNote.note,
                                                                      noteState.velocity), samplePosition);
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
    arp.currentIndex = 0;
}

void MidiProcessor::playCurrentNotes(const int samplePosition)
{
    // Nothing to do here if arpeggiator is on, it has its own loop to play the notes.
    if (arpeggiatorParams.activated->get()) {
        return;
    }

    for (const auto noteState: currentOutputNotesOn) {
        if (noteState.note >= 0 && noteState.note < 128) {
            processedMidi.addEvent(juce::MidiMessage::noteOn(noteState.channel, noteState.note, noteState.velocity),
                                   samplePosition);
        }
    }
}

void MidiProcessor::stopCurrentNotes(const juce::uint8 velocity, const int samplePosition)
{
    if (arpeggiatorParams.activated->get()) { return; }

    for (const auto& noteState: currentOutputNotesOn) {
        processedMidi.addEvent(juce::MidiMessage::noteOff(noteState.channel, noteState.note, velocity), samplePosition);
    }
}

void MidiProcessor::removeHeldNote(const int note)
{
    currentInputNotesOn.erase(std::remove_if(currentInputNotesOn.begin(), currentInputNotesOn.end(),
                                              [&note](const auto& note_state) {
                                                  return note_state.note == note;
                                              }), currentInputNotesOn.end());
}

std::vector<MidiProcessor::NoteState> MidiProcessor::getMappedNotes(const NoteState& noteState) const
{
    std::vector<NoteState> mappedNotes;
    const int baseNote = noteState.note % 12;
    const auto octaveTranspose = midiParams.octaveTranspose->get();

    // If there's an octave transpose, add the root note at its original height.
    if (octaveTranspose != 0) {
        mappedNotes.push_back(noteState);
    }

    // Then add all the notes from the mapping vector at the transposed height.
    for (const auto noteMapping: notesMapping[static_cast<size_t>(baseNote)]) {
        mappedNotes.push_back({noteState.note + noteMapping + (octaveTranspose * 12), noteState.channel, noteState.velocity});
    }

    return mappedNotes;
}

/*================================================================ */
// ARPEGGIATOR
/*================================================================ */
void MidiProcessor::processArpeggiator(const int numSamples, juce::AudioPlayHead* playHead)
{
    juce::Optional<juce::AudioPlayHead::PositionInfo> positionInfo;
    if (playHead != nullptr) {
        positionInfo = playHead->getPosition();
    }

    // Update the arpeggiated notes if there's been an update in NoteParam listener
    // and the updated mapping is the currently played note.
    if (arp.noteUpdated > -1 && (lastNoteOn.note % 12) == arp.noteUpdated) {
        auto mappedNotes = getMappedNotes(lastNoteOn);
        if (mappedNotes.size() < currentOutputNotesOn.size()) {
            arp.currentIndex = juce::jmin(arp.currentIndex, (int) mappedNotes.size() - 1);
        }
        currentOutputNotesOn.swap(mappedNotes);
        arp.noteUpdated = -1;
    }

    // The synced arpeggiator is used only in a DAW context currently playing.
    // The note duration will still use the current BPM to be calculated anyway.
    if (arpeggiatorParams.synced->get() && positionInfo.hasValue() && positionInfo->getIsPlaying()) {
        arpeggiateSync(numSamples, *positionInfo);
    } else {
        arpeggiate(numSamples, *positionInfo);
    }
}

int MidiProcessor::getArpeggiatorNoteDuration(const juce::AudioPlayHead::PositionInfo& positionInfo)
{
    if (arpeggiatorParams.synced->get() && positionInfo.getBpm().hasValue() && positionInfo.getBpm() != 0.) {
        arp.division = Notes::divisions[static_cast<size_t>(arpeggiatorParams.syncRate->get())].division;
        auto bpm = positionInfo.getBpm().orFallback(60.);
        auto samplesPerBeat = arp.sampleRate / (bpm / 60.);
        return static_cast<int> (std::ceil(samplesPerBeat * arp.division));
    }
    return static_cast<int> (std::ceil(arp.sampleRate * .1 * (.1 + (5. - 5. * arpeggiatorParams.rate->get()))));
}

// This is a simple arpeggiator function that does not care about transport timing because it's not in a playing context.
void MidiProcessor::arpeggiate(const int numSamples, const juce::AudioPlayHead::PositionInfo& positionInfo)
{
    auto noteDuration = getArpeggiatorNoteDuration(positionInfo);
    /*
    There are two possibilities here :
    - The number of samples of the current block is less than the number of samples of the note duration.
      => There can be only one note played during the block, and it will happen when note duration is before the next block compared
         to the elapsed time since last note was played.
    - The number of samples of the current block is greater than the number of samples of the note duration.
      => We loop through the number of samples and count elapsed time until no more note can be played.
    */
    if (numSamples < noteDuration) {
        if ((arp.time + numSamples) >= noteDuration) {
            auto offset = juce::jmax(0, juce::jmin((int) (noteDuration - arp.time), numSamples - 1));
            playArpeggiatorNote(offset);
        }
        arp.time = (arp.time + numSamples) % noteDuration;
    } else {
        while (arp.time < numSamples) {
            auto offset = arp.time + noteDuration;
            if (offset < numSamples) {
                playArpeggiatorNote(offset);
            }
            arp.time += noteDuration;
        }
        arp.time = arp.time % numSamples;
    }
}

// The synced arpeggiator is used when there's a track playing in a DAW, and it requires a few calculations to snap to the grid.
void MidiProcessor::arpeggiateSync(const int numSamples, const juce::AudioPlayHead::PositionInfo& positionInfo)
{
    auto beatPosition = positionInfo.getPpqPosition();
    auto samplesPerBeat = arp.sampleRate / (positionInfo.getBpm().orFallback(60.) / 60.);
    int offset { 0 };

    while (offset < numSamples) {
        // Reset the position calculation if the division has changed.
        auto lastDivision = Notes::divisions[static_cast<size_t>(arpeggiatorParams.syncRate->get())].division;
        if (arp.division != lastDivision) {
            // Update the current division from parameter
            arp.division = lastDivision;
            arp.nextBeatPosition = 0.;
        }

        // We need to get the current quarter note and see what's the next candidate position to snap to the current time division.
        if (arp.nextBeatPosition == 0.) {
            int nb_divisions = 1;
            while (arp.nextBeatPosition == 0.) {
                // For divisions greater than 1.0, we just snap to the next quarter note.
                auto nextDivision = std::floor(*beatPosition) + (nb_divisions * std::min(1., arp.division));
                if (nextDivision >= *beatPosition) {
                    arp.nextBeatPosition = nextDivision;
                }

                nb_divisions++;
            }
        }

        // The next "snapping" time division occurs in this block! We need to calculate the offset here and play the note.
        offset = static_cast<int> ((arp.nextBeatPosition - *beatPosition) * samplesPerBeat);
        if (offset < numSamples) {
            playArpeggiatorNote(offset);
            arp.nextBeatPosition += arp.division;
        }
    }
}

void MidiProcessor::playArpeggiatorNote(const int offset)
{
    if (arp.currentNote.note > -1) {
        processedMidi.addEvent(juce::MidiMessage::noteOff(arp.currentNote.channel, arp.currentNote.note), offset);
        arp.currentNote.reset();
    }

    if (!currentOutputNotesOn.empty()) {
        arp.currentNote = currentOutputNotesOn[static_cast<size_t>(arp.currentIndex)];
        processedMidi.addEvent(
                juce::MidiMessage::noteOn(arp.currentNote.channel, arp.currentNote.note, arp.currentNote.velocity),
                offset);
        arp.currentIndex = (arp.currentIndex + 1) % ((int) currentOutputNotesOn.size());
    }
}

/*================================================================ */
// PARAMETERS
/*================================================================ */
void MidiProcessor::addParameters(juce::AudioProcessor& p)
{
    midiParams.addParams(p);
    arpeggiatorParams.addParams(p);
    noteParams.addParams(p);

    // Each note param has its own listener and lambda function so only the corresponding note
    // is updated in the notesMapping vector. It also avoids testing paramID in parameterChanged method.
    for (auto& noteParam: noteParams.notes) {
        noteParam->update = [this, &noteParam]() { updateNoteMapping(*noteParam); };
    }

    initParameters();
}

void MidiProcessor::initParameters()
{
    for (auto& noteParam: noteParams.notes) {
        updateNoteMapping(*noteParam);
    }
}

// This method is called by the lambda associated to every noteParam when one of the note parameters is updated.
void MidiProcessor::updateNoteMapping(const Params::NoteParam& inNoteParam)
{
    std::set<int> new_mapping;
    // There's a slider on each note defining a new interval to add.
    const auto map_note = inNoteParam.mapNote->get();
    if (map_note) {
        auto transpose = inNoteParam.transpose->get();
        // Add the transposed value first, it's the root note.
        new_mapping.insert(transpose);

        // Then add the selected intervals.
        for (const auto& interval_param : inNoteParam.intervals) {
            new_mapping.insert(transpose + interval_param->interval->get());
        }
    } else {
        new_mapping.insert(0);
    }

    // Finally, replace the old mapping.
    notesMapping[static_cast<size_t>(inNoteParam.noteIndex)].swap(new_mapping);

    // Notify arpeggiator that there's been an update on this note.
    arp.noteUpdated = inNoteParam.noteIndex;
}
