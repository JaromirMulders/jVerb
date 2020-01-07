#include "allPass.h"

allPass::allPass(){
  tapin  = 0;
  tapout = 0;
  
  samplerate = 44100;
  numSamples = 512;
  delayBufferSize = nextPowerOfTwo(samplerate/4);
  delayBufferMask = delayBufferSize-1;

  delayBuffer = new float[delayBufferSize];
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
}

allPass::~allPass(){
  delete[] delayBuffer;
  delayBuffer = NULL;
}

long allPass::nextPowerOfTwo(long n){
    --n;
  
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    return n + 1;
}

void allPass::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;
  delayBufferSize = nextPowerOfTwo(samplerate/4);
  delayBufferMask = delayBufferSize-1;
  
  delete[] delayBuffer;
  delayBuffer = new float[delayBufferSize];
  
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
}

void allPass::process_samples(float *samples, float *output,float *color, float *colorGain){
 
  int delaytime = (int)*color;
  
  for(auto i = 0; i < numSamples; i++){
    tapin++;
    tapin&=delayBufferMask;
    
    //set delay time
    tapout = tapin - delaytime;

    tapout = (tapout + delayBufferSize)&delayBufferMask;
    
    delayBuffer[tapin] = samples[i] + delayBuffer[tapout] * *colorGain;
    
    output[i] = delayBuffer[tapout] + delayBuffer[tapin] * -*colorGain;
  }
  
}//delay
