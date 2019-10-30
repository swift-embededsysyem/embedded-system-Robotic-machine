#include "inc/tm4c123gh6pm.h"
void Stepper_Init(void);

void Stepper_CW (uint32_t delay);

void Stepper_CCW (uint32_t delay);

void Stepper_Seek (uint8_t desired, uint32_t time);