/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "delay.h"
#include "allPass.h"

#define INPUT_ID "input"
#define INPUT_NAME "Input"
#define OUTPUT_ID "output"
#define OUTPUT_NAME "Output"
#define COLOR_ID "color"
#define COLOR_NAME "Color"
#define COLORGAIN_ID "colorgain"
#define COLORGAIN_NAME "Color Gain"
#define DIFFUSION_ID "diffusion"
#define DIFFUSION_NAME "Diffusion"
#define DAMPING_ID "damping"
#define DAMPING_NAME "Damping"
#define PREDELAY_ID "predelay"
#define PREDELAY_NAME "Pre-Delay"
#define DECAY_ID "decay"
#define DECAY_NAME "Decay"
#define SIZE_ID "size"
#define SIZE_NAME "Size"
#define DRYWET_ID "Dry/Wet"
#define DRYWET_NAME "Dry/Wet"
#define LFOFREQ_ID "lfofreq"
#define LFOFREQ_NAME "Freq"
#define LFODEPTH_ID "lfodepth"
#define LFODEPTH_NAME "Depth"
#define LFOWAVEFORM_ID "lfowaveform"
#define LFOWAVEFORM_NAME "Waveform"
#define ZERO_ID "zero"
#define ZERO_NAME " "

#define cFdnChanAmnt 4

//==============================================================================
/**
*/
class JVerbAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JVerbAudioProcessor();
    ~JVerbAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void onDAWChange(int samplerate, int numSamples);
    float mix(float a, float b, float *m);
  
    //==============================================================================

    float rawGainValue;
    AudioProcessorValueTreeState treeState;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    //==============================================================================
    Delay   preDelay1;
    Delay   preDelay2;
    Delay   *delays;
    allPass *allPassFilters;
  
  
    //==============================================================================
    
    float **passBuffers;
    float *writeBufferL;
    float *writeBufferR;
  
    //==============================================================================
    int samplerate;
    int numSamples;
    int oldNumSamples;
    int oldSamplerate;
  
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JVerbAudioProcessor)
 
};
