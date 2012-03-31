#include "adc.h" 
/*
ISR(INT0_vect)
{
	blink(RED,PULSE);
	reset_mpu();
}
void shutdown (void)
{
	DDRA=0x00;//input for INT0
	PORTA=0xFF;//pullups
//	stop_timers();
//	adc_disable();
	_delay_ms(1);
	GIMSK|=(1<<INT0);//enable INT0 interrupt
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_cpu();
}
void reset_mpu (void)
{
wdt_enable(WDTO_15MS);
for(;;);
}
*/
