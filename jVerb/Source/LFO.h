#pragma once
#include "math.h"

#define cTableSize 512
#define cPi          3.14159265359
#define cTwoPi       6.28318530718

class LFO
{
public:
  //functions
  LFO();
  ~LFO();
  
  void oscGen(float *passOutput,float *sizeOutput ,float *fq, float *depth);
  void makeTable(double shape);
  void setup(int cSamplerate, int cNumSamples);
  
private:
  float  waveTable[cTableSize];
  float fqCounter;
  long   index1;
  long   index2;
  float frac;
  float phasor;
  float wave;
  int tableMask;
  
  int samplerate;
  int numSamples;
  
}; //
