#include "rtos_driver.h"
#include "tm4c123gh6pm.h"
#include "SysTick_driver.h"

void red_led_task_func	(void);
void bleu_led_task_func	(void);
void sw1_task_func		(void);
void gpio_init			(void);


task_handler_t red_led_task;
task_handler_t bleu_led_task;
task_handler_t sw1_task;

int main(void) {
	gpio_init();
    rtos_init();
	
	red_led_task.stack_size 	= 	128;
	red_led_task.task_func 		= red_led_task_func;
    rtos_create_task			(&red_led_task);
	
	bleu_led_task.stack_size 	= 	128;
	bleu_led_task.task_func 	= bleu_led_task_func;
    rtos_create_task			(&bleu_led_task);

	sw1_task.stack_size 		= 	128;
	sw1_task.task_func 			= sw1_task_func;
    rtos_create_task			(&sw1_task);
	
    rtos_start();

    while(1)
	{
		;
	}
}

void gpio_init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;      
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0)
	{
		;
	}  

    GPIO_PORTF_DIR_R 	|= 	(1<<1) | (1<<2) | (1 << 3); 
    GPIO_PORTF_DEN_R 	|= 	(1<<1) | (1<<2) | (1 << 3);  
    GPIO_PORTF_DATA_R 	&= ~((1<<1)| (1<<2) | (1 << 3)); 
	
	GPIO_PORTF_DEN_R    |=  (1 << 4);
	GPIO_PORTF_DIR_R    &= ~(1 << 4);
	GPIO_PORTF_PUR_R    |=  (1 << 4);
}

void red_led_task_func(void)
{	 
	GPIO_PORTF_DATA_R |= (1<<1);
	
	while(1)
	{
		GPIO_PORTF_DATA_R ^= (1<<1);
		rtos_delay(250);
	}
}

void bleu_led_task_func(void)
{	 
	GPIO_PORTF_DATA_R |= (1<<2);
	
	while(1)
	{
		GPIO_PORTF_DATA_R ^= (1<<2);
		rtos_delay(1000);
	}
}

void sw1_task_func(void)
{	 
	while(1)
	{
		if((GPIO_PORTF_DATA_R & (1 << 4)) != (1 << 4))
		{
			GPIO_PORTF_DATA_R &= ~(1<<1);
			GPIO_PORTF_DATA_R &= ~(1<<2);
			rtos_disable_task(&red_led_task);
	        rtos_disable_task(&bleu_led_task);
		}
		else
		{
			rtos_enable_task(&red_led_task);
	        rtos_enable_task(&bleu_led_task);
		}
		GPIO_PORTF_DATA_R ^= (1<<3);
		rtos_delay(500);
	}
}



