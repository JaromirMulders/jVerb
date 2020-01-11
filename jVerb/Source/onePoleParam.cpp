#include "onePoleParam.h"

OnePoleParam::OnePoleParam(){
  samplerate = 44100;
  numSamples = 512;
  history = 0.;
}

OnePoleParam::~OnePoleParam(){
}

void OnePoleParam::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;

}

void OnePoleParam::process_samples(float &paramater, float frequency){
  for(auto i = 0; i < numSamples; i++){
    paramater = history * (1.-frequency) + paramater * frequency;
    history = paramater;
  }//for
}
