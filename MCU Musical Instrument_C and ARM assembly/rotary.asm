;
; rotary.asm
;
;  Created on: Mar 6, 2018
;      Author: Ben Daniel
;

 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "rotary.h", "driverlib/adc.h"

					.text
ADC_PERIPH		.field 	SYSCTL_PERIPH_ADC0
ADC_BASE		.field	ADC0_BASE
ADC_TRIGGER_PROC .field	ADC_TRIGGER_PROCESSOR
ADC_IE			.field	ADC_CTL_IE
ADC_END			.field	ADC_CTL_END
ADC_CH4			.field	ADC_CTL_CH4
ADC_CH5			.field  ADC_CTL_CH5
ADC1_BASE_		.field  ADC1_BASE
ADC1_PERIPH		.field SYSCTL_PERIPH_ADC1
;
; void rotaryInit(): Initialze the rotary sensor as input to ADC0
;
rotaryInit      PUSH 	{r4,r5,lr}

                ; Call SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0)
                LDR   	r0, ADC_PERIPH			; into r0 for function call
                BL    	SysCtlPeripheralEnable

                ; Call ADCSequenceConfigure(ADC0_BASE, 0 /* sequencer #0 */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);
                LDR   	r0, ADC_BASE
                MOV   	r1, #0
                LDR		r2, ADC_TRIGGER_PROC
                MOV		r3, #0
                BL    	ADCSequenceConfigure


				; Call ADCSequenceStepConfigure(ADC0_BASE, 0 /* sequencer #0 */,
				;								0 /* step */,
				;								ADC_CTL_IE /* set interrupt flag */
				;								| ADC_CTL_END /* mark the last step */
				;								| ADC_CTL_CH4 /* ADC channel 4 */);
                LDR   	r0, ADC_BASE
                MOV   	r1, #0
                MOV		r2, #0
                ; Perform OR operation on ADC_CTL_IE, ADC_CTL_END, ADC_CTL_CH4
                LDR		r3, ADC_IE
                LDR		r4, ADC_END
                LDR		r5, ADC_CH4
                ORR		r3, r4
                ORR		r3, r5
                ;MOV		r3, #(ADC_IE | ADC_END | ADC_CH4)
                ; Call function
                BL    	ADCSequenceStepConfigure

				; Call ADCSequenceEnable(ADC0_BASE, 0 /* sequencer 0*/);
                LDR   	r0, ADC_BASE
                MOV   	r1, #0
                BL    	ADCSequenceEnable

				; Function calls for ADC 1

			 	;Call SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1)
                LDR   	r0, ADC1_PERIPH			; into r0 for function call
                BL    	SysCtlPeripheralEnable

                 ;Call ADCSequenceConfigure(ADC1_BASE, 0 /* sequencer #0 */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);
                LDR   	r0, ADC1_BASE_
                MOV   	r1, #0
                LDR		r2, ADC_TRIGGER_PROC
                MOV		r3, #0
                BL    	ADCSequenceConfigure

                ; Call ADCSequenceStepConfigure(ADC1_BASE, 0 /* sequencer #0 */,
				;								0 /* step */,
				;								ADC_CTL_IE /* set interrupt flag */
				;								| ADC_CTL_END /* mark the last step */
				;								| ADC_CTL_CH5 /* ADC channel 5 */);
                LDR   	r0, ADC1_BASE_
                MOV   	r1, #0
                MOV		r2, #0
                ; Perform OR operation on ADC_CTL_IE, ADC_CTL_END, ADC_CTL_CH5
                LDR		r3, ADC_IE
                LDR		r4, ADC_END
                LDR		r5, ADC_CH5
                ORR		r3, r4
                ORR		r3, r5
                ; Call function
                BL    	ADCSequenceStepConfigure

				; Call ADCSequenceEnable(ADC1_BASE, 0 /* sequencer 0*/);
                LDR   	r0, ADC1_BASE_
                MOV   	r1, #0
                BL    	ADCSequenceEnable

               POP   	{r4,r5,pc}
;
; uint16_t adcRead(): Read ADC as input
;
adcRead			PUSH 	{lr}

				; Call ADCProcessorTrigger(ADC0_BASE, 0 /* sequencer 0*/);
                LDR   	r0, ADC_BASE
                MOV   	r1, #0
                BL    	ADCProcessorTrigger

;				; while (!ADCIntStatus(ADC0_BASE, 0 /* sequencer #0 */,
;				;		false /* no interrupt mask */))
;while_loop		LDR		r0, ADC_BASE
;				MOV		r1, #0
;				MOV		r2, #0				; r2 = false
;				BL		ADCIntStatus
;				CMP		r0, #0
;				BEQ		while_loop			; compare and branch if r0 == 0 == false

				; Call ADCSequenceDataGet(ADC0_BASE, 0 /* sequencer #0 */,
				;						  &adcReading);
				SUB		sp, #4				; allocate space for adcReading
				LDR   	r0, ADC_BASE
                MOV   	r1, #0
				MOV		r2, sp				; address of adcReading is sp
                BL    	ADCSequenceDataGet

				POP		{r0,pc}
;
; uint16_t adcRead(): Read ADC as input
;
adc1Read			PUSH 	{lr}

				; Call ADCProcessorTrigger(ADC1_BASE, 0 /* sequencer 0*/);
                LDR   	r0, ADC1_BASE_
                MOV   	r1, #0
                BL    	ADCProcessorTrigger

;				; while (!ADCIntStatus(ADC1_BASE, 0 /* sequencer #0 */,
;				;		false /* no interrupt mask */))
;while_loop_		LDR		r0, ADC1_BASE_
;				MOV		r1, #0
;				MOV		r2, #0				; r2 = false
;				BL		ADCIntStatus
;				CMP		r0, #0
;				BEQ		while_loop_			; compare and branch if r0 == 0 == false

				; Call ADCSequenceDataGet(ADC1_BASE, 0 /* sequencer #0 */,
				;						  &adcReading);
				SUB		sp, #4				; allocate space for adcReading
				LDR   	r0, ADC1_BASE_
                MOV   	r1, #0
				MOV		r2, sp				; address of adcReading is sp
                BL    	ADCSequenceDataGet
				POP		{r0,pc}
