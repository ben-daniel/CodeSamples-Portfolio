#ifndef _CHORD_H
#define _CHORD_H

#include <global_declarations.h>
#include <mm_functions.h>

const uint8_t NUM_STRINGS = 6;

class Chord {
  private:
    uint8_t frets[NUM_STRINGS];
    uint8_t colors[NUM_STRINGS];
    String chord_name;
  public:
    // constructor function
    Chord() {
        frets[0] = e_0;
        frets[1] = B_0;
        frets[2] = G_0;
        frets[3] = D_0;
        frets[4] = A_0;
        frets[5] = E_0;
        colors[0] = none;
        colors[1] = none;
        colors[2] = none;
        colors[3] = none;
        colors[4] = none;
        colors[5] = none;
    }
    Chord(
      String chord_name_new,
      uint8_t e_fret_new, uint8_t B_fret_new, uint8_t G_fret_new,
      uint8_t D_fret_new, uint8_t A_fret_new, uint8_t E_fret_new,
      uint8_t e_color_new, uint8_t B_color_new, uint8_t G_color_new,
      uint8_t D_color_new, uint8_t A_color_new, uint8_t E_color_new) {
        chord_name = chord_name_new;
        frets[0] = e_fret_new;
        frets[1] = B_fret_new;
        frets[2] = G_fret_new;
        frets[3] = D_fret_new;
        frets[4] = A_fret_new;
        frets[5] = E_fret_new;
        colors[0] = e_color_new;
        colors[1] = B_color_new;
        colors[2] = G_color_new;
        colors[3] = D_color_new;
        colors[4] = A_color_new;
        colors[5] = E_color_new;
     }
    void print_chord() {
      Serial.println(chord_name);
    }
    void display_LEDs() {
      clearAll(NUM_FRETS, tlc);
      //delay(500);
      enum colors col;
      for(uint8_t i=0; i<NUM_STRINGS; i++) {
        if(colors[i] != mute) {
          color(colors[i]);
          tlc.setLED(frets[i], r, g, b);
        }
        else {
          color(red);
          colorString(r, g, b, i+1, NUM_FRETS, tlc);
        }
      }
      tlc.write();
    }

    // mutators
    void set_e_fret(uint8_t e_fret_new) {
        frets[0] = e_fret_new;
    }
    void set_B_fret(uint8_t B_fret_new) {
        frets[1] = B_fret_new;
    }
    void set_G_fret(uint8_t G_fret_new) {
        frets[2] = G_fret_new;
    }
    void set_D_fret(uint8_t D_fret_new) {
        frets[3] = D_fret_new;
    }
    void set_A_fret(uint8_t A_fret_new) {
        frets[4] = A_fret_new;
    }
    void set_E_fret(uint8_t E_fret_new) {
        frets[5] = E_fret_new;
    }
    void set_e_color(uint8_t e_color_new) {
        colors[0] = e_color_new;
    }
    void set_B_color(uint8_t B_color_new) {
        colors[1] = B_color_new;
    }
    void set_G_color(uint8_t G_color_new) {
        colors[2] = G_color_new;
    }
    void set_D_color(uint8_t D_color_new) {
        colors[3] = D_color_new;
    }
    void set_A_color(uint8_t A_color_new) {
        colors[4] = A_color_new;
    }
    void set_E_color(uint8_t E_color_new) {
        colors[5] = E_color_new;
    }

    String get_chord_name() {
      return chord_name;
    }
};

#endif
