// SysTick.c
// Runs on TM4C123
// Starter file for CECS346 Project 2
// By Dr. Min He


// oscilloscope or LED connected to PF2 for period measurement

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types

void SysTick_Init(void) {	
	NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC; // set SysTick timer to use core clock: 50MHz
}

void SysTick_Start(uint32_t HALF_SEC) {	
	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = HALF_SEC-1;// reload value: if period = 4
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x1FFFFFFF)|0x60000000; // priority 3
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_ENABLE; // enable SysTick with core clock and interru
}

void SysTick_Stop(void) {
	NVIC_ST_CTRL_R = 0; // disable SysTick during setup
}