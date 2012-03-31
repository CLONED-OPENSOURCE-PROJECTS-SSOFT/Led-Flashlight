#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "routines.h"
#include "led.h"
void indicator(unsigned short adc)
{
	adc/=28;
	if (adc<=9) blink(RED, PULSE);//no power!
	else if (adc<=10) blink(RED, SHORT);//running out
	else if (adc<=12) blink(ORANGE, SHORT);//half
	else if (adc<=14) blink(GREEN, SHORT);//enough
	else if (adc<=30) blink(GREEN, LONG);// more than enough
	else if (adc>30) blink(RED, GLOW);//overpower
}
void set_brightness(unsigned short mode)
{
	char pwm [] = {255,251, 247,239,223,195,127,0};
				// 0 1  2  3  4  5  6  7
	OCR0B = pwm[mode];
}
void init_mpu(void)
{
	DDRA|=(1<<PA7);//MCE
	PORTA|=(1<<PA7);//PB3 mce
	DDRA|=(1<<PA1)|(1<<PA5);// LEDs
	PORTB|=(1<<PB2);//button0 pullup
	PORTA|=(1<<PA0);//button1 pullup
//power saving
	DIDR0&=0x00;
//start PWM	
	TCCR0A|=(1<<COM0B1);//OC0B non-inverting mode OCR0B = 0xFF goes for continious high, which means off
	TCCR0A|=(1<<WGM00)|(1<<WGM01);//PWM with top = 0xFF
	TCCR0B|=(1<<CS00);//no prescaler for high frequency
	set_brightness(0);
}
void start_timer1_ctc(char mode,unsigned int value)
{
	TCCR1A&=0x00;//stop counter, clear all control bits
	TCCR1B&=0x00;//stop counter, clear all control bits
	TCNT1 = 0x0000;//clear counter
	TCCR1B|=(1<<WGM12);//CTC mode clear on compare
	TIMSK1 |= (1<<OCIE1A);//enable compa_vect
	unsigned long tacts;
	if (mode==PERIOD) tacts=(F_CPU/1000)*value; else tacts=F_CPU/value;
	if (tacts<0xFFFF) 
	{
		OCR1A=tacts;
		TCCR1B|=TIMER1_PREDIVISOR_1;
		return;
	}
	if (tacts/8<0xFFFF) 
	{
		OCR1A=tacts/8;
		TCCR1B|=TIMER1_PREDIVISOR_8;
		return;
	}
	if (tacts/64<0xFFFF) 
	{
		OCR1A=tacts/64;
		TCCR1B|=TIMER1_PREDIVISOR_64;
		return;
	}
	if (tacts/256<0xFFFF) 
	{
		OCR1A=tacts/256;
		TCCR1B|=TIMER1_PREDIVISOR_256;
		return;
	}
	if (tacts/1024<0xFFFF) 
	{
		OCR1A=tacts/1024;
		TCCR1B|=TIMER1_PREDIVISOR_1024;
		return;
	}
}

