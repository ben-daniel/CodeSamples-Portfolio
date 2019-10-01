
#include <mm_functions.h>
#include <Adafruit_TLC5947.h>

/* Deprecated:
// Sets col variable for use in other functions
void color(){
    if (col == red){ //red
        r = on;
        g = off;
        b = off;
    }
    if (col == green){ //green
        r = off;
        g = on;
        b = off;
    }
    if (col == blue){ //blue
        r = off;
        g = off;
        b = on;
    }
    if (col == black){ //off
        r = off;
        g = off;
        b = off;
    }
    if (col == white){ //white
        r = on;
        g = on;
        b = on;
    }
    if (col == yellow){ //yellow
        r = on;
        g = on;
        b = off;
    }
    if (col == purple){ //purple
        r = on;
        g = off;
        b = on;
    }
}
*/

// Color chord
void colorChordOld(uint16_t r, uint16_t g, uint16_t b, uint16_t chord_array[],
    Adafruit_TLC5947 tlc) {
  for(uint16_t i=0; i<6; i++) {
      tlc.setLED(chord_array[i], r, g, b);
      tlc.write();
  }
}

// Color group of six on one fret
void colorFret(uint16_t r, uint16_t g, uint16_t b, uint16_t fret,
    Adafruit_TLC5947 tlc) {
  for(uint16_t i=8*fret; i<8*(fret+1); i++) {
      tlc.setLED(i, r, g, b);
      tlc.write();
  }
}

// Color single note
void colorNote(uint16_t r, uint16_t g, uint16_t b, uint16_t note,
    Adafruit_TLC5947 tlc) {
  tlc.setLED(note, r, g, b);
  tlc.write();
}

// Color entire string on LEDs
// String is 1-6.
void colorString(uint16_t r, uint16_t g, uint16_t b, uint16_t string,
    uint16_t num_frets, Adafruit_TLC5947 tlc) {
  for(uint16_t i=0+(string+1); i<num_frets*8; i=i+8) {
      tlc.setLED(i, r, g, b);
      tlc.write();
  }
}

// Turn off all LEDs
void clearAll(uint16_t num_frets, Adafruit_TLC5947 tlc) {
    for(uint16_t i=0; i<num_frets*8; i++) {
        tlc.setLED(i, 0, 0, 0);
        //tlc.write();
    }
    tlc.write();
}
