#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class allPass
{
public:
  allPass();
  ~allPass();
  void process_samples(float *samples,float *output, float *lfo,float *color, float *colorGain);
  void setup(int cSamplerate, int cNumSamples);
  
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
