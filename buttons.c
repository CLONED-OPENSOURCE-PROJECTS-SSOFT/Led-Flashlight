/*
 * buttons.c
 *
 *  Created on: 19.03.2011
 *      Author: Yuriy
 */
#include "avr_compiler.h"
#include "buttons.h"

void Button_recieveClick(Button_struct_t * Button, Click_enum_t * pClick)
{
	*pClick = Button->CurrentState;
	Button->CurrentState=FREE;
}
/**
 * @param pointer to button structure which should be checked
 */
void Button_checkButton(Button_struct_t * Button)
{
	/* button is released - we should do nothing if it happened after longclick. And if it was a click
	 * we should report it.	Think of button as a one-element queue. Counter variable represents how long
	 *  the button is pressed - it is only cleared when the button is released*/
	if (Button->CurrentState==FREE){
		if (((*Button->Port)&Button->Mask)==0) {  //if button is pressed
			//if the button is not active at the moment increment counter by one, which is 10 ms

				if (Button->Counter==LONG_CLICK_DURATION) {//long click state is achieved
					Button->CurrentState=LONGCLICK;
					Button->Counter=-1;
				}
				if (Button->Counter!=-1){
					Button->Counter++;
				}
		}
		else {
			//First take a look what do we have in counter
			if(Button->Counter>=CLICK_DURATION) {
				Button->CurrentState=CLICK;
			}
			Button->Counter=0;//if button is released - clear counter
		}
	}
}
