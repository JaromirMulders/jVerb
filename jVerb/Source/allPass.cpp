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
  
  delayBuffer = new float[delayBufferSize];
  
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
}

void allPass::process_samples(float *samples, float *output , float *lfo,float *color, float *colorGain){
 
  for(auto i = 0; i < numSamples; i++){
    
    
    float dColor = oldColor * 0.9995 + *color * 0.0005;
    oldColor = dColor;
    
    tapin++;
    tapin&=delayBufferMask;
    
    //set delay time
    tapout =  tapin - (int)(dColor + lfo[i]);
    tapout = (tapout + delayBufferSize)&delayBufferMask;
    
    delayBuffer[tapin] = samples[i] + delayBuffer[tapout] * *colorGain;
    
    output[i] = delayBuffer[tapout] + delayBuffer[tapin] * -*colorGain;
  }
  
}//delay
