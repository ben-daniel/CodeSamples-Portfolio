#ifndef _FFT_HELPER_H_
#define _FFT_HELPER_H_

#include "arduinoFFT.h"
#include <Adafruit_TLC5947.h>
#include <SD.h>
#include <mm_functions.h>
#include <global_declarations.h>

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define CHANNEL A0
const uint16_t samples = 256; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 4000; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

const String notefreq_file_name = "notefreq.txt";

double peakFreq(double & magnitude) {
  /*SAMPLING*/
  microseconds = micros();    //Overflows after around 70 minutes!
  for(int i=0; i<samples; i++)
  {
      vReal[i] = analogRead(CHANNEL);
      vImag[i] = 0;
      while(micros() < (microseconds + sampling_period_us)){
        //empty loop
      }
      microseconds += sampling_period_us;
  }
  /* Print the results of the sampling according to time */
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
  double peak_freq = FFT.MajorPeak(vReal, samples, samplingFrequency);
  for (uint16_t i = 0; i < (samples >> 1); i++)
  {
    double abscissa;
    abscissa = ((i * 1.0 * samplingFrequency) / samples);
    if(abs(abscissa - peak_freq) < (samplingFrequency / samples)) {
      magnitude = vReal[i];
//      Serial.println(peak_freq);
      break;
    }
  }
  return peak_freq;
}

// Pass lower and upper frequency bounds.
// Once the detected peak frequency is in the bounds, the function
// will return.
void waitForFreq(double min_freq, double max_freq, double min_mag) {
  double mag;
  double freq = peakFreq(mag);
  // Serial.println("waiting...");
  // while(
  //     (freq < min_freq || freq > max_freq )
  //     ||
  //     mag < min_mag
  // ) {
  while(!(
      ((freq >= min_freq && freq <= max_freq )
      ||
      (freq >= 2*min_freq && freq <= 2*max_freq ))
      &&
      mag > min_mag
  )) {
    freq = peakFreq(mag);
//    Serial.print("freq: ");
//    Serial.println(freq);
//    Serial.print("mag: ");
//    Serial.println(mag);
//    delay(500);
  }
//  Serial.println("Found the Frequency!");
//  delay(2000);
  return;
}

#endif
