; pwmbuzzer.asm: Assembly PWM-BUZZER functions
; Lab 8, ECE 266, spring 2018
; Created by Kevin Calva
; Buzzer/speaker to connect to PD0 / J5

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_ints.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmspeaker.h","pwmled.h"
                .text

; Field and any other declarations
PORTD           .field  GPIO_PORTD_BASE
TIMER2_PERIPH   .field  SYSCTL_PERIPH_WTIMER2
BUZZER_PERIPH   .field  SYSCTL_PERIPH_GPIOD
TIMER2          .field  WTIMER2_BASE

PIN_ROUTE_1     .field  GPIO_PD0_WT2CCP0

; void buzzerInit()
                .asmfunc
speakerInit      PUSH    {lr}

                ; Enable Timer 2 and PORT D
                ;   call SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2)
                ;   call SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD)
                LDR     r0, TIMER2_PERIPH
                BL      SysCtlPeripheralEnable

                LDR     r0, BUZZER_PERIPH
                BL      SysCtlPeripheralEnable

                ; Connect pins to those timers
                ;   call GPIOPinTypeTimer(PORTD, GPIO_PIN_0)
                ;   call GPIOPinConfigure(GPIO_PD0_WT2CCP0)
                LDR     r0, PORTD
                MOV     r1, #GPIO_PIN_0
                BL      GPIOPinTypeTimer

                LDR     r0, PIN_ROUTE_1
                BL      GPIOPinConfigure

                ; Select PWM for WTimer 2 sub-Timer A
                ;   call TimerConfigure(TIMER2, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM))
                LDR     r0, TIMER2
                MOV     r1, #TIMER_CFG_SPLIT_PAIR
                ORR     r1, #TIMER_CFG_A_PWM
                BL      TimerConfigure

                ; Invert the PWM waveform, so that the Match register value is the pulse width.
                ; Otherwise, the pulse width will be (Load value) - (Match value).
                ; The inversion is done by enabling output inversion on the PWM pins.
                ;   call TimerControlLevel(TIMER2, TIMER_A, true /* output inversion */)
                LDR     r0, TIMER2
                MOV     r1, #TIMER_A
                MOV     r2, #1
                BL      TimerControlLevel

                ;   call TimerLoadSet(TIMER2, TIMER_A, 3500)
                ;   call TimerMatchSet(TIMER2, TIMER_A, 0)
                LDR     r0, TIMER2
                MOV     r1, #TIMER_A
                MOV     r2, #3500
                BL      TimerLoadSet

                LDR     r0, TIMER2
                MOV     r1, #TIMER_A
                MOV     r2, #0
                BL      TimerMatchSet

                ; Enable the WTimer 2's TimerA
                ;   call TimerEnable(TIMER2, TIMER_A)
                LDR     r0, TIMER2,
                MOV     r1, #TIMER_A
                BL      TimerEnable

                POP     {pc}
                .endasmfunc

; void pwmSetVolume(pwm_t volume)
pwmSetVolume  	.asmfunc
				PUSH    {lr, r0}            		; Push r0 for later function calls
				MOV     r2, r0, LSR #16             ; r2 = volume.period
			  	LDR     r0, TIMER2
              	MOV     r1, #TIMER_A
                BL      TimerLoadSet

                LDR     r0, TIMER2
                MOV     r1, #TIMER_A
     			LDR		r2, [sp,#0]               	; r2 = pushed r0
     			BFC		r2, #16, #16             	; clear bits 31-16, i.e. r2 = volume.pulse_width
                BL      TimerMatchSet
                POP     {r0, pc}
                .endasmfunc
