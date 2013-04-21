#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_

#define OFF	0
#define MAX_BRIGHTNESS 6

#define ADMUX_ADC0 0x00
#define ADMUX_ADC1 0x01
#define ADMUX_ADC2 0x02
#define ADMUX_ADC3 0x03
#define ADMUX_ADC4 0x04
#define ADMUX_ADC5 0x05
#define ADMUX_ADC6 0x06
#define ADMUX_ADC7 0x07
#define ADMUX_ADC8 0x08
#define ADMUX_ADC9 0x09
#define ADMUX_ADC10 0x0A

/* LED PART */
#define PORTLED PORTB

#include "avr_compiler.h"
#include "Handler.h"

typedef enum {
	BOARD_LED_gm = (1<<PB0)|(1<<PB1)|(1<<PB2),
	BOARD_LED_RED = (1<<PA0),
	BOARD_LED_GREEN = (1<<PB2),
    BOARD_LED_BLUE = (1<<PB1),
	BOARD_LED_WHITE = (1<<PB0)|(1<<PB1)|(1<<PB2),
} Board_LED_Color_t;

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
void Brightness_set(uint8_t mode);
void StateMachineTask (Handler *handler);
void ButtonCheckerTask(void);
void initPorts();

#endif /* FLASHLIGHT_H_ */
