#include "Params.h"

namespace Params
{
/***********************************************
 * NoteParams
 ***********************************************/
NoteParams::NoteParams()
{
    notes.reserve(Notes::count);
    for (int i = 0; i < Notes::count; i++) {
        notes.emplace_back(new NoteParam(i));
    }
}

void NoteParams::addParams(juce::AudioProcessor& p)
{
    for (auto& note: notes) {
        note->addParams(p);
    }
}

}