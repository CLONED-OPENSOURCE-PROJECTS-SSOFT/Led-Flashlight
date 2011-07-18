/*
 * ADC.c
 *
 *  Created on: 02.02.2011
 *      Author: Yuriy
 */
#include "avr_compiler.h"
#include "flashlight.h"
#include "scheduler.h"
unsigned int ADC_Battery=0;
unsigned int ADC_Temperature=0;
ISR(ADC_vect)
{
	//this one takes place ONLY after call adc_enable(char);
	switch (ADMUX)//put result in corresponding register
	{
		case 1: ADC_Temperature=ADC;
		case 0: ADC_Battery=ADC;
	}
	ADCSRA&=~(1<<ADEN);//disable ADC
	ADCSRA&=~(1<<ADIE);//disable interrpt from ADC
}
void ADC_checkBatteryTask (void)
{
	ADC_enable(ADMUX_ADC0);//
	Task_create(ADC_checkBatteryTask,1000,NOT_FROM_ISR);
}
void ADC_checkTemperatureTask (void)
{
	ADC_enable(ADMUX_ADC1);//
	Task_create(ADC_checkTemperatureTask,100,NOT_FROM_ISR);
}
