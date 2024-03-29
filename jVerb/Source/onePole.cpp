#include "onePole.h"

OnePole::OnePole(){
  samplerate = 44100;
  numSamples = 512;
  history = 0.;
}

OnePole::~OnePole(){
}

void OnePole::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;

}

void OnePole::process_samples(float *samples, float frequency){
  for(auto i = 0; i < numSamples; i++){
    samples[i] = history * (1. - frequency) + samples[i] * frequency;
    history = samples[i];
  }//for
}
