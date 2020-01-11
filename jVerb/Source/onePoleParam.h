#pragma once

class OnePoleParam
{
public:
  OnePoleParam();
  ~OnePoleParam();
  
  void setup(int cSamplerate, int cNumSamples);
  void process_samples(float &paramater, float frequency);
  
private:

  int samplerate;
  int numSamples;
  float history;
};

