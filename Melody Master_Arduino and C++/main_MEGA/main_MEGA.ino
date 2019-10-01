/***************************************************
  Based on an example for Adafruit 24-channel PWM/LED driver

  These drivers uses SPI to communicate, 3 pins are required to
  interface: Data, Clock and Latch. The boards are chainable

  Example code written by Limor Fried/Ladyada for Adafruit 
  Industries.
  BSD license, all text above must be included in any 
  redistribution
 ****************************************************

 The circuit:
 * TLC5947 chips attached to Arduino MEGA 2560 as follows:
 ** DATA - pin 4
 ** CLOCK - pin 5
 ** LATCH - pin 6
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 51
 ** MISO - pin 50
 ** CLK - pin 52
 ** CS - pin 53 (for MKRZero SD: SDCARD_SS_PIN)
 * Microphone attached to Arduino MEGA 2560 as follows:
 ** A0 on microphone module - pin A0

 Code written by Ben Daniel, Robert Kurek, David Olsen,
 Michael Neputy for Melody Master, LED guitar teaching device.
 Copyright 2019
 */
 
#include <Adafruit_TLC5947.h>
#include <mm_functions.h>
#include <global_declarations.h>
#include <chord.h>
#include <chord_helper.h>
#include <scale_helper.h>
#include <FFT_helper.h>
#include <settings_helper.h>
#include <SPI.h>
#include <SD.h>
#include "arduinoFFT.h"

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 53;

uint16_t program_counter = 0;
String chord = "";

void setup() {
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  Serial.begin(115200);
//  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  tlc.begin();
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    //digitalWrite(oe, LOW);
  }

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(53, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  uint8_t octave;
  uint8_t scale_pattern;
  if (program_counter == 0) {
    clearAll(NUM_FRETS, tlc);
  }
  int choice = menuSelect();
  if (choice == 1) {
    // Chord Mode
    chord = readChordInput(); // chord should be guaranteed to be valid input
    my_chord.display_LEDs();
  }
  else if (choice == 2) {
    // Scale Mode
    wait_mode = false;
    scale_shift = setScale();
    octave = setOctave();
//    setPattern();
    scaleShifter(scale_shift);
    clearAll(NUM_FRETS, tlc);
    displayScale(octave);
  }
  else if (choice == 3) {
    // scale Pattern Mode
    wait_mode = false;
    setScaleV2(scale_shift, scale_pattern);
    scaleShifterV2(scale_shift, scale_pattern);
    clearAll(NUM_FRETS, tlc);
    displayScaleV2();
  }
  else if (choice == 4) {
    // Wait for scale mode
    wait_mode = true;
    setScaleV2(scale_shift, scale_pattern);
    scaleShifterV2(scale_shift, scale_pattern);
    clearAll(NUM_FRETS, tlc);
    displayScaleV2();
//    scale_shift = setScale();
//    octave = setOctave();
////    setPattern();
//    scaleShifter(scale_shift);
//    clearAll(NUM_FRETS, tlc);
//    displayScaleFFT();
  }
  else if (choice == 0) {
    while(SettingsMenu()) {
    }
  }
  else if(choice == -1) {
    clearAll(NUM_FRETS, tlc);
  }

  program_counter++;
}

int menuSelect() {
  String option = "";
  while (true) {
    Serial.println("MAIN MENU");
    Serial.println("Choose mode:\n"
                    "Chords // Scales // scale Patterns // 'Wait' scale mode // General settings // turn Off");
    while (Serial.available() == 0)
    {
    }
    if (Serial.available() > 0) {
      option = Serial.readString();
      Serial.print("Input: ");
      Serial.println(option);
    }
    // Check for valid entry. If valid return. Else get new input.
    if (option == "c" || option == "C") {
      return 1;
    }
    else if (option == "s" || option == "S") {
      return 2;
    }
    else if (option == "p" || option == "P") {
      return 3;
    }
    else if (option == "w" || option == "W") {
      return 4;
    }
    else if (option == "g" || option == "G") {
      return 0;
    }
    else if (option == "o" || option == "O") {
      return -1;
    }
    Serial.println("Invalid");
  }
}
