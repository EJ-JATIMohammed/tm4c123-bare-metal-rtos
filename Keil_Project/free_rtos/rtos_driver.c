#include "rtos_driver.h"
#include "SysTick_driver.h"
#include "tm4c123gh6pm.h"

#include <stddef.h>

//==============================================
//            rtos constants;
//==============================================
#define  MAX_TASK 				((uint32_t)	6)
#define  TASK_STACK_MAX 		((uint32_t)	128)


//==============================================
//            rtos global vraibles;
//==============================================

int     		current_index;
uint32_t 		task_count;
uint32_t 		task_stack[MAX_TASK][TASK_STACK_MAX];
task_handler_t 	task_ready[MAX_TASK];
task_handler_t 	*current_task;
task_handler_t 	*next_task;

uint32_t tick;

void rtos_init(void)
{
	/*	Reset global variables*/
	task_count 				= 0;
	current_index 			= -1;
	tick					= 0;
	current_task 			= NULL;
	next_task 				= NULL;
	
	task_handler_t task_init;
	task_init.sp 			= NULL;
	task_init.task_func 	= NULL;
	task_init.stack_size 	= TASK_STACK_MAX;
	task_init.state 		= DORMANT;
	
	for(uint32_t i = 0; i < MAX_TASK ; i++)
	{
		task_ready[i] 		= task_init;
	}
	
	/*	Configure Systick timer (eache tick is a 1 ms)*/
	sysTick_init(INTERRUPT_ENABLE, PIOSC_D_4, 4000);
	
}

void rtos_create_task(task_handler_t *task_tcb)
{
	/*	Alocate task stack */
	uint32_t *sp = &task_stack[task_count][task_tcb->stack_size - 1];
	
	/*	Hardware stack registers*/
	*(--sp) = 0x01000000; 						//xPSR
	*(--sp) = (uint32_t) task_tcb->task_func; 	//PC
	*(--sp) = 0xFFFFFFFD; 						//LR
	*(--sp) = 0x00000000; 						//R12
	*(--sp) = 0x00000000; 						//R3
	*(--sp) = 0x00000000; 						//R2
	*(--sp) = 0x00000000; 						//R1
	*(--sp) = 0x00000000; 						//R0

	/*	software stack registers*/
	*(--sp) = 0x0000000; 						//R11
	*(--sp) = 0x0000000; 						//R10
	*(--sp) = 0x0000000; 						//R9
	*(--sp) = 0x0000000; 						//R8
	*(--sp) = 0x0000000; 						//R7
	*(--sp) = 0x0000000; 						//R6
	*(--sp) = 0x0000000; 						//R5
	*(--sp) = 0x0000000; 						//R4	
	
	/*	initialize task parametres*/ 
	task_tcb->sp = sp;
	task_tcb->state = READY;
	task_ready[task_count] = *task_tcb;
	
	/* increment the task counter*/
	task_count++;
	current_index = task_count - 1;
}

void rtos_disable_task(task_handler_t *task_tcb)
{
    for(uint32_t i = 0; i < task_count; i++)
    {
        if(task_ready[i].task_func == task_tcb->task_func)
        {
            task_ready[i].state = DORMANT;
            break;
        }
    }
}

void rtos_enable_task(task_handler_t *task_tcb)
{
    for(uint32_t i = 0; i < task_count; i++)
    {
        if(task_ready[i].task_func == task_tcb->task_func)
        {
            task_ready[i].state = READY;
            break;
        }
    }
}

void rtos_schedule(void)
{
	/*
		Round-Robin scheduler.
		Walk through ALL tasks starting right after current_index,
		and pick the first one that is READY. If none are READY,
		fall back to keeping the current task running.
	*/

	for(uint32_t i = 0; i < task_count; i++)
	{
		current_index = (current_index + 1) % task_count;

		if(task_ready[current_index].state == READY)
		{
			next_task = &task_ready[current_index];
			return;
		}
	}

	/* No READY task found: stay on whatever is currently running */
	next_task = current_task;
}


void rtos_start(void)
{
	/*	get the nexet task*/
	rtos_schedule();
	
	/* up date current task*/
	current_task = next_task;
	/* load data old data in cpu reisters form stack*/
	__asm volatile
	(
	    /*	load satck pointer for task in cpu*/
		"LDR R0, =current_task		\n"       
		"LDR R0, [R0]				\n"                
		"LDR R0, [R0, #0]			\n"    
		"MSR PSP, R0				\n" 
	
	    /*	load thumb in spechale regiters*/
		"MRS R1, CONTROL			\n"       
		"ORR R1, R1, #2 			\n"       
		"MSR CONTROL, R1			\n" 
	
	    /* this load task satak in cpu */
		"POP {R4-R11}				\n" 
		"POP {R0-R3}				\n"
		"POP {R12}					\n"
		"POP {LR}					\n"
		"POP {PC}					\n"
	);
}

void rtos_dispatch(void)
{
	/* set up state the current task as excuting state*/
	current_task->state = EXCUTING;
	
	__asm volatile
	(
		/* save task in stack (cpu --> stack)*/
		"MRS   R0, PSP          \n"   //Move from Special Register
		"STMDB R0!, {R4-R11}    \n"   //STore Multiple, Decrement Before
		"MOV   R1, %0           \n"  
		"STR   R0, [R1]         \n"  

		/* restore task form stack (stack --> cpu)*/
		"MOV   R1, %1           \n"  
		"LDR   R0, [R1]         \n"  
		"LDMIA R0!, {R4-R11}    \n"    //LoaD Multiple, Increment After
		"MSR   PSP, R0          \n"    //Move to Special Register
		:
		: "r" (&current_task->sp), "r" (&next_task->sp)  
		: "r0", "r1", "memory"
	);
	/* up date the current task*/
	current_task->state = READY;
	current_task = next_task;
	current_task->state = READY;
}

void rtos_delay(uint32_t delay_ms)
{
	uint32_t start_tick = tick;
	current_task->state = BLOKED;
	
	while((tick - start_tick) < delay_ms)
	{
		;
	}
	
	current_task->state = EXCUTING;
}

void PendSV_Handler(void) 
{
	rtos_dispatch();	
}

void SysTick_Handler(void)
{
	/* calculation tiks*/
	tick++;
    /* sheduling the tasks*/
    rtos_schedule();

    /* trigger the pendsv*/
    NVIC_INT_CTRL_R |= NVIC_INT_CTRL_PEND_SV;
}