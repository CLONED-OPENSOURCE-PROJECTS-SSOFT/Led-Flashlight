#include "avr_compiler.h"
#include "scheduler.h"
#include "tasks.h"
#include "flashlight.h"

ISR(TIM1_COMPA_vect)
{
	doTimerService();//Decrement time to execution for active tasks
}

int main(void)
{
	initScheduler();
	initPorts();
	Task_create(ButtonCheckerTask,0,NOT_FROM_ISR);//Create task and execute immediately (time =0)
	Task_create(StateMachineTask,0,NOT_FROM_ISR);


	/* Enable interrupts */
	sei();
	/* Enter the superloop which manages tasks */
	while(1)
	{
		TaskScheduler();	//Run active or idle tasks
	}
	return 0;//Should never get here
}



