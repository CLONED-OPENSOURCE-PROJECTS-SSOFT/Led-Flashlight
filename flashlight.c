#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "adc.h"
#include "led.h"
#include "flashlight.h"
#include "routines.h"
//------------GLOBALS---------------------------
short mode=0;//this global used as index of power array
volatile unsigned short status=INDICATE;
int counter[]={0,0};
unsigned int button[]={0,0};//time counters
//----------------------------------------------
ISR(TIM1_COMPA_vect)
{	
	
	if(counter[0]++==1000)//every 1 second 
		{
			counter[0]=0;//resart timer
			adc_enable(ADMUX_ADC2,BACKGROUND);//check battary
			//status|=INDICATE;
		}

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
	set_brightness(mode);
}


int main(void)
{
	init_mpu();
	sei();
	set_sleep_mode(SLEEP_MODE_IDLE);
	adc_enable(ADMUX_ADC2,SLEEP);
	start_timer1_ctc(FREQUENCY,1000);
	for(;;) 
	{
		if (status==INDICATE) {indicator(adc[ADMUX_ADC2]);status&=~INDICATE;}//indicate
        sleep_enable();
        sleep_cpu();
        sleep_disable();
	}
	return 0;
}//main

