/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../ame/ame.hpp"

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
    static constexpr int maximumChannels = 2;      //BiQuadクラスが処理できる最大のチャンネル数
    static constexpr int maximumBufferSize = 8192; //ame用オーディオバッファーが確保するサンプル数（チャンネルあたり）
    ame::IIR::BiQuad::BiQuad<maximumChannels> lpf;
    static constexpr int BufferCapacity = maximumChannels * maximumBufferSize;                        //ame用オーディオバッファーが確保するサンプル数（全チャンネル合計）
    ame::AudioBuffer<float, BufferCapacity> interleavedBuffer { maximumChannels, maximumBufferSize }; //ame用オーディオバッファー（インターリーブ）

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmejuceAudioProcessor)
};
