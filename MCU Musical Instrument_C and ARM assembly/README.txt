The body of C and ARMv7 assembly code in this directory was part of a class
project for Embedded Systems. The code was primarily written by myself, but I
worked with a partner on the code and design of the system. From a high-level,
the code is loaded onto a Texas Instruments MCU, which connects to an RGB LED,
an ultrasonic distance sensor, a speaker (with a low-pass filter before it),
and two potentiometers as controls. The code handles inputs from the various
sensors and plays back PWM tones through the speaker to make a simple musical
tool. Distance from the sensor is converted to various musical pitches as well
as various colors on the RGB LED.

The header files and main function was written in C, and the supporting
functions for routing I/O on the sensors was written in ARMv7 assembly to
practice developing code for limited memory applications. It's not the most
well-organized code I've written as it was written about a year and a half ago,
but it shows that I am familiar coding systems that integrate hardware sensors
to accomplish a task.
