/*
 * main.c
 *
 *  Created on: Apr 10, 2018
 *      Authors: Ben Daniel, Kevin Calva
 *
 *  Rotary Sensor connected to J8 controls volume
 *  Rotary Sensor connected to J7 controls pulse tempo
 *  Speaker connected to J5
 *  Ultrasonic ranger connected to J16
 *
 *  Potential additions:
 *      Add another ultrasonic sensor to increase range of playable notes
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "launchpad.h"
#include "ranger.h"
#include "rotary.h"
#include "pwmspeaker.h"
#include "pwmled.h"

const uint32_t speed_sound = 343; //speed of sound in m/s

// in inches
const uint16_t min_distance = 1;
const uint16_t max_distance = 25; // 2' 1"
const uint16_t window_size = 2;
const uint16_t windows = 12; // (max_distance - min_distance) / window_size

const uint16_t period_default = 3500;

// run/pause mode for entire system.
// controlled by SW1.
static volatile enum {
     RUN, PAUSE
} mode = PAUSE;

static volatile enum{
    RUNDUR, PAUSEDUR
} durMode = PAUSEDUR;

// on/off for pulsation of notes
static volatile enum {
     ON, OFF
} play = OFF;

//HAVE QUESTIONS ABOUT THIS LINE
// defines parameters for PWM note.
typedef struct {
    pwm_t pwm;              // PWM parameters
    uint16_t duration;      // Duration of this PWM segment
} pwmSeg_t;

// used for indexing periods[] array.
enum {
    G5, Ab5, A5, Bb5, B5,
    C6, Db6, D6, Eb6, E6, F6, Gb6, G6, Ab6, A6, Bb6, B6,
    C7, Db7, D7, Eb7, E7, F7, Gb7, G7, Ab7, A7, Bb7, B7,
    C8, Db8, D8, Eb8, E8, F8, Gb8, G8, Ab8, A8, Bb8, B8
} note_name = G5;

// calculated by clock_rate/frequency
// change clock_rate?? would lower period values - could play lower notes with 16bit periods
uint16_t periods[41] =
    {63776, // G5
    60197, // Ab5
    56818, // A5
    53629, // Bb5
    50619, // B5
    47778, // C6
    45097, // Db6
    42566, // D6
    40176, // Eb6
    37922, // E6
    35793, // F6
    33784, // Gb6
    31888, // G6
    30098, // Ab6
    28409, // A6
    26815, // Bb6
    25310, // B6
    23889, // C7
    22548, // Db7
    21283, // D7
    20088, // Eb7
    18961, // E7
    17897, // F7
    16892, // Gb7
    15944, // G7
    15049, // Ab7
    14205, // A7
    13407, // Bb7
    12655, // B7
    11945, // C8
    11274, // Db8
    10641, // D8
    10044, // Eb8
    9480,  // E8
    8948,  // F8
    8446,  // Gb8
    7972,  // G8
    7525,  // Ab8
    7102,  // A8
    6704,  // Bb8
    6327  // B8
};

// size is windows from above
uint16_t scale1[12] =
{
    C6, D6, E6, F6, G6, A6, B6, C7, D7, E7, F7, G7
    //Eb6, F6, G6, Ab6, Bb6, C7, D7, Eb7, F7, G7, Ab7, Bb7

};

// size is windows from above
const pwm_t red_LED[12] =
{
    {10, 100}, // matches with first note in scale array
    {0, 100},
    {2, 100},
    {15, 100},
    {8, 100},
    {13, 100}, // m6
    {0, 100},
    {10, 100}, // root
    {0, 100},
    {2, 100},
    {15, 100},
    {8, 100}
};

// size is windows from above
const pwm_t green_LED[12] =
{
    {10, 100}, // matches with first note in scale array
    {3, 100},
    {15, 100},
    {8, 100},
    {8, 100},
    {2, 100}, // m6
    {10, 100},
    {10, 100}, // root
    {3, 100},
    {15, 100},
    {8, 100},
    {8, 100}
};

// size is windows from above
const pwm_t blue_LED[12] =
{
    {10, 100}, // matches with first note in scale array
    {15, 100},
    {8, 100},
    {0, 100},
    {12, 100},
    {10, 100}, // m6
    {15, 100},
    {10, 100}, // root
    {15, 100},
    {8, 100},
    {0, 100},
    {12, 100}
};

//uint16_t scale1[18] =
//{
//    C6, D6, E6, F6, G6, A6, B6,
//    C7, D7, E7, F7, G7, A7, B7,
//    C8, D8, E8, F8
//};
//
//// size is windows from above
//pwm_t red_LED[18] =
//{
//    {10, 100}, // matches with first note in scale array
//    {0, 100},
//    {8, 100},
//    {15, 100},
//    {8, 100},
//    {13, 100}, // m6
//    {0, 100},
//    {10, 100}, // root
//    {0, 100},
//    {8, 100},
//    {15, 100},
//    {8, 100},
//    {13, 100}, // m6
//    {0, 100},
//    {10, 100}, // root
//    {0, 100},
//    {8, 100},
//    {15, 100}
//};
//
//// size is windows from above
//pwm_t green_LED[18] =
//{
//    {10, 100}, // matches with first note in scale array
//    {15, 100},
//    {12, 100},
//    {8, 100},
//    {8, 100},
//    {2, 100}, // m6
//    {10, 100},
//    {10, 100}, // root
//    {15, 100},
//    {12, 100},
//    {8, 100},
//    {8, 100},
//    {2, 100}, // m6
//    {10, 100},
//    {10, 100}, // root
//    {15, 100},
//    {12, 100},
//    {8, 100}
//};
//
//// size is windows from above
//pwm_t blue_LED[18] =
//{
//    {10, 100}, // matches with first note in scale array
//    {15, 100},
//    {0, 100},
//    {0, 100},
//    {12, 100},
//    {10, 100}, // m6
//    {15, 100},
//    {10, 100}, // root
//    {15, 100},
//    {0, 100},
//    {0, 100},
//    {12, 100},
//    {10, 100}, // m6
//    {15, 100},
//    {10, 100}, // root
//    {15, 100},
//    {0, 100},
//    {0, 100}
//};

//// 16th notes in ms for BPM 10 - 500 (non-linear spacing)
uint16_t durations[100] =
{
    1500,1000,750, 653, 577, 517, 469, 429, 395, 366, // 0 - 9
    341, 319, 300, 288, 278, 268, 259, 250, 242, 234,
    227, 221, 214, 208, 202, 197, 192, 188, 183, 179, // 20 - 29
    174, 170, 167, 163, 160, 156, 153, 150, 147, 144,
    142, 139, 136, 134, 132, 129, 127, 125, 123, 121, // 40 - 49
    119, 117, 115, 114, 112, 110, 109, 107, 106, 104,
    103, 101, 100, 98,  96,  94,  93,  91,  89,  88,  // 60 - 69
    86,  85,  83,  81,  79,  77,  75,  73,  71,  70,
    68,  65,  63,  60,  58,  56,  54,  52,  49,  47,  // 80 - 89
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46
};

//void checkPushButton(uint32_t time){
//    int code = pbRead();
//    uint32_t delay;
//
//    switch (code) {
//    case 1:                                     // SW1:
//        if(mode == PAUSE)
//        {
//            mode = RUN;
//            uprintf("%s\n\r", "RUN");
//        }
//        else // if(mode == RUN)
//        {
//            mode = PAUSE;
//            uprintf("%s\n\r", "PAUSE");
//        }
//        delay = 250;
//        break;
//
////    case 2:                                     // SW2:
////        break;
//
//    default:
//        delay = 10;
//    }
//
//    schdCallback(checkPushButton, time + delay);
//}

/*
 * Interrupt handler for both push buttons (pins PF0 and PF4)
 */
void
pbIntrHandler()
{
    // Clear interrupt. This is necessary, otherwise the interrupt handler will be executed forever.
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    uint32_t code = pbRead();

    if(code == 1) // SW1
    {
        if(mode == PAUSE)
        {
            mode = RUN;
            play = ON;
        }
        else // if(mode == RUN)
        {
            mode = PAUSE;
        }
    }
    else if(code == 2)//SW2
    {
        if(durMode == PAUSEDUR){
            durMode = RUNDUR;
            play = ON;
        }
        else{
            durMode = PAUSEDUR;
        }
    }
}

/*
 * Select a set of interrupts that can wake up the LaunchPad
 */
void
setInterrupts()
{
    // Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)
    GPIOIntRegister(GPIO_PORTF_BASE, pbIntrHandler);            // register the interrupt handler
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);    // enable interrupts on SW1 and SW2 input
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,    // interrupt on falling edge, note that SW1 and SW2 are active low
            GPIO_FALLING_EDGE);
    IntPrioritySet(INT_GPIOF, 7);                           // raise interrupt level to 7
}

void main_loop(uint32_t time_)
{
    uint32_t delay = 100; // default time if duration not controlled by RUNDUR (pulsing sound)

    uint32_t time = 300000;
    uint32_t rot_percent;
    uint32_t distance;

    pwm_t note = {0, 3500};
    pwm_t red = {1, 100};
    pwm_t green = {1, 100};
    pwm_t blue = {1, 100};

    uint16_t period = 2; // could be any number less than 2^16
    // uint32_t pulse_width; // dictates volume of PWM note
    uint16_t duration = 100; // dictates rhythm of PWM playback

    //Distance in inches = (falling edge time – rising edge time) * speed_of_sound / 2
    if(mode == RUN)
    {
        if(play == ON)
        {
            // Period **************************************************************

            time = rangerDetect();
            distance = (time * speed_sound)/2540000; // distance in inches

            // generates PWM period from distance
            if(distance >= min_distance && distance < max_distance)
            {
                uint16_t i;
                for(i = 0; i < windows; i++)
                {
                    if(distance >= (min_distance + i*window_size)           // lower bound of given window
                    && distance < (min_distance + (i + 1)*window_size) )    // upper bound of given windwo
                    {
                        period = periods[scale1[i]];

                        red = red_LED[i];
                        green = green_LED[i];
                        blue = blue_LED[i];
                    }
                }
                note.period = period;
//                // Pulse Width *********************************************************

                // adc0 (rotary sensor connected to J8) controls volume
                rot_percent = (adcRead() * 100)/4095;
                // flip direction of turn:
                rot_percent = 100 - rot_percent;
                // could run into issues here... will easily exceed 16 bit
                // pulse_width = period * rot_percent / 500;
                // workaround:
                uint32_t pulse_width100 = period * rot_percent / (500); // sets range of duty cycle as 0% - 20%
                // after execution, pulse_width100 < 2^16, so we can do:
                note.pulse_width = pulse_width100;
            }
            else // out of range
            {
                period = period_default; // could be any number less than 2^16
                note.pulse_width =  0; // silent

                red.period = 100;
                red.pulse_width = 0; // off
                green.period = 100;
                green.pulse_width = 0; // off
                blue.period = 100;
                blue.pulse_width = 0; // off
            }

            // Duration ************************************************************
            if(durMode == RUNDUR)
            {
                //adc1 (rotary sensor connected to J7) controls duration
                rot_percent = (adc1Read() * 100)/4095;

                if(rot_percent >= 90) // 0 <= rot_percent <= 99
                {
                    duration = 50; // avoids issue w/ system freezing at short durations
                }
                else
                {
                    duration = durations[rot_percent];
                }
                play = OFF;
            }
            else // if(durMode == PAUSEDUR)
            {
                duration = delay;
            }

            // Set up PWM parameters for the speaker
            note.period = period;
            // note.pulse_width =  pulse_width;
            pwmSetVolume(note);
            pwmledSetColor(red, green, blue);

            //schdCallback(main_loop, time_ + (uint32_t)duration);
            schdCallback(main_loop, time_ + duration);
        } // end if(play == ON)
        else // if (play == OFF)
        {
            note.period = period_default; // could be any number less than 2^16
            note.pulse_width =  0; // silent
            pwmSetVolume(note);

            red.period = 100;
            red.pulse_width = 0; // off
            green.period = 100;
            green.pulse_width = 0; // off
            blue.period = 100;
            blue.pulse_width = 0; // off
            pwmledSetColor(red, green, blue);

            // Duration********************************************
            //adc1 (rotary sensor connected to J7) controls duration
            rot_percent = (adc1Read() * 100)/4095;

            if(rot_percent >= 90)
            {
                duration = 50; // avoids freezing at short durations
            }
            else
            {
                duration = durations[rot_percent];
            }

            play = ON;

            schdCallback(main_loop, time_ + duration);
        } // end else if(play == OFF)
    } // end if(mode == RUN)
    else // if(mode == PAUSE)
    {
        note.period = period_default; // default value
        note.pulse_width =  0; // silent
        pwmSetVolume(note);

        red.period = 100;
        red.pulse_width = 0;
        green.period = 100;
        green.pulse_width = 0;
        blue.period = 100;
        blue.pulse_width = 0;
        pwmledSetColor(red, green, blue);

        schdCallback(main_loop, time_ + delay);
    } // end else if(mode == PAUSE)
} // end main_loop()

void main(void)
{
    // Initialize the launchpad, timer, ranger, rotary sensors
    lpInit();
    timerInit();
    pwmledInit();
    speakerInit();
    rangerInit();
    rotaryInit();

    // set pushbutton interrupts
    setInterrupts();

    uprintf("%s\n\r", "Final Project - Testing2");

    // schedule first callbacks
    schdCallback(main_loop, 1000);
    //schdCallback(checkPushButton, 1005);

    while(true)
    {
        // inf loop
        schdExecute();
    }
} // end main()
