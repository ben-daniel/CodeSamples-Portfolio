#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include <Adafruit_TLC5947.h>

// How many frets (include zeroth fret)?
#define NUM_FRETS 22
#define MAX_CAPO 12

#define data_pin   4
#define clock_pin   5
#define latch_pin   6
#define oe  -1  // set to -1 to not use the enable pin (its optional)

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_FRETS, clock_pin, data_pin, latch_pin);

enum colors {red, green, blue, black, white, yellow, purple, cyan};
enum colors col;

uint8_t capo = 0;
uint16_t chord_shift = 0;
int scale_shift = 0;
bool wait_mode;

uint8_t none = white;
uint8_t index = blue;
uint8_t middle = green;
uint8_t ring = cyan;
uint8_t pinky = purple;
uint8_t thumb = yellow;
uint8_t mute = black;

uint16_t on = 400;
uint16_t off = 0;

uint16_t r = 0;
uint16_t b = 0;
uint16_t g = 0;

enum noteFret { blank100, blank101, e_0, B_0, G_0, D_0, A_0, E_0,
                blank0, blank1, e_1, B_1, G_1, D_1, A_1, E_1,
                blank2, blank3, e_2, B_2, G_2, D_2, A_2, E_2,
                blank4, blank5, e_3, B_3, G_3, D_3, A_3, E_3,
                blank6, blank7, e_4, B_4, G_4, D_4, A_4, E_4,
                blank8, blank9, e_5, B_5, G_5, D_5, A_5, E_5,
                blank10, blank11, e_6, B_6, G_6, D_6, A_6, E_6,
                blank12, blank13, e_7, B_7, G_7, D_7, A_7, E_7,
                blank14, blank15, e_8, B_8, G_8, D_8, A_8, E_8,
                blank16, blank17, e_9, B_9, G_9, D_9, A_9, E_9,
                blank18, blank19, e_10, B_10, G_10, D_10, A_10, E_10,
                blank20, blank21, e_11, B_11, G_11, D_11, A_11, E_11,
                blank22, blank23, e_12, B_12, G_12, D_12, A_12, E_12,
                blank24, blank25, e_13, B_13, G_13, D_13, A_13, E_13,
                blank26, blank27, e_14, B_14, G_14, D_14, A_14, E_14,
                blank28, blank29, e_15, B_15, G_15, D_15, A_15, E_15,
                blank30, blank31, e_16, B_16, G_16, D_16, A_16, E_16,
                blank32, blank33, e_17, B_17, G_17, D_17, A_17, E_17,
                blank34, blank35, e_18, B_18, G_18, D_18, A_18, E_18,
                blank36, blank37, e_19, B_19, G_19, D_19, A_19, E_19,
                blank38, blank39, e_20, B_20, G_20, D_20, A_20, E_20,
                blank40, blank41, e_21, B_21, G_21, D_21, A_21, E_21,
				blank42, blank43, e_22, B_22, G_22, D_22, A_22, E_22,
                blank44, blank45, e_23, B_23, G_23, D_23, A_23, E_23,
				blank46, blank47, e_24, B_24, G_24, D_24, A_24, E_24,
                blank48, blank49, e_25, B_25, G_25, D_25, A_25, E_25,
                blank50, blank51, e_26, B_26, G_26, D_26, A_26, E_26,
                blank52, blank53, e_27, B_27, G_27, D_27, A_27, E_27};

// Sets col variable for use in other functions
void color(uint16_t col){
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
    if (col == cyan){
        r = off;
        g = on;
        b = on;
    }
}

#endif
