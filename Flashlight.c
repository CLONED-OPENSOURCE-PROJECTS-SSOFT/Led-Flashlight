#include "avr_compiler.h"
#include "flashlight.h"
#include "scheduler.h"

inline void ADC_enable(unsigned char admux)
{
	ADCSRA|=(1<<ADEN);//enable ADC
	ADCSRA|=(1<<ADIE);//enable interrupts from ADC
	ADMUX=admux;
	_delay_ms(1);
	ADCSRA|=(1<<ADSC);//start conversion in BACKGROUND mode
}

void Brightness_set (uint8_t mode)
{
	uint8_t pwm=0;
	switch (mode)
	{
		case 0:
			pwm = 0;
			PORTB&=~(1<<PB0);//!SHDN enable the device
			TCCR0A&=~(1<<WGM00)|(1<<WGM01);//Stop PWM to save power
			break;
		case 1: pwm = 10; break;
		case 2: pwm = 15; break;
		case 3: pwm = 31; break;
		case 4: pwm = 63; break;
		case 5: pwm = 127; break;
		case 6: pwm = 255; break;
		default: pwm = 0;
	}
	if (pwm!=0)
	{
		OCR0B = pwm;//Set mode
		TCCR0A|=(1<<WGM00)|(1<<WGM01);//Start PWM Timer
		PORTB|=(1<<PB0);//!SHDN enable the device and mosfetS
	}
}

inline void LED_setNone(void){PORTLED&=~BOARD_LED_gm;}
inline void LED_toggleOrange(void){PORTLED^=BOARD_LED_ORANGE;}
inline void LED_toggleGreen(void){PORTLED^=BOARD_LED_GREEN;}
inline void LED_toggleRed(void){PORTLED^=BOARD_LED_RED;}

