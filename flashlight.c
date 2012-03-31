#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "adc.h"
#include "led.h"
#include "flashlight.h"
#include "routines.h"
//------------GLOBALS---------------------------
short mode=0;//this global used as index of power array
short offset=0;//offsets mode by 2 if in highpower
int ADCcounter=0;//this is for ADC to work every 5 seconds
int idlecounter=0;
//----------------------------------------------
void wait_ms(uint16_t ms){
	counter=0;
	TCNT1 = 0;
	OCR1A = 0x08EE;				//FCPU:8:2286 = 1 kHz mit FCPU=1MHz
	OCR1A = 0x07D0;
	TIMSK1 |= (1<<OCIE1A);
	//TCCR1A |= (1<<CS11) | (1<<WGM11);	//F_CPU:8
	TCCR1B |= (1<<WGM12) | (1<<CS11);
	sei();
	while(counter<ms);
	//cli();
	//TCCR1A &= ~(1<<CS11) & ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12) & ~(1<<CS11);
	TIMSK1 &= ~(1<<OCIE1A);
	return;
}

void start_timer1(void){
	counter2=0;
	TCNT2 = 0x00;
	TIMSK2 |= (1<<TOIE2);
	TCCR2B |= (1<<CS22) | (1<<CS21)| (1<<CS20);	// = FCPU/1024 ca. 0,0142 s bei 1MHz
	sei();
	return;
}

void stop_timer1(void){
	TCCR2B &= ~(1<<CS22) & ~(1<<CS21) & ~(1<<CS20);
	return;
}

void button_latch (void)
{
	if (mode==OFF)
	{
		mode=WAKE;
		
		for (int ms=0;((PINB&0x40)==0)||((PINA&0x08)==0);ms++)
		{
			if (ms>WAKE_POWER_LATCH)
			{
				start_timer1_pwm(0x64);
				_delay_ms(100);
				start_timer1_pwm(0x32);
				_delay_ms(100);
				offset=2;
			}
			_delay_ms(1);
		}
	}
	_delay_ms(1);
	if (mode!=OFF)
		for (int ms=0;((PINB&0x40)==0)||((PINA&0x08)==0);ms++)
		{
			if (ms==SHUTDOWN_LATCH)
			{ 
				start_timer1_pwm(0x00);
				mode=OFF;
				offset=0;
			}
			_delay_ms(1);
		}

}
ISR(TIMER0_OVF0_vect)
{	
	if(ADCcounter++==5000&&mode==OFF)
	{
		adc_enable(ADMUX_ADC6);
		ADCcounter=0;
		if(idlecounter++==5); 
	}
//--------------------------------------------------------------
	unsigned short pwr [] = {0,8,16,32,64,128,255};
						  // 0 1  2  3  4  5   6
	if((PINB&0x40)==0)//goes here ONLY if button0 JUST pressed
	{
		button_latch();//waits UNTIL you release button
		if(mode!=OFF)
		{
			if (mode==1) mode=2;//if in dark mode change brightness
				else mode=1;//else just switch to dark mode
			start_timer1_pwm(pwr[mode+offset]); 
		}
	}
	if((PINA&0x08)==0)//goes here ONLY if button1 JUST pressed
	{
		button_latch();//waits UNTIL you release button
		if(mode!=OFF)
		{
			if (mode==3) mode=4;//if in bright mode change brightness
				else mode=3;//else just switch to bright mode
			start_timer1_pwm(pwr[mode+offset]); 
		}
	}
	timer0_ovf_corrector();
}
ISR(ADC_vect)
{
	int adc;
	adc=ADC/28;//now we have amount of V
	if (adc>0&&adc<=9) blink(RED, PULSE);//no power!
	if (adc>9&&adc<=10) blink(RED, SHORT);//running out
	if (adc>10&&adc<=12) blink(ORANGE, SHORT);//half
	if (adc>12&&adc<=14) blink(GREEN, SHORT);//enough
	if (adc>14&&adc<=30) blink(GREEN, LONG);// more than enough
	if (adc>30) blink(RED, GLOW);//overpower
	adc_disable();
}

int main(void)
{
	MCUSR = 0;//it is necessary to avoid continios reset after softwate reset
	wdt_disable();//it is necessary to avoid continios reset after softwate reset
	init_mpu();
	start_timer0(1000);
	sei();
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	for(;;) 
		{
		sleep_cpu();
		}
	return 0;
}//main

