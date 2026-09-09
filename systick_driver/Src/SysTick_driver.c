#include "SysTick_driver.h"
#include "tm4c123gh6pm.h"

void sysTick_init(uint8_t interrupt_enable, uint8_t clock_source, uint32_t reload_value)
{
    NVIC_ST_CTRL_R = 0;

    NVIC_ST_RELOAD_R = (reload_value - 1) & NVIC_ST_RELOAD_M;

    NVIC_ST_CURRENT_R = 0;
	
	if(clock_source == SYSTEM_CLOCK)
	{
		NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;
	}
	if(interrupt_enable == INTERRUPT_ENABLE)
	{
		NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;
	}

    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}