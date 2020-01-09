#pragma once

class OnePole
{
public:
  OnePole();
  ~OnePole();
  
  void setup(int cSamplerate, int cNumSamples);
  void process_samples(float *samples, float *output,float *frequency);
  
private:

  int samplerate;
  int numSamples;
  float history;
};
