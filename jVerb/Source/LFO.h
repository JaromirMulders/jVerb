#pragma once
#include "math.h"
#include "onePole.h"

#define cTableSize   512
#define cPi          3.14159265359
#define cTwoPi       6.28318530718
#define smoothing    0.01

class LFO
{
public:
  //functions
  LFO();
  ~LFO();
  
  void oscGen(float *passOutput,float *sizeOutput ,float *fq, float *depth, int *waveform);
  void makeTable(int shape);
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
  float history;
  
  int samplerate;
  int numSamples;
  
  
}; //
