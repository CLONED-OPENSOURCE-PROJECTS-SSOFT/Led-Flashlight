#include "avr_compiler.h"
#include "adc.h"

unsigned int adc[]={0,0,0,0,0,0,0,0};//in adc.c

ISR(ADC_vect)
{
	//this one takes place ONLY after call adc_enable(char);
	adc[ADMUX]=ADC;//put result in corresponding register
	adc_disable();//disable ADC to save power
}

void adc_enable(unsigned char admux)
{
	ADCSRA|=(1<<ADEN);//enable ADC
	ADCSRA|=(1<<ADIE);//enable interrupts from ADC
	ADMUX=admux;
	_delay_ms(1);
	ADCSRA|=(1<<ADSC);//start conversion in BACKGROUND mode
}

void adc_disable(void)
{
	ADCSRA&=~(1<<ADEN);//disable ADC
	ADCSRA&=~(1<<ADIE);//disable interrpt from ADC
}


