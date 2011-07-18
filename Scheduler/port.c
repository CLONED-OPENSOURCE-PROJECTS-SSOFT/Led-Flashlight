/*
 * port.c
 *
 *  Created on: 17.01.2011
 *      Author: Yuriy
 */
#include "avr_compiler.h"
#include "port.h"

inline void configureTickTimer (void)
{

	TCCR1A&=0x00;//stop counter, clear all control bits
	TCCR1B&=0x00;//stop counter, clear all control bits
	TCNT1 = 0x0000;//clear counter
	TCCR1B|=(1<<WGM12);//CTC mode clear on compare
	TIMSK1 |= (1<<OCIE1A);//enable compa_vect
	#if (F_CPU/TICK_FREQUENCY>0xFFFF)
		#error "hey thats too slow!"
	#endif
	OCR1A=(F_CPU/TICK_FREQUENCY);
	TCCR1B|=TIMER1_PREDIVISOR_1;
}
/* Idle task - use for power saving or debug output */
inline void  doIdleTask(void)
{
	//sleep_mode();
}
