#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Delay
{
public:
  Delay();
  ~Delay();

  
  void setup(int cSamplerate, int cNumSamples);
  void process_samples(float *samples, float *output, float *lfo, float *delayTime);
  
private:
  long nextPowerOfTwo(long n);
  
  long tapin;
  long tapout;
  long delayBufferSize;
  long delayBufferMask;
  
  float* delayBuffer;

  //==============================================================================
  int samplerate;
  int numSamples;
  
};
