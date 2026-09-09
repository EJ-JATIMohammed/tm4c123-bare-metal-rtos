# TM4C123GH6PM Bare-Metal RTOS

A lightweight, bare-metal real-time operating system (RTOS) implementation for the Texas Instruments TM4C123GH6PM microcontroller. This project demonstrates ARM Cortex-M4 assembly programming, task scheduling, and embedded systems concepts without relying on third-party OS frameworks.

## Overview

This project implements a simple preemptive, round-robin task scheduler for the TM4C123GH6PM MCU with integrated GPIO control and SysTick timer management. It serves as both an educational resource for learning bare-metal embedded systems programming and a foundation for more complex RTOS implementations.

**Key Features:**
- Custom RTOS kernel with round-robin scheduling
- SysTick timer driver with configurable clock sources
- GPIO driver for port configuration and control
- Task state management (DORMANT, READY, EXECUTING, BLOCKED)
- ARM Cortex-M4 context switching with assembly optimization

## Hardware

**Target Microcontroller:** TM4C123GH6PM (ARM Cortex-M4 @ 80 MHz)

**Demo Application:**
- Red LED: Toggles at 250 ms intervals
- Blue LED: Toggles at 1000 ms intervals  
- Green LED: Toggles at 500 ms intervals (status indicator)
- Push Button (SW1): Enables/disables red and blue LED tasks

All I/O uses GPIO Port F on the TM4C123GH6PM evaluation board.

## Project Structure

```
tm4c123-bare-metal-rtos/
├── rtos_driver/                    # RTOS kernel implementation
│   ├── Inc/
│   │   └── rtos_driver.h          # RTOS API and task control block definitions
│   └── Src/
│       └── rtos_driver.c          # RTOS scheduler and context switching
├── systick_driver/                 # SysTick timer driver
│   ├── Inc/
│   │   └── SysTick_driver.h       # SysTick API
│   ├── Src/
│   │   └── SysTick_driver.c       # SysTick hardware initialization
│   └── main.c                      # Demo application with LED tasks
├── Keil_Project/                   # Keil MDK-ARM project files
│   └── free_rtos/                  # Project configuration
├── LICENSE                         # MIT License
└── README.md                        # This file
```

## Core Components

### RTOS Kernel (`rtos_driver`)

The kernel provides:
- **Task Management:** Create, enable, and disable tasks with independent stacks
- **Scheduling:** Round-robin task scheduler invoked by SysTick timer
- **Context Switching:** ARM assembly routines using PSP (Process Stack Pointer) for task isolation
- **Delay Mechanism:** Task blocking with millisecond precision

**Task States:**
```c
DORMANT   // Task disabled
READY     // Task ready to execute
EXECUTING // Task currently running
BLOCKED   // Task waiting (in delay)
```

**Key RTOS APIs:**
```c
void rtos_init(void);                          // Initialize scheduler
void rtos_create_task(task_handler_t *task);   // Register new task
void rtos_start(void);                         // Launch first task
void rtos_schedule(void);                      // Schedule next ready task
void rtos_dispatch(void);                      // Context switch
void rtos_enable_task(task_handler_t *task);   // Resume task
void rtos_disable_task(task_handler_t *task);  // Pause task
void rtos_delay(uint32_t delay_ms);            // Block task for N ms
```

### SysTick Timer Driver (`systick_driver`)

Provides precise timing for the RTOS scheduler:
- Configurable clock source (PIOSC/4 or system clock)
- Configurable reload value for different tick frequencies
- Interrupt-driven operation

**Configuration Example (1 ms tick):**
```c
sysTick_init(INTERRUPT_ENABLE, PIOSC_D_4, 4000);
```

### GPIO Driver

Bare-metal GPIO initialization and control:
- Configures Port F for LED outputs and switch input
- Sets up digital enable and pull-up resistors
- Provides direct register access for task-level I/O control

## Development Environment

**IDE:** Keil MDK-ARM (μVision)

**Compiler:** ARM Compiler Toolchain

**Build Tools:**
- Arm compiler (armcc)
- Arm linker (armlink)
- Arm assembler (armasm)

### Prerequisites

1. **Keil MDK-ARM** installed with TM4C123GH6PM device support
2. **TM4C123 LaunchPad** evaluation board (or compatible hardware)
3. **USB to UART adapter** (for debugging, optional)

## Building and Running

### Build Steps

1. **Open the project in Keil:**
   ```bash
   Keil_Project/free_rtos/project.uvprojx
   ```

2. **Build the project:**
   - In Keil: Project → Build Target (or Ctrl+F7)

3. **Program the device:**
   - Connect TM4C123 LaunchPad via USB
   - In Keil: Flash → Download (or Ctrl+F8)
   - Select your debugger (OnBoard ICDI or external)

### Execution

Once programmed, the application starts automatically:
- LEDs will blink at their configured intervals
- Pressing SW1 toggles the red and blue LEDs on/off
- Green LED indicates RTOS scheduler activity

### Debugging

Use Keil's integrated debugger to:
- Step through code
- Set breakpoints in task functions or scheduler
- Inspect task stacks and global RTOS state
- Monitor SysTick timer interrupts

## Features

- **Bare-Metal Implementation:** No dependency on third-party RTOS frameworks (FreeRTOS, RTEMS, etc.)
- **Preemptive Scheduling:** SysTick timer triggers context switches every 1 ms
- **Multiple Task Support:** Up to 6 concurrent tasks with independent stacks
- **ARM Assembly Optimization:** Efficient context switching using inline assembly
- **Clean API:** Simple, intuitive RTOS interface for embedded applications
- **Portable Design:** Modular drivers can be adapted for other ARM Cortex-M MCUs

## Future Improvements

- **Task Priorities:** Implement priority-based scheduling (currently round-robin)
- **Task Periods:** Add periodic task support with deadline tracking
- **Mutex/Semaphore:** Synchronization primitives for inter-task communication
- **Message Queues:** Task-to-task data passing
- **Memory Management:** Dynamic heap allocation for variable stack sizes
- **Idle Task:** Power management with WFI (Wait For Interrupt)
- **FreeRTOS Integration:** Comparison/migration path to industry-standard RTOS
- **Extended GPIO Support:** Additional port drivers and peripheral abstraction
- **Timer Drivers:** PWM, input capture for advanced control applications

## References

- [TM4C123GH6PM Datasheet](https://www.ti.com/product/TM4C123GH6PM)
- [ARM Cortex-M4 Generic User Guide](https://developer.arm.com/documentation/100166/0001/)
- [Keil MDK-ARM Documentation](https://www2.keil.com/mdk5)

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Author

EJ-JATIMohammed

---

**Note:** This implementation is optimized for educational purposes and embedded systems portfolio demonstration. For production applications, consider using well-tested RTOS frameworks like FreeRTOS or RTX.
