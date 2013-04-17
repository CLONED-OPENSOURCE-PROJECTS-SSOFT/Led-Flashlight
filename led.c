#include "avr_compiler.h"
#include "led.h"

void led(uint8_t color)
{
	switch (color)
	{
	case GREEN://green
		PORTGREEN|=(1<<PGREEN);
		PORTRED&=~(1<<PRED);
	break;
	case RED://red
		PORTGREEN&=~(1<<PGREEN);
		PORTRED|=(1<<PRED);
	break;
	case ORANGE://orange
		PORTGREEN|=(1<<PGREEN);
		PORTRED|=(1<<PRED);//two leds shining
	break;
	default://none
		PORTGREEN&=~(1<<PGREEN);
		PORTRED&=~(1<<PRED);//no leds
	break;
	}
}


void blink (uint8_t color,uint8_t type)
{
	switch (type)
	{
		case SHORT:
			led(color);
			_delay_ms(50);
			led(NONE);
			_delay_ms(50);
			led(color);
			_delay_ms(100);
			led(NONE);
		break;
  		case LONG:
			led(color);
			_delay_ms(150);
			led(NONE);
			_delay_ms(150);
			led(color);
			_delay_ms(150);
			led(NONE);
		break;
  		case PULSE:
			for (uint8_t i=0;i<=5;i++)
			{
				led(color);
				_delay_ms(50);
				led(NONE);
				_delay_ms(50);
			}
		break;
		default:  //none delayed
			led(NONE);
			_delay_ms(200);
		break;
	}//end switch
}
