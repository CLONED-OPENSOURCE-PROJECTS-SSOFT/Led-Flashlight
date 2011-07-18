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
#define PORTLED PORTA

typedef enum {
	BOARD_LED_gm = 0x0C,
	BOARD_LED_RED = 0x08,
	BOARD_LED_GREEN = 0x04,
    BOARD_LED_ORANGE = 0x0C,
	BOARD_LED_NONE = 0x00,
} Board_LED_Color_t;
/*
typedef enum {
	BUTTON_LED_gm = 0x0C,
	BUTTON_LED_RED = 0x08,
	BUTTON_LED_GREEN = 0x04,
	BUTTON_LED_ORANGE = 0x0C,
	BUTTON_LED_NONE = 0x00,
} Button_LED_Color_t;
*/

void LED_setNone(void);
void LED_toggleOrange(void);
void LED_toggleGreen(void);
void LED_toggleRed(void);

void ADC_enable(unsigned char admux);

void Brightness_set(uint8_t mode);

#endif /* FLASHLIGHT_H_ */
