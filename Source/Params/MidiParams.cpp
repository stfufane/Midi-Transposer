#include "Params.h"

/********************************
 * MidiParams
 ********************************/

void MidiParams::addParams(juce::AudioProcessor& p)
{
    p.addParameter(inputChannel    = new juce::AudioParameterInt(ParamIDs::inChannel,
                                                                 "Input Channel",
                                                                 0, 16, 1,
                                                                 AudioParameterIntAttributes().withLabel("Input Channel")));
    p.addParameter(outputChannel   = new juce::AudioParameterInt(ParamIDs::outChannel,
                                                                 "Output Channel",
                                                                 0, 16, 1,
                                                                 AudioParameterIntAttributes().withLabel("Output Channel")));
    p.addParameter(octaveTranspose = new juce::AudioParameterInt(ParamIDs::octaveTranspose,
                                                                 "Transpose Octaves",
                                                                 -1, 4, 0,
                                                                 AudioParameterIntAttributes().withLabel("Transpose Octaves")));
}

