#include "LFO.h"

LFO::LFO() //constructor
{
  for(int i = 0; i < cTableSize; i++){ //fill waveTable with sinewave
    waveTable[i] = sin((float)i/(float)cTableSize * cTwoPi);
    /*
    float f = 1.;
    if(((float)i/(float)cTableSize) > 0.5){
      f = -1.;
    }
    waveTable[i] = f;
    */
  }
  
  tableMask = cTableSize-1;
  fqCounter = 0;
  index1 = 0;
  index2 = 0;
  phasor = 0;
  frac   = 0;
  wave   = 0.;
  history = 0.;
  
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
    
    //one pole filter lfo
    //float lfoOut = history * 0.999 + wave * 0.001;
    //history = lfoOut;
    
    wave*= *depth;
    
    //send to output and multiply by the buffersize of the allpass filters and delays
    passOutput[i] = wave * ((float)samplerate * 0.015);
    sizeOutput[i] = wave * ((float)samplerate * 0.25);
  }
  
}



