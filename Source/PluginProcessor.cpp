#include "PluginProcessor.h"

namespace
{
    constexpr int maxChannels = 18;
}

ImWrapAudioProcessor::ImWrapAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withOutput ("Output", juce::AudioChannelSet::discreteChannels (maxChannels), true))
{
}

void ImWrapAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = (float) sampleRate;
    for (auto& p : channelPhases) p = 0.0f;
    gain = 0.0f;
}

bool ImWrapAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& out = layouts.getMainOutputChannelSet();
    return out == juce::AudioChannelSet::discreteChannels (maxChannels)
        || out == juce::AudioChannelSet::stereo();
}

void ImWrapAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();

    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            gate = true;
            currentFreq = 440.0f * std::pow (2.0f, (msg.getNoteNumber() - 69) / 12.0f);
        }
        else if (msg.isNoteOff())
        {
            gate = false;
        }
    }

    const float fadeSpeed = 2.0f / currentSampleRate;
    const int numSamples = buffer.getNumSamples();

    if (gate)
        gain = juce::jmin (1.0f, gain + fadeSpeed * (float) numSamples);
    else
        gain = juce::jmax (0.0f, gain - fadeSpeed * (float) numSamples);

    if (gain <= 0.0f)
        return;

    const int numChans = juce::jmin (buffer.getNumChannels(), maxChannels);

    for (int i = 0; i < numChans; ++i)
    {
        const float chFreq = currentFreq * std::pow (2.0f, (float) i / 12.0f);
        const float inc = juce::MathConstants<float>::twoPi * chFreq / currentSampleRate;

        auto* w = buffer.getWritePointer (i);
        for (int s = 0; s < numSamples; ++s)
        {
            w[s] = 0.25f * gain * std::sin (channelPhases[(size_t) i]);
            channelPhases[(size_t) i] += inc;
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ImWrapAudioProcessor();
}