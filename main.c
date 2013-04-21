#include "avr_compiler.h"
#include "main.h"
#include "Handler.h"

Button_struct_t ButtonMinus;
Button_struct_t ButtonPlus;
Message msgArray[QUEUE_MAX_LEN];

typedef enum {
	CHECK_BUTTONS = 1, RUN_SM_TASK = 2, SET_OFF = 3, HEARTBEAT = 4,
} Message_code_t;

ISR(TIM0_COMPA_vect) {
	tick++;
}

void handleMessage(Message msg, void *context, Handler *handler) {
	switch (msg.what) {
	case CHECK_BUTTONS:
		Button_checkButton(&ButtonMinus);
		Button_checkButton(&ButtonPlus);
		Message *buttonMessage = Handler_obtain(handler, CHECK_BUTTONS);
		Handler_sendMessageDelayed(handler, buttonMessage, BUTTON_CHECK_PERIOD);
		break;
	case SET_OFF:
		LED_off(msg.arg1);
		break;
	case HEARTBEAT:
		LED_toggle(BOARD_LED_BLUE);
		Message *message = Handler_obtain(handler, HEARTBEAT);
		Handler_sendMessageDelayed(handler, message, 1000);
		break;
	case RUN_SM_TASK:
		StateMachineTask(handler);
		Message *smMessage = Handler_obtain(handler, RUN_SM_TASK);
		Handler_sendMessageDelayed(handler, smMessage, 100);
		break;
	}
}

int main(void) {
	initPorts();
	configureTickTimer();
	MsgQueue queue;
	MsgQueue_init(&queue, msgArray, 10);
	Handler handler;
	Handler_init(&handler, &queue, handleMessage, 0);

	Handler_sendEmptyMessage(&handler, CHECK_BUTTONS);
	Handler_sendEmptyMessage(&handler, RUN_SM_TASK);
	Handler_sendEmptyMessage(&handler, HEARTBEAT);

	/* Enable interrupts */sei();
	/* Enter the superloop which manages tasks */
	while (1) {
		MsgQueue_processNextMessage(&queue);
	}
	return 0;	//Should never get here
}

void Button_recieveClick(Button_struct_t * Button, Click_enum_t * pClick) {
	*pClick = Button->CurrentState;
	Button->CurrentState = FREE;
}
/**
 * @param pointer to button structure which should be checked
 */
void Button_checkButton(Button_struct_t * Button) {
	/* button is released - we should do nothing if it happened after longclick. And if it was a click
	 * we should report it.	Think of button as a one-element queue. Counter variable represents how long
	 *  the button is pressed - it is only cleared when the button is released*/
	if (Button->CurrentState == FREE) {
		if (((*Button->Port) & Button->Mask) == 0) {  //if button is pressed
			//if the button is not active at the moment increment counter by one, which is 10 ms

			if (Button->Counter == LONG_CLICK_DURATION) {//long click state is achieved
				Button->CurrentState = LONGCLICK;
				Button->Counter = -1;
			}
			if (Button->Counter != -1) {
				Button->Counter++;
			}
		} else {
			//First take a look what do we have in counter
			if (Button->Counter >= CLICK_DURATION) {
				Button->CurrentState = CLICK;
			}
			Button->Counter = 0;		//if button is released - clear counter
		}
	}
}

inline void configureTickTimer(void) {

	TCCR0A &= 0x00;			//stop counter, clear all control bits
	TCCR0B &= 0x00;			//stop counter, clear all control bits
	TCNT0 = 0x0000;			//clear counter
	TCCR0B |= (1 << WGM02);			//CTC mode clear on compare
	TIMSK0 |= (1 << OCIE0A);			//enable compa_vect
#if (F_CPU/TICK_FREQUENCY>0xFFFF)
#error "hey thats too slow!"
#endif
	OCR0A = (F_CPU / TICK_FREQUENCY);
	TCCR0B |= TIMER1_PREDIVISOR_1;
	tick = 0;
}

void Brightness_set(uint8_t mode) {
	uint8_t pwm = 0;
	switch (mode) {
	case 0:
		pwm = 0;
		PORTA &= ~(1 << PA1);			//!SHDN enable the device
		TCCR1A &= ~(1 << WGM10) | (1 << WGM11);			//Stop PWM to save power
		break;
	case 1:
		pwm = 10;
		break;
	case 2:
		pwm = 15;
		break;
	case 3:
		pwm = 31;
		break;
	case 4:
		pwm = 63;
		break;
	case 5:
		pwm = 127;
		break;
	case 6:
		pwm = 255;
		break;
	default:
		pwm = 0;
		break;
	}
	if (pwm != 0) {
		OCR1B = pwm;			//Set mode
		TCCR1A |= (1 << WGM10) | (1 << WGM11);			//Start PWM Timer
		PORTA |= (1 << PA1);			//!SHDN enable the device and mosfetS
	}
}

void LED_on(char color) {
	PORTLED &= ~color;
}

void LED_off(char color) {
	PORTLED |= color;
}

inline void LED_toggle(char color) {
	PORTLED ^= color;
}

/* Define tasks here. */
inline void initPorts() {
	DDRA |= (1 << PA1);			//!SHDN
	DDRA |= (1 << PA7);			//ctrl output
	PORTA |= (1 << PA7);			//ctrl always eneable
	DDRB |= BOARD_LED_gm;			// LEDs
	LED_off(BOARD_LED_WHITE);

	DIDR0 &= 0x01;			//voltage adc pin

	PORTA |= (1 << PA4);			//button0 pullup
	PORTA |= (1 << PA2);			//button1 pullup

	ButtonPlus.Counter = 0;
	ButtonPlus.CurrentState = FREE;
	ButtonPlus.Port = &PINA;			//
	ButtonPlus.Mask = (1 << PA2);

	ButtonMinus.Counter = 0;
	ButtonMinus.CurrentState = FREE;
	ButtonMinus.Port = &PINA;
	ButtonMinus.Mask = (1 << PA4);

	//start PWM
	TCCR1A |= (1 << COM1B1);//OC0B non-inverting mode OCR0B OCR0B=0xFF goes for continious high, OCR0B=0x00 = OFF
	TCCR1B |= (1 << CS10);			//no prescaler for high frequency
	DDRA |= (1 << PA5);			//PWM
	Brightness_set(0);
}

void StateMachineTask(Handler *handler) {
	static uint8_t state = 0;			//initial state

	Click_enum_t MinusClick = FREE, PlusClick = FREE;

	Button_recieveClick(&ButtonMinus, &MinusClick);
	Button_recieveClick(&ButtonPlus, &PlusClick);

	if (PlusClick == LONGCLICK || MinusClick == LONGCLICK) {
		LED_toggle(BOARD_LED_RED);
		Message *message = Handler_obtain(handler, SET_OFF);
		message->arg1 = BOARD_LED_RED;
		Handler_sendMessageDelayed(handler, message, 198);
	}
	if (MinusClick == CLICK || PlusClick == CLICK) {
		LED_toggle(BOARD_LED_GREEN);
		Message *message = Handler_obtain(handler, SET_OFF);
		message->arg1 = BOARD_LED_GREEN;
		Handler_sendMessageDelayed(handler, message, 198);
	}

	switch (state) {
	case 0:
		if (MinusClick == LONGCLICK) {
			state = 3;
		} else if (PlusClick == LONGCLICK) {
			state = 4;
		}
		break;
	case 1:
		if ((MinusClick == LONGCLICK) || (PlusClick == LONGCLICK)) {
			state = 0;
		}
		if (PlusClick == CLICK) {
			state++;
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		if ((MinusClick == LONGCLICK) || (PlusClick == LONGCLICK)) {
			state = 0;
		}
		if (PlusClick == CLICK) {
			state++;
			//TODO Task_create(state--,10000,NOT_FROM_ISR)
		}
		if (MinusClick == CLICK) {
			state--;
		}
		break;
	case 6:			//TURBO
		if ((MinusClick == LONGCLICK) || (PlusClick == LONGCLICK)) {
			state = 0;
		}
		if (MinusClick == CLICK) {
			state--;
		}
		break;
	}
	Brightness_set(state);
}
