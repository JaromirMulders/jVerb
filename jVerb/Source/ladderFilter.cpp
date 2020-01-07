#include "ladderFilter.h"

LadderFilter::LadderFilter(){
  zi = 0.;
  s0 = 0.;
  s1 = 0.;
  s2 = 0.;
  s3 = 0.;
  
  samplerate = 44100;
  numSamples = 512;
}

LadderFilter::~LadderFilter(){
}

void LadderFilter::setup(int cSamplerate, int cNumSamples){
  samplerate = cSamplerate;
  numSamples = cNumSamples;

}

float LadderFilter::tanhXdX(float x){
    float a = x*x;
    return ((a + 105)*a + 945) / ((15*a + 420)*a + 945);
}

void LadderFilter::process_samples(float *samples, float *output,float *frequency){

  for(auto i = 0; i < numSamples; i++){
    float in1 = samples[i];
    
    float f = *frequency;
    //float r = 0.;//(40/9  * resonance); //with resonance

    float ih = 0.5 * (in1 + zi);
    zi = in1;

    //float t0 = tanhXdX(ih - r * s3);//with resonance
    float t0 = tanhXdX(ih * s3);
    float t1 = tanhXdX(s0);
    float t2 = tanhXdX(s1);
    float t3 = tanhXdX(s2);
    float t4 = tanhXdX(s3);

    float g0 = 1.0f / (1.0f + f*t1);
    float g1 = 1.0f / (1.0f + f*t2);
    float g2 = 1.0f / (1.0f + f*t3);
    float g3 = 1.0f / (1.0f + f*t4);

    float f3 = f*t3*g3;
    float f2 = f*t2*g2*f3;
    float f1 = f*t1*g1*f2;
    float f0 = f*t0*g0*f1;
    
    //float y3 = (g3*s3 + f3*g2*s2 + f2*g1*s1 + f1*g0*s0 + f0*in1) / (1.0f + r*f0); //with resonance
    float y3 = (g3*s3 + f3*g2*s2 + f2*g1*s1 + f1*g0*s0 + f0*in1) / 1.0f; //without resonance
    
    //float xx = t0*(in1 - r*y3); //with resonance
    float xx = t0*(in1);
    float y0 = t1*g0*(s0 + f*xx);
    float y1 = t2*g1*(s1 + f*y0);
    float y2 = t3*g2*(s2 + f*y1);

    s0 += 2*f * (xx - y0);
    s1 += 2*f * (y0 - y1);
    s2 += 2*f * (y1 - y2);
    s3 += 2*f * (y2 - t4*y3);

    output[i] = y3;
  }
  
}
