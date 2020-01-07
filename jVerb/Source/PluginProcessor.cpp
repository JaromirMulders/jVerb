#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "delay.h"

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
  samplerate = 44100;
  numSamples = 512;
  oldNumSamples = 0;
  oldSamplerate = 0;
  writeBufferL = new float[numSamples];
  writeBufferR = new float[numSamples];
  passBuffers = new float*[cFdnChanAmnt*2];
  
  allPassFilters = new allPass[cFdnChanAmnt];
  delays = new Delay[cFdnChanAmnt];
  ladderFilters = new LadderFilter[cFdnChanAmnt];
  
  for(int i = 0; i < cFdnChanAmnt*2; i++){
    passBuffers[i] = new float[numSamples];
    for(int j = 0; j < numSamples; j++){
      passBuffers[i][j] = 0.;
      writeBufferL[j] = 0.;
      writeBufferR[j] = 0.;
    }//for
  }//for

  
}

AudioProcessorValueTreeState::ParameterLayout JVerbAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;
    
    auto inputParam = std::make_unique<AudioParameterFloat>(INPUT_ID, INPUT_NAME, 0.0f, 1.0f, 0.5f);
    auto outputParam = std::make_unique<AudioParameterFloat>(OUTPUT_ID, OUTPUT_NAME, 0.0f, 1.0f, 0.5f);
    auto colorParam = std::make_unique<AudioParameterFloat>(COLOR_ID, COLOR_NAME, 0.0f, 1.0f, 0.5f);
    auto colorGainParam = std::make_unique<AudioParameterFloat>(COLORGAIN_ID, COLORGAIN_NAME, 0.0f, 1.0f, 0.1f);
    auto diffusionParam = std::make_unique<AudioParameterFloat>(DIFFUSION_ID, DIFFUSION_NAME, 0.0f, 1.0f, 0.5f);
    auto dampingParam = std::make_unique<AudioParameterFloat>(DAMPING_ID, DAMPING_NAME, 10.0f, 20000.0f, 0.5f);
    auto predelayParam = std::make_unique<AudioParameterFloat>(PREDELAY_ID, PREDELAY_NAME, 0.0f, 250.0f, 50.0f);
    auto decayParam = std::make_unique<AudioParameterFloat>(DECAY_ID, DECAY_NAME, 0.0f, 1.0f, 0.5f);
    auto sizeParam = std::make_unique<AudioParameterFloat>(SIZE_ID, SIZE_NAME, 0.0f, 1.0f, 0.5f);
    auto drywetParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 100.0f, 70.0f);
    auto lfofreqParam = std::make_unique<AudioParameterFloat>(LFOFREQ_ID, LFOFREQ_NAME, 0.0f, 1.0f, 0.5f);
    auto lfodepthParam = std::make_unique<AudioParameterFloat>(LFODEPTH_ID, LFODEPTH_NAME, 0.0f, 1.0f, 0.5f);
    auto zeroParam = std::make_unique<AudioParameterFloat>(ZERO_ID, ZERO_NAME, 0.0f, 0.0f, 0.0f);
    
    params.push_back(std::move(inputParam));
    params.push_back(std::move(outputParam));
    params.push_back(std::move(colorParam));
    params.push_back(std::move(colorGainParam));
    params.push_back(std::move(diffusionParam));
    params.push_back(std::move(dampingParam));
    params.push_back(std::move(predelayParam));
    params.push_back(std::move(decayParam));
    params.push_back(std::move(sizeParam));
    params.push_back(std::move(drywetParam));
    params.push_back(std::move(lfofreqParam));
    params.push_back(std::move(lfodepthParam));
    params.push_back(std::move(zeroParam));
  
    return { params.begin(), params.end() };
}

JVerbAudioProcessor::~JVerbAudioProcessor()
{
  delete []writeBufferL;
  writeBufferL = NULL;
  delete []writeBufferR;
  writeBufferR = NULL;
  for(int i = 0; i < cFdnChanAmnt*2; i++){
    delete [] passBuffers[i];
  }//for
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

      numSamples = buffer.getNumSamples();
      samplerate = getSampleRate();
      onDAWChange(samplerate,numSamples);

      //auto sliderGainValue = treeState.getRawParameterValue(INPUT_ID);
      auto preDelayValue = treeState.getRawParameterValue(PREDELAY_ID);
      float nPreDelayVal = *preDelayValue*0.001*samplerate;
      preDelayValue = &nPreDelayVal;
  
      auto colorValue = treeState.getRawParameterValue(COLOR_ID);
      float nColorVal = *colorValue*0.15*samplerate;
      colorValue = &nColorVal;
  
      auto colorGainValue = treeState.getRawParameterValue(COLORGAIN_ID);
      float nColorGainVal = *colorGainValue;
      colorGainValue = &nColorGainVal;
  
      auto sizeValue = treeState.getRawParameterValue(SIZE_ID);
      float nSizeValue = *sizeValue*samplerate;
      sizeValue = &nSizeValue;
  
      auto dryWetValue = treeState.getRawParameterValue(DRYWET_ID);
      float nDryWetVal = *dryWetValue*0.01;
      dryWetValue = &nDryWetVal;
 
      auto dampingValue = treeState.getRawParameterValue(DAMPING_ID);
      float nDampingValue = tan(cPi * (*dampingValue * 1./samplerate));
      dampingValue = &nDampingValue;

      auto decayValue = treeState.getRawParameterValue(DECAY_ID);
      float nDecayValue = *decayValue;
      decayValue = &nDecayValue;
  
      //clear buffers
      for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, numSamples);
      }//for
      
      auto* channelDataL = buffer.getWritePointer(0);
      preDelay1.process_samples(channelDataL,passBuffers[0],preDelayValue);
      allPassFilters[0].process_samples(passBuffers[0], passBuffers[1], colorValue, colorGainValue);
      allPassFilters[1].process_samples(passBuffers[0], passBuffers[2], colorValue, colorGainValue);
      ladderFilters[0].process_samples(passBuffers[1],passBuffers[0],dampingValue);
      ladderFilters[1].process_samples(passBuffers[2],passBuffers[3],dampingValue);
      delays[0].process_samples(passBuffers[0], passBuffers[1], sizeValue);
      delays[1].process_samples(passBuffers[3], passBuffers[2], sizeValue);
  
      auto* channelDataR = buffer.getWritePointer(1);
      preDelay2.process_samples(channelDataR,passBuffers[4],preDelayValue);
      allPassFilters[2].process_samples(passBuffers[4], passBuffers[5], colorValue, colorGainValue);
      allPassFilters[3].process_samples(passBuffers[4], passBuffers[6], colorValue, colorGainValue);
      ladderFilters[2].process_samples(passBuffers[5],passBuffers[4], dampingValue);
      ladderFilters[3].process_samples(passBuffers[6],passBuffers[7], dampingValue);
      delays[2].process_samples(passBuffers[4], passBuffers[5], sizeValue);
      delays[3].process_samples(passBuffers[7], passBuffers[6], sizeValue);

  
      //hardmax matrix
      for(int sample = 0; sample < numSamples; sample++){
        passBuffers[1][sample]-=passBuffers[2][sample];
        passBuffers[1][sample]+=passBuffers[2][sample];
        passBuffers[5][sample]-=passBuffers[6][sample];
        passBuffers[5][sample]+=passBuffers[6][sample];
        passBuffers[1][sample]-=passBuffers[5][sample];
        passBuffers[1][sample]+=passBuffers[5][sample];
        passBuffers[2][sample]-=passBuffers[6][sample];
        passBuffers[2][sample]+=passBuffers[6][sample];
      }
  
      //write to left audio channel
      for(int sample = 0; sample < numSamples; sample++){
        writeBufferL[sample] = passBuffers[0][sample] + passBuffers[3][sample];
        channelDataL[sample] = mix(channelDataL[sample],writeBufferL[sample],dryWetValue);
      }//for
      //write to right audio channel
      for(int sample = 0; sample < numSamples; sample++){
        writeBufferR[sample] = passBuffers[4][sample] + passBuffers[7][sample];
        channelDataR[sample] = mix(channelDataR[sample],writeBufferR[sample],dryWetValue);
      }//for
        
}

float JVerbAudioProcessor::mix(float a, float b, float *m){
  //retrun linear interpolated value of two numbers
  return a * (1.-*m) + b * *m;
}

void JVerbAudioProcessor::onDAWChange(int samplerate, int numSamples){
  //if samplerate or framesize changes reeinitialize buffers and other stuff that depends on framesize or samplerate
  if(samplerate != oldSamplerate || numSamples != oldNumSamples){
    preDelay1.setup(samplerate, numSamples);
    preDelay2.setup(samplerate, numSamples);
    
    delete []writeBufferL;
    delete []writeBufferR;
    writeBufferL = new float[numSamples];
    writeBufferR = new float[numSamples];

    for(int i = 0; i < cFdnChanAmnt*2; i++){
      delete [] passBuffers[i];
    }//for
    
    passBuffers = new float*[cFdnChanAmnt*2];
    for(int i = 0; i < cFdnChanAmnt*2; i++){
      passBuffers[i] = new float[numSamples];
      allPassFilters[i/2].setup(samplerate,numSamples);
      delays[i/2].setup(samplerate, numSamples);
      ladderFilters[i/2].setup(samplerate, numSamples);
      for(int j = 0; j < numSamples; j++){
        passBuffers[i][j] = 0.;
        writeBufferL[j] = 0.;
        writeBufferR[j] = 0.;
      }//for
    }//for

    oldSamplerate = samplerate;
    oldNumSamples = numSamples;
  }//if
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
