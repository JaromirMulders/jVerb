#pragma once
#include "math.h"

class LadderFilter
{
public:
  LadderFilter();
  ~LadderFilter();
  
  void setup(int cSamplerate, int cNumSamples);
  float tanhXdX(float x);
  void process_samples(float *samples, float *output,float *frequency);
  
private:
  float zi;
  float s0;
  float s1;
  float s2;
  float s3;
  
  int samplerate;
  int numSamples;
};
