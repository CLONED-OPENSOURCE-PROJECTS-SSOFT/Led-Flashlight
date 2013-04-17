#include "avr_compiler.h"
#include <avr/sleep.h>
#include "adc.h"
#include "led.h"
#include "flashlight.h"
//------------GLOBALS---------------------------
short mode=0;//this global used as index of power array
volatile unsigned short status=INDICATE;
int tick=0;
int adcTime=0;
unsigned int button[]={0,0};//time counters
//----------------------------------------------
ISR(TIM1_COMPA_vect)
{	
	
	if(adcTime==tick)
		{
			adcTime=tick+1000;//every 1 second 
			adc_enable(ADMUX_ADC2);//check battary
			//status|=INDICATE;//debug
		}
/*===========================NEW CODE PLEASE=====THIS ONE SUCKSSSS=================*/
//-----------PLUS BUTTON-------------------------------------
	if ((PINA&(1<<PA0))==0)
	{
		button[0]++;
		if (button[0]==40) status|=INDICATE;
		if (mode==0)
			{
				if (button[0]==WAKE_DELAY) mode=HIGH_AV_BR;
			}	
			else if (button[0]==SHUTDOWN_DELAY) mode=0;
			
	}
	else
	{
		if (mode!=0&&button[0]>10&&button[0]<SHUTDOWN_DELAY&&mode<MAX_BRIGHTNESS) mode++;//if time is within make action
		button[0]=0;//if button is not pressed clear counter
	}
//------------MINUS BUTTON-----------------------------------
	if ((PINB&(1<<PB2))==0)
	{
		button[1]++;
		if (button[1]==40) {status|=INDICATE;}
		if (mode==0)
		{
			if (button[1]==WAKE_DELAY) mode=LOW_AV_BR;//HEY IT IS to be redone
		}
		else if (button[1]==SHUTDOWN_DELAY) mode=0;
	}
	else
	{
		if (mode!=0&&button[1]>10&&button[1]<SHUTDOWN_DELAY&&mode>1) mode--;//if time is within make action
		button[1]=0;//if button is not pressed clear counter
	}
/*===========================NEW CODE PLEASE=====THIS ONE SUCKSSSS=================*/	
	
		
	set_brightness(mode);//anyway, update mode
	tick++;//increment tick
}


int main(void)
{
	init_mpu();//init
	sei();//enable interrupts
	set_sleep_mode(SLEEP_MODE_IDLE);//IDLE mode to save power
	start_timer1_ctc(1000);//assuming 1000 Hz is enough for event handling
	status=INDICATE;
	for(;;) 
	{
		//this is a simple yeild from tick function. 
		if (status==INDICATE) {indicator(adc[ADMUX_ADC1]);status&=~INDICATE;}//indicate
        sleep_enable();
        sleep_cpu();
        sleep_disable();
	}
	return 0;
}//main

void init_mpu(void)
{
	DDRB|=(1<<PB0);//!SHDN
	DDRA|=(1<<PA7);//ctrl output
	
	DDRA|=(1<<PA2)|(1<<PA3);// LEDs
	PORTB|=(1<<PB2);//button0 pullup
	PORTA|=(1<<PA0);//button1 pullup
	PORTA|=(1<<PA4);//button1 pullup
	PORTA|=(1<<PA6);//button1 pullup
//power saving
	DIDR0&=0x01;//voltage adc pin
//start PWM	
	TCCR0A|=(1<<COM0B1);//OC0B non-inverting mode OCR0B = 0xFF goes for continious high, which means ON
	TCCR0A|=(1<<WGM00)|(1<<WGM01);//PWM with top = 0xFF
	TCCR0B|=(1<<CS00);//no prescaler for high frequency
	set_brightness(0);
}
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

char start_timer1_ctc(unsigned int frequency)
{
	TCCR1A&=0x00;//stop counter, clear all control bits
	TCCR1B&=0x00;//stop counter, clear all control bits
	TCNT1 = 0x0000;//clear counter
	TCCR1B|=(1<<WGM12);//CTC mode clear on compare
	TIMSK1 |= (1<<OCIE1A);//enable compa_vect
	unsigned long tacts = F_CPU/frequency;
	if (tacts<0xFFFF) 
	{
		OCR1A=tacts;
		TCCR1B|=TIMER1_PREDIVISOR_1;
		return 0;//no error
	}
		return 1;//returns error
}
