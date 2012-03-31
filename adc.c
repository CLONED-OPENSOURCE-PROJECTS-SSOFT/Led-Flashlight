#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "adc.h"

unsigned int adc[]={0,0,0,0,0,0,0,0};//in adc.c

ISR(ADC_vect)
{
	//this one takes place ONLY after call adc_enable(char);
	adc[ADMUX]=ADC;
	adc_disable();//disable ADC to save power
}

void adc_enable(unsigned char admux, char mode)
{
	//add some if to check if adc is on to avoid waiting
	ADCSRA|=(1<<ADEN);//enable ADC
	ADCSRA|=(1<<ADIE);//enable interrupts from ADC
	ADMUX=admux;
	_delay_ms(1);
	if (mode==WAIT)
	{
		ADCSRA|=(1<<ADSC);//
		_delay_us(14);//wait until convertion is completed
	}
	else 
		if (mode==SLEEP)
		{
			char mcucr_temp=MCUCR;//store MCUCR with sleep mode settings
			set_sleep_mode(SLEEP_MODE_ADC);//set noise reduction mode
			sleep_enable();
	        sleep_cpu();//start convertion and wake from interrupt
	        MCUCR=mcucr_temp;//restore MCUCR
		}
		else ADCSRA|=(1<<ADSC);//start conversion in BACKGROUND mode
}

void adc_disable(void)
{
	ADCSRA&=~(1<<ADEN);//disable ADC
	ADCSRA&=~(1<<ADIE);//disable interrpt from ADC
}


