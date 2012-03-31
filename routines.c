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
void indicator(unsigned int adc)
{
	if (adc<=2*57) blink(RED, SHORT);//running out
	else if (adc<=4*57) blink(ORANGE, SHORT);//half
		else if (adc<=6*57) blink(GREEN, SHORT);//enough
			else if (adc>8*57) blink(GREEN, LONG);// more than enough
}
void set_brightness(unsigned short mode)
{
	char pwm [] = {0, 7, 8,16,32,64,128,255};
				// 1  2  3  4  5  6  7

	if (mode==0) PORTB&=~(1<<PB0);//!SHDN enable the device
	else
	{	
		OCR0B = pwm[mode];
		PORTB|=(1<<PB0);//!SHDN enable the device
	}
}
void init_mpu(void)
{
	DDRB|=(1<<PB0);//!SHDN
	
	DDRA|=(1<<PA7);//ctrl output
	
	DDRA|=(1<<PA2)|(1<<PA3);// LEDs
	PORTB|=(1<<PB2);//button0 pullup
	PORTA|=(1<<PA0);//button1 pullup
//power saving
	DIDR0&=0x01;//voltage adc pin
//start PWM	
	TCCR0A|=(1<<COM0B1);//OC0B non-inverting mode OCR0B = 0xFF goes for continious high, which means ON
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

