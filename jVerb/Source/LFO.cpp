#include "LFO.h"

LFO::LFO() //constructor
{
  //generate table with sine wave
  makeTable(1);
  
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



void LFO::makeTable(int shape){
  
  static int oldShape = 0;
  
  if(oldShape != shape){
  
    if( shape == 1){
      for(int i = 0; i < cTableSize; i++){ //fill waveTable with sinewave
        waveTable[i] = sin((float)i/(float)cTableSize * cTwoPi);
      }//for
    }else if(shape == 3){
      for(int i = 0; i < cTableSize; i++){ //fill waveTable with sinewave
        float f = 1.;
        if(((float)i/(float)cTableSize) > 0.5){
          f = -1.;
        }//if
        waveTable[i] = f;
      }//for
    }
    
    oldShape = shape;
  }//if
  
}//make table

void LFO::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;

}

void LFO::oscGen(float *passOutput,float *sizeOutput ,float *fq, float *depth, int *waveform){
  
  for(int i = 0; i < numSamples; i++){
    makeTable(*waveform);
    
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
    float lfoOut = history * 0.999 + wave * 0.001;
    history = lfoOut;
    
    lfoOut*= *depth;
    
    //send to output and multiply by the buffersize of the allpass filters and delays
    passOutput[i] = lfoOut * ((float)samplerate * 0.015);
    sizeOutput[i] = lfoOut * ((float)samplerate * 0.25);
  }
  
}



