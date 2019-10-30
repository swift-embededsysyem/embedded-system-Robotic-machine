#include <stdint.h> 
#include "inc/tm4c123gh6pm.h" 
#include "Systick.h"
 
struct State{ 
	uint8_t Out; // Output port D 
	uint8_t Out2; // output port B
	const struct State *Next[2]; // CW/CCW 
	}; 
	typedef const struct State StateType;
	//typedef const struct State StateType2;

typedef StateType *StatePtr; 
	//? Anyone knows what is this code about? 
	#define clockwise 0        // index 
	#define counterclockwise 1 // Next index 
	StateType fsm[4]={        // D3 D2 D1 D0 
		{10, 10,{&fsm[1],&fsm[3]}}, // 1  0  1  0 
		{9,9,{&fsm[2],&fsm[0]}}, // 1  0  0  1 
		{5,5,{&fsm[3],&fsm[1]}}, // 0  1  0  1 
		{6,6,{&fsm[0],&fsm[2]}} // 0  1  1  0 
		}; 

	
		uint8_t Pos; // between 0 and 199 
		const struct State *
			Pt; // Current State

		

void Stepper_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x0A; // 1) activate port D & B
	SysTick_Init(); 
	Pos = 0; 
	Pt = &fsm[0]; 
	
	// 2) no need to unlock PD3-0 
	GPIO_PORTD_AMSEL_R &= ~0x0F; // 3) disable analog functionality on PD3-0 
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF; // 4) GPIO configure PD3-0 as GPIO 
	GPIO_PORTD_DIR_R |= 0x0F; // 5) make PD3-0 out 
	GPIO_PORTD_AFSEL_R &= ~0x0F; // 6) disable alt funct on PD3-0 
	GPIO_PORTD_DR8R_R |= 0x0F; // enable 8 mA drive 
	GPIO_PORTD_DEN_R |= 0x0F; // 7) enable digital I/O on PD3-0 
	//port PB3-0
	GPIO_PORTB_AMSEL_R &= ~0x0F; // 3) disable analog functionality on PD3-0 
	GPIO_PORTB_PCTL_R &= ~0x0000FFFF; // 4) GPIO configure PD3-0 as GPIO 
	GPIO_PORTB_DIR_R |= 0x0F; // 5) make PD3-0 out 
	GPIO_PORTB_AFSEL_R &= ~0x0F; // 6) disable alt funct on PD3-0 
	GPIO_PORTB_DR8R_R |= 0x0F; // enable 8 mA drive 
	GPIO_PORTB_DEN_R |= 0x0F; // 7) enable digital I/O on PD3-0
}


#define STEPPER  (*((volatile uint32_t *)0x4000703C)) //D
#define STEPPER2  (*((volatile uint32_t *)0x4000503C)) //B
	// What is 0x4000703C? 

// Move 1.8 degrees clockwise, delay is the time to wait after each step 
void Stepper_CW(uint32_t delay){ 
	Pt = Pt->Next[clockwise]; // circular 
	STEPPER = Pt->Out; // step motor #1
	STEPPER2 = Pt->Out2; // step motor #2 
	if(Pos==199){ // shaft angle 
		Pos = 0; // reset 
		} 
	else{ Pos++; // CW 
		} 
SysTick_Wait(delay); 
}

void Stepper_CCW(uint32_t delay){ 
	Pt = Pt->Next[counterclockwise]; // circular 
	STEPPER = Pt->Out; 
	STEPPER2 = Pt->Out2; // step motor 
	if(Pos==0){ // shaft angle 
		Pos = 199; // reset 
		} 
	else{ 
		Pos--; // CCW 
		} 
	SysTick_Wait(delay); // blind-cycle wait 
		}

		// (0 <= desired <= 199) 
		// time is the number of bus cycles to wait after each step 
		void Stepper_Seek(uint8_t desired, uint32_t time){ 
			short CWsteps; 
			if((CWsteps = (desired-Pos))<0){ 
				CWsteps+=200; 
				} // CW steps is 0 to 199 
			if(CWsteps > 100){ 
		while(desired != Pos){ 
		Stepper_CCW(time); 
		} 
		} 
		else{ 
		while(desired != Pos){ 
		Stepper_CW(time); 
	}
	} 
	}