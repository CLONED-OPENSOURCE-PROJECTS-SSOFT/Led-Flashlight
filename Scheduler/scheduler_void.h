#ifndef SCHEDULER_H
#define SCHEDULER_H

/* Errors not used yet
#define TaskSetOk			 'A'
#define TaskQueueOverflow	 'B'
#define TimerUpdated		 'C'
#define TimerSetOk			 'D'
#define TimerOverflow		 'E' */

/* Scheduler Configuration */
#define TASK_ARRAY_SIZE	12


#define FROM_ISR 1
#define NOT_FROM_ISR 0
typedef void (*pTask)(void);

typedef struct Timer_struct_defenition
{
	pTask Task; // Pointer
	uint16_t Time; // Time before execution
}Timer_struct_t;

void initScheduler(void);
void Task_create(pTask pNewTask, uint16_t NewTime,uint8_t FromISR);
void TaskScheduler(void);
void doTimerService(void);
void configureTickTimer (void);
#endif
