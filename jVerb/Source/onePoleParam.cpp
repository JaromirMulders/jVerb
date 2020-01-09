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

void OnePoleParam::process_samples(float samples, float output, float *frequency){
  for(auto i = 0; i < numSamples; i++){
    output = history * (1.-*frequency) + samples * *frequency;
    history = output;
  }//for
}
