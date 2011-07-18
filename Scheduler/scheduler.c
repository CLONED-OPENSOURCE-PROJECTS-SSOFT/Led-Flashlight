#include "avr_compiler.h"
#include "scheduler.h"
#include "port.h"


/* Statically allocate memory for task queue and array of timers */
volatile static Timer_struct_t TaskArray[TASK_ARRAY_SIZE];

/* Scheduler initialization - set up initial values to queues */
inline void initScheduler(void)
{
	for(uint8_t i=0;i<TASK_ARRAY_SIZE;i++)
	{
		TaskArray[i].Task = doIdleTask;
		TaskArray[i].Time = 0;
	}
	configureTickTimer();// Call to function from port.c.
}

/* If time to execution is more than zero - decrease by one. */
inline void doTimerService(void)
{
	for(uint8_t i=0;i<TASK_ARRAY_SIZE;i++)//For the whole array
	{
		if(TaskArray[i].Time>0) TaskArray[i].Time--;// Decrement time is non-zero.
	}
}

/* Put new task to queue. Parameter - pointer to new task.
 * Example: if task is declared is function doSomeTask() then pass doSomeTask as parameter.
 */
void Task_create(pTask pNewTask, uint16_t NewTime,uint8_t FromISR)
{
	if (!FromISR)cli();
	for(uint8_t i=0;i<TASK_ARRAY_SIZE;++i)//For the whole array.
	{
		if (TaskArray[i].Task == doIdleTask)
			{
				TaskArray[i].Task = pNewTask;// Set task active - set pointer.
				TaskArray[i].Time = NewTime;// Set time until execution.
				if (!FromISR)sei();	//Enable interrupts
				return;
			}
	}

}

/* Task scheduler.*/
inline void TaskScheduler(void)
{
	cli();//Disable interrupts
	pTask	GoToTask = doIdleTask;// Assume that only idle task is present.
	for(uint8_t i=0;i<TASK_ARRAY_SIZE;i++)//For the whole array.
	{
		if (TaskArray[i].Task!=doIdleTask)//Look for active tasks.
			if(TaskArray[i].Time==0)//If active task is up to execute.
			{
				GoToTask=TaskArray[i].Task;//Put pointer to that task into variable.
				TaskArray[i].Task=doIdleTask;//Remove pointer from array (task is no more active).
				break;
			}
	}
	sei();//Enable interrupts
	(GoToTask)();//Go to the task. If there weren't any tasks except idle, then idle is executed.
}



