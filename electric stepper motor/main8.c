#include "inc/tm4c123gh6pm.h"
#include <stdint.h> 
#include "stepper.h" 
#define T1ms 16000      // assumes using 16 MHz PIOSC (default setting // for clock source) 



int main(void){ 
	SYSCTL_RCGC2_R |= 0x10; //Activate Port E for switch  
	GPIO_PORTE_DIR_R &= ~0x10; 
	GPIO_PORTE_AMSEL_R &= ~0X01; // PE0 analog feature
	GPIO_PORTE_AFSEL_R &= ~0X01; // PE0 alternative features
	GPIO_PORTE_DEN_R |= 0x01; // PE0 Digital Enable
	GPIO_PORTE_PCTL_R &= ~0x0000000F; // clearing bits 
	
	
	
	
	
	
	Stepper_Init(); 
	Stepper_CCW(T1ms); // Pos=1; GPIO_PORTD_DATA_R=9 
	Stepper_CCW(T1ms); // Pos=2; GPIO_PORTD_DATA_R=5 
	Stepper_CCW(T1ms); // Pos=3; GPIO_PORTD_DATA_R=6 
	Stepper_CCW(T1ms); // Pos=4; GPIO_PORTD_DATA_R=10 
	Stepper_CCW(T1ms); // Pos=5; GPIO_PORTD_DATA_R=9 
	Stepper_CCW(T1ms); // Pos=6; GPIO_PORTD_DATA_R=5 
	Stepper_CCW(T1ms); // Pos=7; GPIO_PORTD_DATA_R=6 
	Stepper_CCW(T1ms); // Pos=8; GPIO_PORTD_DATA_R=10 
	while(1){
			if ((GPIO_PORTE_DATA_R &0x01) == 0x00){ 
				Stepper_CW(2*T1ms);
			}
		else
		Stepper_CCW(2*T1ms); // output every 10ms  
		} 
}