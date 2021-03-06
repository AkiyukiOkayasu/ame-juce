/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "ame.hpp"
#include "ame_Biquad.hpp"
#include "ame_Reverb.hpp"

#include <JuceHeader.h>
#include <array>

//==============================================================================
/**
*/
class AmejuceAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    AmejuceAudioProcessor();
    ~AmejuceAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    static constexpr int maximumChannels = 2;                                      //BiQuad??????????????????????????????????????????????????????
    static constexpr int maximumBufferSize = 8192;                                 //ame?????????????????????????????????????????????????????????????????????????????????????????????
    static constexpr int BufferCapacity = maximumChannels * maximumBufferSize;     //ame?????????????????????????????????????????????????????????????????????????????????????????????
    ame::AudioBuffer<float, BufferCapacity> interleavedBuffer { maximumChannels }; //ame????????????????????????????????????????????????????????????
    ame::dsp::Freeverb<float, 2, 96000> reverb { 48000 };
    ame::dsp::Biquad<float, 2> lpf { 48000 };
    juce::Reverb jucereberb {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmejuceAudioProcessor)
};
