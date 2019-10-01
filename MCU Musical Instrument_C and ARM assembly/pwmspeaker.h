/*
 * pwmbuzzer.h
 *
 *  Created on: Apr 3, 2018
 *      Author: USER
 */

#ifndef PWMSPEAKER_H_
#define PWMSPEAKER_H_

#include <stdint.h>
#include "pwmled.h"

// The PWM parameters
//typedef struct {
//    uint16_t pulse_width;       // PWM pulse width
//    uint16_t period;            // PWM period in cycles
//};

void speakerInit();

//function to control the volume
void pwmSetVolume(pwm_t volume);

#endif /* PWMSPEAKER_H_ */
