#include "avr_compiler.h"
#include "flashlight.h"
#include "scheduler.h"

void Brightness_set (uint8_t mode)
{
	uint8_t pwm=0;
	switch (mode)
	{
		case 0:
			pwm = 0;
			TCCR0A&=~(1<<WGM00)|(1<<WGM01);//Stop PWM to save power
			TCCR0A&=~(1<<COM0B1)|(1<<COM0B0);//OC0B non-inverting mode OCR0B OCR0B=0xFF goes for continious high, OCR0B=0x00 = OFF
			TCCR0B&=~(1<<CS01);//prescaler for high frequency

			PORTA|=(1<<PA7);
			break;
		case 1: pwm = 8; break;
		case 2: pwm = 16; break;
		case 3: pwm = 32; break;
		case 4: pwm = 64; break;
		case 5: pwm = 128; break;
		case 6: pwm = 255; break;
		default: pwm = 0;
	}
	if (pwm!=0)
	{
		TCCR0A|=(1<<COM0B1)|(1<<COM0B0);//OC0B non-inverting mode OCR0B OCR0B=0xFF goes for continious high, OCR0B=0x00 = OFF
		TCCR0B|=(1<<CS01);// prescaler for high frequency
		OCR0B = pwm;//Set mode
		TCCR0A|=(1<<WGM00)|(1<<WGM01);//Start PWM Timer
	}
}

inline void LED_setNone(void){PORTLED&=~BOARD_LED_gm;}
inline void LED_toggleOrange(void){PORTLED^=BOARD_LED_ORANGE;}
inline void LED_toggleGreen(void){PORTLED^=BOARD_LED_GREEN;}
inline void LED_toggleRed(void){PORTLED^=BOARD_LED_RED;}

