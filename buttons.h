/*
 * buttons.h
 *
 *  Created on: 19.03.2011
 *      Author: Yuriy
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON_CHECK_PERIOD 10
/* In 10ms units */
#define CLICK_DURATION 5
#define LONG_CLICK_DURATION 75

#if (CLICK_DURATION>0xFF)
	#error "hey thats too much!"
#endif
#if (LONG_CLICK_DURATION>0xFF)
	#error "hey thats too much!"
#endif

typedef enum {
	CLICK = 0x01,
	LONGCLICK = 0x02,
	FREE = 0x00,
} Click_enum_t;

typedef struct Button_struct_defenition
{
	Click_enum_t CurrentState;
	int8_t Counter;
	char * Port;
	uint8_t Mask;
} Button_struct_t;

void Button_checkButton(Button_struct_t * Button);
void Button_recieveClick(Button_struct_t * Button, Click_enum_t * pClick);

#endif /* BUTTONS_H_ */
