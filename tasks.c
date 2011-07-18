/*
 * Tasks.c
 *
 *  Created on: 20.01.2011
 *      Author: Kate
 */
#include "avr_compiler.h"
#include "flashlight.h"
#include "tasks.h"
#include "scheduler.h"
#include "buttons.h"

Button_struct_t ButtonPlus, ButtonMinus;

/* Define tasks here. */
inline void initPorts()
{
		DDRB|=(1<<PB0);//!SHDN
		DDRA|=(1<<PA7);//ctrl output
		DDRA|=BOARD_LED_gm;// LEDs
		LED_setNone();

		DIDR0&=0x01;//voltage adc pin

		PORTB|=(1<<PB2);//button0 pullup
		PORTA|=(1<<PA0);//button1 pullup

		ButtonPlus.Counter=0;
		ButtonPlus.CurrentState=FREE;
		ButtonPlus.Port=&PINA;//
		ButtonPlus.Mask=0x01;//PA0

		ButtonMinus.Counter=0;
		ButtonMinus.CurrentState=FREE;
		ButtonMinus.Port=&PINB;
		ButtonMinus.Mask=0x04;//PB2

	//start PWM
		TCCR0A|=(1<<COM0B1);//OC0B non-inverting mode OCR0B OCR0B=0xFF goes for continious high, OCR0B=0x00 = OFF
		TCCR0B|=(1<<CS00);//no prescaler for high frequency
		Brightness_set(0);
}

void ButtonCheckerTask(void)
{
	Button_checkButton(&ButtonMinus);
	Button_checkButton(&ButtonPlus);
	Task_create(ButtonCheckerTask,BUTTON_CHECK_PERIOD,NOT_FROM_ISR);
}


void StateMachineTask (void)
{
	static uint8_t state=0;//initial state

	Click_enum_t MinusClick=FREE, PlusClick=FREE;

	Button_recieveClick(&ButtonMinus,&MinusClick);
	Button_recieveClick(&ButtonPlus,&PlusClick);

	if (PlusClick==LONGCLICK||MinusClick==LONGCLICK) {
		LED_toggleRed();
		Task_create(LED_setNone,198,NOT_FROM_ISR);
	}
	if (MinusClick==CLICK||PlusClick==CLICK) {
		LED_toggleGreen();
		Task_create(LED_setNone,198,NOT_FROM_ISR);
	}

	switch(state)
	{
		case 0:
			if (MinusClick == LONGCLICK) {state = 3;}
			else if (PlusClick == LONGCLICK) {state = 4;}
			break;
		case 1:
			if ((MinusClick == LONGCLICK)||(PlusClick == LONGCLICK)) {state = 0;}
			if (PlusClick == CLICK) {state++;}
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			if ((MinusClick == LONGCLICK)||(PlusClick == LONGCLICK)) {state = 0;}
			if (PlusClick == CLICK) {
				state++;
				//TODO Task_create(state--,10000,NOT_FROM_ISR)
			}
			if (MinusClick == CLICK) {state--;}
			break;
		case 6://TURBO
			if ((MinusClick == LONGCLICK)||(PlusClick == LONGCLICK)) {state = 0;}
			if (MinusClick == CLICK) {state--;}
			break;
	}
	Brightness_set(state);
	Task_create(StateMachineTask,100,NOT_FROM_ISR);
}


