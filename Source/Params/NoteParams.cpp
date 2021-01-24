#include "Params.h"

/***********************************************
 * NoteParams
 ***********************************************/
NoteParams::NoteParams()
{
    notes.reserve(Notes::count);
    for (int i = 0; i < Notes::count; i++)
        notes.emplace_back(new NoteParam(i));
}

void NoteParams::addParams(AudioProcessor& p)
{
    for (auto& note : notes) {
        note->addParams(p);
    }
}