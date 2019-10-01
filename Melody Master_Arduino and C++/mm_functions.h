#ifndef _TEST_HEADER_H
#define _TEST_HEADER_H

#include <Arduino.h>
#include <Adafruit_TLC5947.h>

// Sets col variable for use in other functions
//void color();

// Color chord
void colorChordOld(uint16_t r, uint16_t g, uint16_t b, uint16_t chord_array[],
    Adafruit_TLC5947 tlc);

// Color group of six on one fret
void colorFret(uint16_t r, uint16_t g, uint16_t b, uint16_t fret,
    Adafruit_TLC5947 tlc);

// Color single note
void colorNote(uint16_t r, uint16_t g, uint16_t b, uint16_t note,
    Adafruit_TLC5947 tlc);

// Color entire string on LEDs
void colorString(uint16_t r, uint16_t g, uint16_t b, uint16_t string,
    uint16_t num_frets, Adafruit_TLC5947 tlc);

// Turn off all LEDs
void clearAll(uint16_t num_frets, Adafruit_TLC5947 tlc);
#endif
