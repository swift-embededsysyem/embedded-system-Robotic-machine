#include "inc/tm4c123gh6pm.h"
//task 1 turn on red LED
void init() {
	
	SYSCTL_RCGC2_R = 0x20; //Activate Port F
	
	int delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_DIR_R |= 0x08; //Port F1 is output
												   // 0 => input
												  // 1 => output
	GPIO_PORTF_DIR_R |= 0x08;
	GPIO_PORTF_AMSEL_R &= ~0x08;	//analog feature
	GPIO_PORTF_AFSEL_R &= ~0x08;	//alternative features 
	GPIO_PORTF_DEN_R |= 0x08;     // Digital Enable
}

	void delay() {
		int i = 1000000;
		while( i > 0){
			i--;
		}
	}
	int main() {
	while(1) {
		init();
		
		//GPIO_PORTF_DATA_R |= 0X0E; // turn on all colors
		//GPIO_PORTF_DATA_R |= 0X02; // PF 1, turn red on
		//GPIO_PORTF_DATA_R |= 0X04;// turn blue on
		GPIO_PORTF_DATA_R ^= 0X08; // turn green on
		//GPIO_PORTF_DATA_R ^= 0X0E;
		//delay();
		GPIO_PORTF_DATA_R ^= 0X04;
		delay();//
	}
	
	return 0;
	
}
