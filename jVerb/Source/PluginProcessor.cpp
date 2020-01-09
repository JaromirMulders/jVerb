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
  lfoPassBuffer = new float[numSamples];
  lfoSizeBuffer = new float[numSamples];
  writeBufferL = new float[numSamples];
  writeBufferR = new float[numSamples];
  vInputR = new float[numSamples];
  vInputL = new float[numSamples];
  emptyBuffer = new float[numSamples];
  passBuffers = new float*[cFdnChanAmnt*2];
  hardmaxBuffer = new float*[cFdnChanAmnt];
  
  allPassFilters = new allPass[cFdnChanAmnt];
  delays = new Delay[cFdnChanAmnt];
  ladderFilters = new LadderFilter[cFdnChanAmnt];
  
  for(int i = 0; i < cFdnChanAmnt*2; i++){
    passBuffers[i]   = new float[numSamples];
    hardmaxBuffer[i/2] = new float[numSamples];
    for(int j = 0; j < numSamples; j++){
      passBuffers[i][j] = 0.;
      hardmaxBuffer[i/2][j] = 0.;
      writeBufferL[j] = 0.;
      writeBufferR[j] = 0.;
      vInputR[i] = 0.;
      vInputL[i] = 0.;
      emptyBuffer[i] = 0.;
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
    auto dampingParam = std::make_unique<AudioParameterFloat>(DAMPING_ID, DAMPING_NAME, 10.0f, 20000.0f, 10000.0f);
    auto predelayParam = std::make_unique<AudioParameterFloat>(PREDELAY_ID, PREDELAY_NAME, 0.0f, 250.0f, 50.0f);
    auto decayParam = std::make_unique<AudioParameterFloat>(DECAY_ID, DECAY_NAME, 0.0f, 1.0f, 0.5f);
    auto sizeParam = std::make_unique<AudioParameterFloat>(SIZE_ID, SIZE_NAME, 0.0f, 1.0f, 0.5f);
    auto drywetParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 100.0f, 70.0f);
    auto lfofreqParam = std::make_unique<AudioParameterFloat>(LFOFREQ_ID, LFOFREQ_NAME, 0.1f, 20.0f, 10.0f);
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
  delete []lfoPassBuffer;
  lfoPassBuffer = NULL;
  delete []lfoSizeBuffer;
  lfoSizeBuffer = NULL;
  delete []writeBufferL;
  writeBufferL = NULL;
  delete []writeBufferR;
  writeBufferR = NULL;
  delete []vInputR;
  vInputR = NULL;
  delete []vInputL;
  vInputL = NULL;
  delete []emptyBuffer;
  emptyBuffer = NULL;
  for(int i = 0; i < cFdnChanAmnt*2; i++){
    delete [] passBuffers[i];
  }//for
  for(int i = 0; i < cFdnChanAmnt; i++){
    delete [] hardmaxBuffer[i];
  }//for
  
}

void JVerbAudioProcessor::scaleParams(){

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
      auto inputValue = treeState.getRawParameterValue(INPUT_ID);
      float nInputVal = *inputValue*0.5;

      //auto sliderGainValue = treeState.getRawParameterValue(INPUT_ID);
      auto outputValue = treeState.getRawParameterValue(OUTPUT_ID);
      float nOutputVal = *outputValue;
  
      //auto sliderGainValue = treeState.getRawParameterValue(INPUT_ID);
      auto preDelayValue = treeState.getRawParameterValue(PREDELAY_ID);
      float nPreDelayVal = *preDelayValue*0.001*samplerate;
  
      auto colorValue = treeState.getRawParameterValue(COLOR_ID);
      float nColorVal1 = *colorValue*0.015*samplerate;
      float nColorVal2 = nColorVal1 * 0.8876;//* 0.932;
      float nColorVal3 = nColorVal1 * 0.61234;//* 0.865;
      float nColorVal4 = nColorVal1 * 0.45432;//* 0.789;
  
      auto colorGainValue = treeState.getRawParameterValue(COLORGAIN_ID);
      float nColorGainVal = *colorGainValue;
  
      auto sizeValue = treeState.getRawParameterValue(SIZE_ID);
      float nSizeValue1 = *sizeValue*0.25*samplerate;
      float nSizeValue2 = nSizeValue1 * 0.887123;//0.976;
      float nSizeValue3 = nSizeValue1 * 0.69877;//0.854;
      float nSizeValue4 = nSizeValue1 * 0.48887;//0.784;
  
      auto dryWetValue = treeState.getRawParameterValue(DRYWET_ID);
      float nDryWetVal = *dryWetValue*0.01;
 
      auto dampingValue = treeState.getRawParameterValue(DAMPING_ID);
      float nDampingVal = tan(cPi * (*dampingValue * 1./samplerate));

      auto decayValue = treeState.getRawParameterValue(DECAY_ID);
      float nDecayVal = *decayValue*0.5;

      auto lfoFreq = treeState.getRawParameterValue(LFOFREQ_ID);
      float nLfoFreqVal = *lfoFreq;
 
       auto lfoDepth = treeState.getRawParameterValue(LFODEPTH_ID);
       float nLfoDepth = *lfoDepth;
  
      //clear buffers
      for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, numSamples);
      }//for

      auto* channelDataL = buffer.getWritePointer(0);
      auto* channelDataR = buffer.getWritePointer(1);
  
      lfo.oscGen(lfoPassBuffer, lfoSizeBuffer, &nLfoFreqVal, &nLfoDepth);
  
      preDelay1.process_samples(channelDataL,vInputL,emptyBuffer,&nPreDelayVal);
      preDelay2.process_samples(channelDataR,vInputR,emptyBuffer,&nPreDelayVal);
      
      ladderFilters[0].process_samples(hardmaxBuffer[0],passBuffers[1],&nDampingVal);
      ladderFilters[1].process_samples(hardmaxBuffer[1],passBuffers[3],&nDampingVal);
      allPassFilters[0].process_samples(passBuffers[1], passBuffers[0], lfoPassBuffer, &nColorVal1, &nColorGainVal);
      allPassFilters[1].process_samples(passBuffers[3], passBuffers[2], lfoPassBuffer, &nColorVal2, &nColorGainVal);
      delays[0].process_samples(passBuffers[0], passBuffers[1], emptyBuffer, &nSizeValue1);
      delays[1].process_samples(passBuffers[2], passBuffers[3], emptyBuffer, &nSizeValue2);

      ladderFilters[2].process_samples(hardmaxBuffer[2],passBuffers[5], &nDampingVal);
      ladderFilters[3].process_samples(hardmaxBuffer[3],passBuffers[7], &nDampingVal);
      allPassFilters[2].process_samples(passBuffers[5], passBuffers[4], lfoPassBuffer, &nColorVal3, &nColorGainVal);
      allPassFilters[3].process_samples(passBuffers[7], passBuffers[6], lfoPassBuffer, &nColorVal4, &nColorGainVal);
      delays[2].process_samples(passBuffers[4], passBuffers[5], emptyBuffer, &nSizeValue3);
      delays[3].process_samples(passBuffers[6], passBuffers[7], emptyBuffer, &nSizeValue4);
  
      for(int sample = 0; sample < numSamples; sample++){
        float hardmax1_0 = passBuffers[1][sample] - passBuffers[2][sample];
        float hardmax2_0 = passBuffers[1][sample] + passBuffers[2][sample];
        float hardmax3_0 = passBuffers[5][sample] - passBuffers[6][sample];
        float hardmax4_0 = passBuffers[5][sample] + passBuffers[6][sample];
        hardmaxBuffer[0][sample] = vInputL[sample] * nInputVal + tanh(hardmax1_0 - hardmax3_0) * nDecayVal;
        hardmaxBuffer[2][sample] = tanh(hardmax1_0 + hardmax3_0) * nDecayVal;
        hardmaxBuffer[1][sample] = tanh(hardmax2_0 - hardmax4_0) * nDecayVal;
        hardmaxBuffer[3][sample] = vInputR[sample] * nInputVal + tanh(hardmax2_0 + hardmax4_0) * nDecayVal;
        
        writeBufferL[sample] = passBuffers[1][sample] + passBuffers[3][sample];
        channelDataL[sample] = mix(channelDataL[sample],writeBufferL[sample],&nDryWetVal) * nOutputVal;

        writeBufferR[sample] = passBuffers[5][sample] + passBuffers[7][sample];
        channelDataR[sample] = mix(channelDataR[sample],writeBufferR[sample],&nDryWetVal) * nOutputVal;
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
    lfo.setup(samplerate, numSamples);
    lfoSlide.setup(samplerate, numSamples);
    
    delete []lfoPassBuffer;
    delete []lfoSizeBuffer;
    delete []writeBufferL;
    delete []writeBufferR;
    delete []vInputR;
    delete []vInputL;
    delete []emptyBuffer;
    
    vInputR = new float[numSamples];
    vInputL = new float[numSamples];
    lfoPassBuffer = new float[numSamples];
    lfoSizeBuffer = new float[numSamples];
    writeBufferL = new float[numSamples];
    writeBufferR = new float[numSamples];
    emptyBuffer = new float[numSamples];
    
    for(int i = 0; i < cFdnChanAmnt*2; i++){
      delete [] passBuffers[i];
    }//for
    for(int i = 0; i < cFdnChanAmnt; i++){
      delete [] hardmaxBuffer[i];
    }//for
    
    passBuffers = new float*[cFdnChanAmnt*2];
    hardmaxBuffer = new float*[cFdnChanAmnt];
    for(int i = 0; i < cFdnChanAmnt*2; i++){
      passBuffers[i] = new float[numSamples];
      hardmaxBuffer[i/2] = new float[numSamples];
      allPassFilters[i/2].setup(samplerate,numSamples);
      delays[i/2].setup(samplerate, numSamples);
      ladderFilters[i/2].setup(samplerate, numSamples);
      for(int j = 0; j < numSamples; j++){
        passBuffers[i][j] = 0.;
        hardmaxBuffer[i/2][j] = 0.;
        writeBufferL[j] = 0.;
        writeBufferR[j] = 0.;
        vInputL[i] = 0.;
        vInputR[i] = 0.;
        emptyBuffer[i] = 0.;
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
