#include "delay.h"

Delay::Delay(){
  tapin  = 0;
  tapout = 0;
  
  samplerate = 44100;
  numSamples = 512;
  delayBufferSize = nextPowerOfTwo(samplerate/2);
  delayBufferMask = delayBufferSize-1;

  delayBuffer = new float[delayBufferSize];
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
}

Delay::~Delay(){
  delete[] delayBuffer;
  delayBuffer = NULL;
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
  
  delete[] delayBuffer;
  delayBuffer = new float[delayBufferSize];
  
  for(auto i = 0; i < delayBufferSize; i++){
    delayBuffer[i] = 0.0f;
  }
  
}

void Delay::process_samples(float *samples, float *output,float *lfo, float *delayTime){
  
  int iDelaytime = (int)(*delayTime);
  
  for(auto i = 0; i < numSamples; i++){
    
    iDelaytime+=lfo[i];
    
    tapin++;
    tapin&=delayBufferMask;
    
    //set delay time
    tapout = tapin - iDelaytime;

    tapout = (tapout + delayBufferSize)&delayBufferMask;
    
    delayBuffer[tapin] = samples[i];
    
    output[i] = delayBuffer[tapout];
    
  }
  
}//delay


