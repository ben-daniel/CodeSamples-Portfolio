/*
 * rotary.h
 *
 *  Created on: Mar 6, 2018
 *      Author: Ben Daniel
 */

#ifndef ROTARY_H_
#define ROTARY_H_

// Initialize rotary sensor
void rotaryInit();

// Read from ADC0
uint16_t adcRead();

//Read from ADC1
uint16_t adc1Read();

#endif /* ROTARY_H_ */
