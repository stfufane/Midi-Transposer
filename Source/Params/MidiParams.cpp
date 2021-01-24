#include "Params.h"

/********************************
 * MidiParams
 ********************************/
MidiParams::~MidiParams()
{
    inputChannel->removeListener(this);
    outputChannel->removeListener(this);
    octaveTranspose->removeListener(this);
}

void MidiParams::addParams(AudioProcessor& p)
{
    p.addParameter(inputChannel = new AudioParameterInt(ParamIDs::inChannel, "Input Channel", 0, 16, 1, "Input Channel"));
    p.addParameter(outputChannel = new AudioParameterInt(ParamIDs::outChannel, "Output Channel", 0, 16, 1, "Output Channel"));
    p.addParameter(octaveTranspose = new AudioParameterInt(ParamIDs::octaveTranspose, "Transpose Octaves", -1, 4, 0, "Transpose Octaves"));

    inputChannel->addListener(this);
    outputChannel->addListener(this);
    octaveTranspose->addListener(this);
}

void MidiParams::parameterValueChanged(int, float)
{
    if (update != nullptr) update();
}
