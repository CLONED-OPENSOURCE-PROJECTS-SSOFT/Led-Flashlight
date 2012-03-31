#include "adc.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

void adc_enable(short admux)
{
	ADMUX|=admux;
	ADCSR|=(1<<ADEN);
	ADCSR|=(1<<ADIE);
	ADCSR|=(1<<ADSC);
}
void adc_disable(void)
{
	ADCSR&=~(1<<ADEN);
	ADCSR&=~(1<<ADIE);
}
