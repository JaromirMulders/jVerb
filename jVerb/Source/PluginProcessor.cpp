/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JVerbAudioProcessor::JVerbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

AudioProcessorValueTreeState::ParameterLayout JVerbAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;
    
    auto inputParam = std::make_unique<AudioParameterFloat>(INPUT_ID, INPUT_NAME, 0.0f, 1.0f, 0.5f);
    auto outputParam = std::make_unique<AudioParameterFloat>(OUTPUT_ID, OUTPUT_NAME, 0.0f, 1.0f, 0.5f);
    auto colorParam = std::make_unique<AudioParameterFloat>(COLOR_ID, COLOR_NAME, 0.0f, 1.0f, 0.5f);
    auto diffusionParam = std::make_unique<AudioParameterFloat>(DIFFUSION_ID, DIFFUSION_NAME, 0.0f, 1.0f, 0.5f);
    auto dampingParam = std::make_unique<AudioParameterFloat>(DAMPING_ID, DAMPING_NAME, 0.0f, 1.0f, 0.5f);
    auto predelayParam = std::make_unique<AudioParameterFloat>(PREDELAY_ID, PREDELAY_NAME, 0.0f, 1.0f, 0.5f);
    auto decayParam = std::make_unique<AudioParameterFloat>(DECAY_ID, DECAY_NAME, 0.0f, 1.0f, 0.5f);
    auto sizeParam = std::make_unique<AudioParameterFloat>(SIZE_ID, SIZE_NAME, 0.0f, 1.0f, 0.5f);
    auto drywetParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 1.0f, 0.5f);
    auto lfofreqParam = std::make_unique<AudioParameterFloat>(LFOFREQ_ID, LFOFREQ_NAME, 0.0f, 1.0f, 0.5f);
    auto lfodepthParam = std::make_unique<AudioParameterFloat>(LFODEPTH_ID, LFODEPTH_NAME, 0.0f, 1.0f, 0.5f);
  
    params.push_back(std::move(inputParam));
    params.push_back(std::move(outputParam));
    params.push_back(std::move(colorParam));
    params.push_back(std::move(diffusionParam));
    params.push_back(std::move(dampingParam));
    params.push_back(std::move(predelayParam));
    params.push_back(std::move(decayParam));
    params.push_back(std::move(sizeParam));
    params.push_back(std::move(drywetParam));
    params.push_back(std::move(lfofreqParam));
    params.push_back(std::move(lfodepthParam));
  
    return { params.begin(), params.end() };
}

JVerbAudioProcessor::~JVerbAudioProcessor()
{
}

//==============================================================================
const String JVerbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JVerbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JVerbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JVerbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JVerbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JVerbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JVerbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JVerbAudioProcessor::setCurrentProgram (int index)
{
}

const String JVerbAudioProcessor::getProgramName (int index)
{
    return {};
}

void JVerbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JVerbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JVerbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JVerbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void JVerbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
      ScopedNoDenormals noDenormals;
      auto totalNumInputChannels  = getTotalNumInputChannels();
      auto totalNumOutputChannels = getTotalNumOutputChannels();

      // In case we have more outputs than inputs, this code clears any output
      // channels that didn't contain input data, (because these aren't
      // guaranteed to be empty - they may contain garbage).
      // This is here to avoid people getting screaming feedback
      // when they first compile a plugin, but obviously you don't need to keep
      // this code if your algorithm always overwrites all the output channels.
    
    
      for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
          buffer.clear (i, 0, buffer.getNumSamples());
      // This is the place where you'd normally do the guts of your plugin's
      // audio processing...
      // Make sure to reset the state if your inner loop is processing
      // the samples and the outer loop is handling the channels.
      // Alternatively, you can process the samples with the channels
      // interleaved by keeping the same state.
  
      auto sliderGainValue = treeState.getRawParameterValue(INPUT_ID);
  
      for (int channel = 0; channel < totalNumInputChannels; ++channel)
      {
        auto* channelData = buffer.getWritePointer (channel);

          // ..do something to the data...
        for(int sample = 0; sample < buffer.getNumSamples(); sample++){
          channelData[sample] = buffer.getSample(channel, sample) * *sliderGainValue;
        }
      }
}

//==============================================================================
bool JVerbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JVerbAudioProcessor::createEditor()
{
    return new JVerbAudioProcessorEditor (*this);
}

//==============================================================================
void JVerbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void JVerbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

  
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
  std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
  if (xmlState.get() != nullptr){
    if (xmlState->hasTagName (treeState.state.getType())){
            treeState.replaceState (ValueTree::fromXml (*xmlState));
    }
  }
  
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JVerbAudioProcessor();
}
