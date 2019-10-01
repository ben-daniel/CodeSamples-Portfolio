/*
 * timer.h
 *
 *  Created on: Mar 13, 2018
 *      Author: Ben Daniel, Kevin Calva
 */

#ifndef TIMER_H_
#define TIMER_H_

void rangerIntrHandler();

void timerInit();

//void ADCInit();

void rangerInit();

static void sendStartPulse();

uint32_t rangerDetect();

#endif /* TIMER_H_ */
