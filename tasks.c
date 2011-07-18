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

		DDRA|=(1<<PA7);//ctrl output
		PORTA|=(1<<PA7);
		DDRA|=BOARD_LED_gm;
		LED_setNone();

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
	static uint8_t state=3;//initial state

	Click_enum_t MinusClick=FREE, PlusClick=FREE;

	Button_recieveClick(&ButtonMinus,&MinusClick);
	Button_recieveClick(&ButtonPlus,&PlusClick);

	switch(state)
	{
		case 0:
			if ((MinusClick == LONGCLICK)||(PlusClick == LONGCLICK)) {
				state = 3;
				LED_setNone();
				LED_toggleOrange();
				Task_create(LED_toggleOrange,300,NOT_FROM_ISR);//off
				Task_create(LED_toggleOrange,600,NOT_FROM_ISR);//on
				Task_create(LED_toggleOrange,900,NOT_FROM_ISR);//off
			}
			if ((PlusClick == CLICK)||(MinusClick == CLICK)) {
				LED_setNone();
				LED_toggleGreen();
				Task_create(LED_setNone,200,NOT_FROM_ISR);//off
			}

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
			if (MinusClick == CLICK) {
				state--;
				LED_setNone();
				LED_toggleGreen();
				Task_create(LED_setNone,300,NOT_FROM_ISR);//off
			}
			break;
		case 6://TURBO
			if ((MinusClick == LONGCLICK)||(PlusClick == LONGCLICK)) {state = 0;}
			if (MinusClick == CLICK) {
				state--;
				LED_setNone();
				LED_toggleGreen();
				Task_create(LED_setNone,1000,NOT_FROM_ISR);//off
			}
			break;
	}
	Brightness_set(state);
	Task_create(StateMachineTask,100,NOT_FROM_ISR);
}



