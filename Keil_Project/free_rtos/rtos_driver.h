#ifndef RTOS_DRIVER_H
#define RTOS_DRIVER_H

#include <stdint.h>


/*
	this stract created to presente manging task
*/

typedef enum
{
	DORMANT,
	READY,
	EXCUTING,
	BLOKED
	
}task_state;

typedef struct 
{
    uint32_t *sp;             		/* Current Stack Pointer (must be first for ASM access) */
    void (*task_func)(void);  		/* Task entry point */
    uint32_t stack_size;      		/* Size of the stack in bytes */
	task_state state;               /* task state*/
	//uint32_t periode;             /* task preaode*/
	//uint32_t priority;			/* task pririty*/
}task_handler_t;

//==========================================================
//                   freertos APIs:
//==========================================================
void rtos_init(void);
void rtos_create_task(task_handler_t *task_tcb);
void rtos_disable_task(task_handler_t *task_tcb);
void rtos_enable_task(task_handler_t *task_tcb);
void rtos_schedule(void);
void rtos_start(void);
void rtos_dispatch(void);
void rtos_delay(uint32_t delay_ms);

#endif