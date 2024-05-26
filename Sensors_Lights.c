// Sensors_Lights.c
// Starter file for CECS346 Project 2
// By Dr. Min He
#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include "Sensors_Lights.h"

#define SENSOR_SW_MASK  0x03
#define LIGHT_MASK   0xFF
#define RESET_MASK 0x01

#define PORT_E_PRI 0x000000E0
#define PORTE_INT_PRI 2U
#define NVIC_EN0_PORTE 0x00000010

#define PORT_D_PRI 0xE0000000
#define PORTD_INT_PRI 1U
#define NVIC_EN0_PORTD 0x00000008

// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors
void Sensors_Init(void){//portE
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // activate port E
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){};
	
	GPIO_PORTE_DIR_R &= ~SENSOR_SW_MASK; // make PE0 - PE1 out (built-in blue LED) input
  GPIO_PORTE_AFSEL_R &= ~SENSOR_SW_MASK;// disable alt funct on PF3
  GPIO_PORTE_DEN_R |= SENSOR_SW_MASK; // enable digital I/O on PF3                           
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // configure PE0 - PE1 as GPIO
  GPIO_PORTE_AMSEL_R |= SENSOR_SW_MASK;   // disable analog functionality on PF3  
	GPIO_PORTE_IS_R &= ~SENSOR_SW_MASK;     // (d) PF0 is edge-sensitive
  GPIO_PORTE_IBE_R |= SENSOR_SW_MASK;    //     PE0 - PE1  both edges
  //GPIO_PORTE_IEV_R |= SENSOR_SW_MASK;    //      so set IEV PF0 rising edge event dontcare
  GPIO_PORTE_ICR_R |= SENSOR_SW_MASK;     // (e) clear flag PF0 - PE1
  GPIO_PORTE_IM_R |= SENSOR_SW_MASK;      // (f) arm interrupt on PE0 - PE1
	NVIC_PRI0_R = (NVIC_PRI0_R&~PORT_E_PRI)|PORTE_INT_PRI<<21; // (g) PORTF Interrupt priority bits: 23-21, priority set to 4
  NVIC_EN0_R |= NVIC_EN0_PORTE;     // (h) PORTF interrupt number is 30, enable bit 30 in NVIC. 
	
}


// Initialize the reset button: use level triggered interrupt
void Reset_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3; // activate port D
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R3)!=SYSCTL_RCGCGPIO_R3){};
	
	GPIO_PORTD_DIR_R &= ~RESET_MASK; // make PD0 al
  GPIO_PORTD_AFSEL_R &= ~RESET_MASK;// disable alt funct on PD0
  GPIO_PORTD_DEN_R |= RESET_MASK; // enable digital I/O on PD0                  
  GPIO_PORTD_PCTL_R &= ~0x0000000F; // configure PD0
  GPIO_PORTD_AMSEL_R |= RESET_MASK;   // disable analog functionality on PD0 
	GPIO_PORTD_IS_R |= RESET_MASK;     // (d) PD0 is level triggered
  GPIO_PORTD_IBE_R &= ~RESET_MASK;    //     PD0  cleared for 0
  GPIO_PORTE_IEV_R |= RESET_MASK;    //      so set IEV PD0 high level trigger
  GPIO_PORTD_ICR_R |= RESET_MASK;     // (e) clear flag PD0
  GPIO_PORTD_IM_R |= RESET_MASK;      // (f) arm interrupt on PD0
	NVIC_PRI1_R = (NVIC_PRI1_R&~PORT_D_PRI)|PORTD_INT_PRI<<21; // (g) PORTF Interrupt priority bits: 23-21, priority set to 4
  NVIC_EN0_R |= NVIC_EN0_PORTD;     // (h) PORTF interrupt number is 30, enable bit 30 in NVIC. 
}

// Initialize 8 LEDs
void Lights_Init(void){//PORT B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // activate port B
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R1)!=SYSCTL_RCGCGPIO_R1){};
	
	GPIO_PORTB_DIR_R |= LIGHT_MASK; // make PB0 - PB7 all 8 leds output
  GPIO_PORTB_AFSEL_R &= ~LIGHT_MASK;// disable alt funct on PB0 - PB7
  GPIO_PORTB_DEN_R |= LIGHT_MASK; // enable digital I/O on PB0 - PB7                    
  GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF; // configure PB0 - PB7
  GPIO_PORTB_AMSEL_R |= LIGHT_MASK;   // disable analog functionality on PF3  
}

