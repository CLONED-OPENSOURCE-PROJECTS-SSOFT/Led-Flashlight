#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "routines.h"
//----GLOBALS-----------
unsigned short tcnt0_bottom;
//----GLOBALS-----------

void start_timer1_pwm(unsigned short pwm)
{
	TCCR1A|=0x31;//pb3 mode and pwm
	TCCR1B=0x05;//see table in datasheet
	OCR1C=199;
	OCR1B = pwm;
}
void init_mpu(void)
{
	DDRB|=(1<<PB3);//MCE
	PORTB|=(1<<PB3);//PB3 mce
	DDRA|=0x06;//GR LED
	PORTB|=(1<<PB6);//button0 pullup
	PORTA|=(1<<PA3);//button1 pullup
	PORTA|=(1<<PA4);//button2 pullup
}
void start_timer0(int frequency)
{	//returns bottom value for TCNT0
	TCNT0 = 0;
	TIMSK|=(1<<TOIE0);
	uint16_t tacts=F_CPU/frequency;
	if (tacts<255){TCCR0=CK; tcnt0_bottom=255-tacts;return;}
	if (tacts/8<255){TCCR0=CK8; tcnt0_bottom=255-tacts/8;return;}
	if (tacts/64<255){TCCR0=CK64; tcnt0_bottom=255-tacts/64;return;}
	if (tacts/256<255){TCCR0=CK256; tcnt0_bottom=255-tacts/256;return;}
	if (tacts/1024<255){TCCR0=CK1024; tcnt0_bottom=255-tacts/1024;return;}
}
void timer0_ovf_corrector(void)
{
	TCNT0=tcnt0_bottom;
}
