#include "LFO.h"

LFO::LFO() //constructor
{
  for(int i = 0; i < cTableSize; i++){ //fill waveTable with sinewave
    waveTable[i] = sin((float)i/(float)cTableSize * cTwoPi);
  }
  
  tableMask = cTableSize-1;
  fqCounter = 0;
  index1 = 0;
  index2 = 0;
  phasor = 0;
  frac   = 0;
  
}

LFO::~LFO(){
}



void LFO::makeTable(double shape){
  
}

void LFO::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;

}

void LFO::oscGen(float *passOutput,float *sizeOutput ,float *fq, float *depth){
  
  for(int i = 0; i < numSamples; i++){
    frac = (float)cTableSize * *fq / (float)samplerate; //calculate frequency

    fqCounter = fmod(fqCounter,(float)cTableSize);
    
    index1 = (int)fqCounter;
    index2 = index1 + 1; //lookup of index
    index2&=tableMask;
    
    phasor = fqCounter - (float)index1; //getting phase
    
    //linear interpolation
    wave = (waveTable[index1] * (1.0f - phasor)) + (waveTable[index2] * phasor);
    
    fqCounter += frac;
    
    wave*= *depth;
    passOutput[i] = wave;// * (samplerate*0.015);
    sizeOutput[i] = wave;// * (samplerate*0.25);
  }
  
}



