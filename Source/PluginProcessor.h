#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ImWrapAudioProcessor : public juce::AudioProcessor
{
public:
    ImWrapAudioProcessor();

    // --- audio ---
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // --- geen editor (v0.1) ---
    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

    // --- administratie ---
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override  { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

private:
    float currentFreq = 110.0f;
    bool  gate = false;               // noot aan/uit
    float currentSampleRate = 48000.0f;
    float gain = 0.0f;                // zachte fade tegen clicks
    std::array<float, 18> channelPhases {};  // eigen oscillatorfase per kanaal

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImWrapAudioProcessor)
};