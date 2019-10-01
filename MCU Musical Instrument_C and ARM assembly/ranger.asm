;
;	ranger.asm
;
;  Created on: Apr 2, 2018
;      Author: Ben Daniel
;

 .cdecls "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "driverlib/timer.h", "launchpad.h", "ranger.h"

RANGER_PORT_PERIPH 	.field	SYSCTL_PERIPH_GPIOC ; The peripheral def of Port C
RANGER_PORT			.field	GPIO_PORTC_BASE     ; The base address of the I2C port
RANGER_PIN			.field	GPIO_PIN_4          ; The 8-bit pin value (mask format) for ranger

TIMER_PERIPH		.field	SYSCTL_PERIPH_WTIMER0
TIMER_BASE			.field	WTIMER0_BASE
GPIO_TIMER_PIN		.field	GPIO_PC4_WT0CCP0

;
; 	void rangerIntrHandler()
;	Interrupt Handler function for ultrasonic ranger
rangerIntrHandler
	PUSH	{lr}

	; Clear interrupt on timer
	; TimerIntClear(WTIMER0_BASE, TIMER_CAPA_EVENT);
	LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_CAPA_EVENT
	BL		TimerIntClear

	POP		{pc}

;
;	void timerInit()
;	Initialize wide timer A for use as input source for interrupt events
;
timerInit
	PUSH	{lr}

	; Call to SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0) to enable timer peripheral
	LDR		r0, TIMER_PERIPH
	BL		SysCtlPeripheralEnable

	; Configure a timer and sub-timer:
	; TimerConfigure(WTIMER0_BASE,
    ;                (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP));
	LDR		r0, TIMER_BASE
	;MOV		r1, #(TIMER_CFG | TIMER_CFG_CAP) ; legal?
	MOV		r1, #TIMER_CFG_SPLIT_PAIR
	ORR		r1, #TIMER_CFG_A_CAP_TIME_UP
	BL		TimerConfigure

	; Configure event edge types
	; TimerControlEvent(WTIMER0_BASE, TIMER_A,
    ;                   TIMER_EVENT_BOTH_EDGES);
	LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_A
	MOV		r2, #TIMER_EVENT_BOTH_EDGES
	BL		TimerControlEvent

	; Enable timer and sub-timer:
	; TimerEnable(WTIMER0_BASE, TIMER_A);
	LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_A
	BL		TimerEnable

	; Enable timer as interrupt source
	; TimerIntEnable(WTIMER0_BASE, TIMER_CAPA_EVENT);
	LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_CAPA_EVENT
	BL		TimerIntEnable

	POP		{pc}

;
;	void rangerInit()
;	Intialize ultrasonic ranger
;
rangerInit
	PUSH	{lr}

	; Enable GPIO Port C as peripheral.
	; SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	LDR		r0, RANGER_PORT_PERIPH
	BL		SysCtlPeripheralEnable

	POP		{pc}

;
; 	static void sendStartPulse();
;	Send a pulse from the ranger's transmitter
;
sendStartPulse
    PUSH	{lr}

    ; Configure as output to send pulse
    ; GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	LDR		r0, RANGER_PORT
	LDR		r1, RANGER_PIN
	BL		GPIOPinTypeGPIOOutput

    ; Write low signal for TIME_LOW us
    ; GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
    LDR		r0, RANGER_PORT
    LDR		r1, RANGER_PIN
    MOV		r2, #0
    BL		GPIOPinWrite

    ; TIME_LOW = 2;
    ; waitUs(TIME_LOW);
    MOV		r0, #2
    BL		waitUs

    ; Write high signal for TIME_HIGH us
    ; GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);
    LDR		r0, RANGER_PORT
    LDR		r1, RANGER_PIN
    LDR		r2, RANGER_PIN
    BL		GPIOPinWrite

    ; TIME_HIGH = 5
    ; waitUs(TIME_HIGH);
    MOV		r0, #5
    BL		waitUs

    ; Write low signal
    ; GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
	LDR		r0, RANGER_PORT
    LDR		r1, RANGER_PIN
    MOV		r2, #0
    BL		GPIOPinWrite

    ; Configure GPIO pin to be compatible with timer input
    ; GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
    LDR		r0, RANGER_PORT
	LDR		r1, RANGER_PIN
	BL		GPIOPinTypeTimer

	POP		{pc}


;	uint32_t rangerDetect()
;	detect distance in clock cycles sensed by ultrasonic ranger
rangerDetect
    PUSH	{lr}

    ; sendStartPulse();
    BL		sendStartPulse

    ; Route GPIO pin to correct timer
    ; GPIOPinConfigure(GPIO_PC4_WT0CCP0);
    LDR		r0, GPIO_TIMER_PIN
    BL 		GPIOPinConfigure

    ; Clear false interrupt from signal send
    ; rangerIntrHandler();
    ; BL		rangerIntrHandler
    LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_CAPA_EVENT
	BL		TimerIntClear

    ; Wait until the timer senses an interrupt
    ; while (!TimerIntStatus(WTIMER0_BASE, false)) {}
loop_1	LDR		r0, TIMER_BASE
		MOV		r1, #0			; #0 == false
		BL		TimerIntStatus
		CMP		r0, #0			; if return is false
		BEQ		loop_1

    ; Store time of rising edge
    ; rising_time = TimerValueGet(WTIMER0_BASE, TIMER_A);
    LDR		r0, TIMER_BASE
    MOV		r1, #TIMER_A
    BL		TimerValueGet
    PUSH	{r0}			; store return value (rising_time)

    ; Clear interrupt from rising edge
    ; rangerIntrHandler();
    ; BL		rangerIntrHandler
    LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_CAPA_EVENT
	BL		TimerIntClear

    ; Wait until the timer senses an interrupt
    ; while (!TimerIntStatus(WTIMER0_BASE, false)) {}
loop_2	LDR		r0, TIMER_BASE
		MOV		r1, #0			; #0 == false
		BL		TimerIntStatus
		CMP		r0, #0			; if return is false
		BEQ		loop_2

    ; Store time of falling edge
    LDR		r0, TIMER_BASE
    MOV		r1, #TIMER_A
    BL		TimerValueGet
    PUSH	{r0}			; store return value (falling_time)

	; Now, falling_time is located at sp
	; 	   rising_time  is located at sp, #4
	; falling_time will be popped first as a result

    ; Clear interrupt from falling edge
    ; rangerIntrHandler();
    ; BL		rangerIntrHandler
    LDR		r0, TIMER_BASE
	MOV		r1, #TIMER_CAPA_EVENT
	BL		TimerIntClear

    ; Return time difference in clock cycles
    ; return (falling_time - rising_time);
    POP		{r0, r1} 	; r0 <- falling_time, r1 <- rising_time
    SUB		r0, r0, r1	; difference in r0 for return

    POP		{pc}
