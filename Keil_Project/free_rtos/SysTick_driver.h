#ifndef SYSTICK_DRIVER_H
#define SYSTICK_DRIVER_H

#include <stdint.h>

#define INTERRUPT_DISABLE      0
#define INTERRUPT_ENABLE       1

#define PIOSC_D_4               0
#define SYSTEM_CLOCK            1

void sysTick_init(
	              uint8_t interruptOption, 
                  uint8_t clockSource, 
                  uint32_t reloadValue
                 );

void SysTick_Handler(void);

#endif