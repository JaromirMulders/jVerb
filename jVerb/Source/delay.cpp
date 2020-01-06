#include "delay.h"

Delay::Delay(){
  tapin  = 0;
  tapout = 0;
  
  samplerate = 44100;
  numSamples = 512;
  delayBufferSize = nextPowerOfTwo(samplerate);
  delayBufferMask = delayBufferSize-1;

  delayBuffer = new float[delayBufferSize];
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
}

long Delay::nextPowerOfTwo(long n){
    --n;
  
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    return n + 1;
}

//if samplerate or framesize changes
void Delay::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;
  delayBufferSize = nextPowerOfTwo(samplerate);
  delayBufferMask = delayBufferSize-1;
  
  float* newBuffer = new float[delayBufferSize];
  float* tempBuffer = delayBuffer;

  delayBuffer = newBuffer;
  delete[] tempBuffer;
  
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
  
}

void Delay::process_samples(float *samples, float *output, float *delayTime){
  
  for(auto i = 0; i < numSamples; i++){
    
    tapin++;
    tapin&=delayBufferMask;
    
    //set delay time
    tapout = tapin - (int)(*delayTime/250.*(float)samplerate + 1);

    tapout = (tapout + delayBufferSize)&delayBufferMask;
    
    delayBuffer[tapin] = samples[i] + delayBuffer[tapout] * 0.5;
    
    output[i] = delayBuffer[tapin];
    
  }
  
}//delay

Delay::~Delay(){
}
