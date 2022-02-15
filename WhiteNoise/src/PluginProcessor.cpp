/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "ame_AudioBuffer.hpp"

//==============================================================================
AmejuceAudioProcessor::AmejuceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
        #endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    #endif
    )
#endif
{
}

AmejuceAudioProcessor::~AmejuceAudioProcessor()
{
}

//==============================================================================
const juce::String AmejuceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmejuceAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AmejuceAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AmejuceAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AmejuceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmejuceAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AmejuceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmejuceAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AmejuceAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmejuceAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AmejuceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate (sampleRate);
    reverb.setDryWet (0.5f);
    jucereberb.setSampleRate (sampleRate);
    lpf.setSampleRate (sampleRate);
    lpf.makeLowPass (440.0f, 0.7f);
}

void AmejuceAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmejuceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
    #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

            // This checks if the input layout matches the output layout
        #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
        #endif

    return true;
    #endif
}
#endif

void AmejuceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numChannels = buffer.getNumChannels();
    const auto bufferSize = buffer.getNumSamples();

    jassert (std::max ({ totalNumInputChannels, totalNumOutputChannels, numChannels }) <= maximumChannels);
    jassert (bufferSize <= maximumBufferSize);

#if 0
    for (auto ch = 0; ch < numChannels; ch++)
    {
        auto b = buffer.getWritePointer (ch);
        for (auto samp = 0; samp < bufferSize; samp++)
        {
            b[samp] = ame::noise();
        }
    }
#endif
    // jucereberb.processStereo (buffer.getWritePointer (0), buffer.getWritePointer (1), bufferSize);

#if 1
    //======== JUCEのチャンネル分割バッファーをameで扱えるようにインターリーブに並び替えてコピー ========
    juce::AudioDataConverters::interleaveSamples (buffer.getArrayOfReadPointers(), interleavedBuffer.buffer.data(), bufferSize, numChannels);

    //======== ameによるエフェクト処理 ========

    auto block = interleavedBuffer.makeAudioBlockView();
    ame::AudioBlockView subView { block.view.subspan (0, numChannels * bufferSize), static_cast<uint_fast32_t> (numChannels) };
    reverb.process (subView);
    // lpf.process (block);
    //delay.process(block);

    //======== ameのインターリーブバッファーをチャンネル分割に並び替えてJUCEに戻す ========
    juce::AudioDataConverters::deinterleaveSamples (interleavedBuffer.buffer.data(), buffer.getArrayOfWritePointers(), bufferSize, numChannels);
#endif
}

//==============================================================================
bool AmejuceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AmejuceAudioProcessor::createEditor()
{
    return new AmejuceAudioProcessorEditor (*this);
}

//==============================================================================
void AmejuceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AmejuceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmejuceAudioProcessor();
}
