// DragRace.c
// Starter file for CECS346 Project 2
// By Dr. Min He
// 3/21/2024

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include <stdbool.h> // provides boolean data type
#include "PLL.h"
#include "SysTick.h"
#include "Sensors_Lights.h"

// Function Prototypes
// External functions from startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // Go to low power mode while waiting for the next interrupt

// Functions defined in this file.
void System_Init(void);

//TODO: Bit addresses for two sensors, 8 color lights, and one reset button 
#define SENSORS 								(*((volatile unsigned long *)0x4002400C)) // bit addresses for  2 sensors portE
#define LIGHTS              		(*((volatile unsigned long *)0x400053FC)) // bit addresses for 8 Race Lights portB
#define RESET                   (*((volatile unsigned long *)0x40007004)) // bit address for one reset button portD
 
// FSM 
struct State { 
	uint8_t Out;
	uint8_t Time;     // multiple of 0.5 second
	uint8_t Next[4];
};

typedef const struct State STyp;

// Constant definitions for FSM outputs

// TODO: define reload value for half second
#define HALF_SEC   12500000 //reload value for 25mhz clk 0.5s delay

//TODO: assign a value to all states in Drag Race FSM
enum DragRace_states {INIT,WFS,Y1,Y2,GO,FSL,FSR,FSB,WINL,WINR,WINB};

//TODO: Define Drag Race FSM
STyp DragRace_FSM[11] = {
{0xFF,2,{WFS,WFS,WFS,WFS}},
{0x00,1,{WFS,WFS,WFS,Y1}},
{0x11,1,{FSB,FSL,FSR,Y2}},
{0x22,1,{FSB,FSL,FSR,GO}},
{0x44,1,{WINB,WINL,WINR,GO}},
{0x08,2,{WFS,WFS,WFS,WFS}},
{0x80,2,{WFS,WFS,WFS,WFS}},
{0x90,2,{WFS,WFS,WFS,WFS}},
{0x04,2,{WFS,WFS,WFS,WFS}},
{0x40,2,{WFS,WFS,WFS,WFS}},
{0x44,2,{WFS,WFS,WFS,WFS}}
};

uint8_t S;  // current state index
uint8_t Input;
bool timesup;
bool reset;  // flag to reset the system, set by the reset button located at breadboard, not the launchpad reset button.
	
	
int main(void){
	LIGHTS = 0xFF;
	System_Init();
	//LIGHTS = 0xFF;
  while(1){
		while (!reset) {
			// TO Do: take care of FSM outputs and time in state.
			LIGHTS = DragRace_FSM[S].Out;
			SysTick_Start(DragRace_FSM[S].Time*HALF_SEC);
			while((!timesup)&&(!reset)){
			  WaitForInterrupt();
			}
			timesup=false;
			S = DragRace_FSM[S].Next[Input];
		}
  }
}

// Initialize the following hardware modules: PLL, Sensors, Reset button, Lights, SysTick timer
// Initialize the following golbal variables: timesup for FSM state time, reset for reset the whole system
void System_Init(void) {
	DisableInterrupts();
	PLL_Init();
  Sensors_Init(); 
	Reset_Init(); 
	Lights_Init();
	SysTick_Init(); 
  // TODO: assign Initial state, reset global variables: timesup and reset 
  S = INIT; //start in Initial state
	timesup = false;
	reset = 0;
	EnableInterrupts();
}

// Interrupt handler for the two sensors: take care of Input here 
void GPIOPortE_Handler(void){//falling or rising update the fsm
	if(GPIO_PORTE_RIS_R & 0x03) {		//if the source for sw2 is set PE0 - 1
		GPIO_PORTE_ICR_R |= 0x03;//acknowledge for port PE0 and PE1
		Input = SENSORS;//update input to be whatever sensor FROM PE0 - PE1
		
	}
}
// Interrupt handler for reset button: Resets the board to the initalization state: take care of global variable reset, timesup and S here 
void GPIOPortD_Handler(void) {
	if(GPIO_PORTD_RIS_R & 0x01) {		//if the source for sw2 is set PD0
		GPIO_PORTD_ICR_R |= 0x01;//acknowledge for port PD0
		S = INIT;//goes back to init state when button is pressed when interrupt is recieved
		reset = 0;
		timesup = false;
	}
}

// Systick interrupt handler: stop systick timer and take care of timesup 
void SysTick_Handler(void) {
	NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
	timesup = false;

}

