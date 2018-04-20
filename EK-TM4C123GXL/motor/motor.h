/*
 * motor.h
 *
 *  Created on: Apr 17, 2018
 *      Author:
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

// Pin Defines
#define PORT_E GPIO_PORTE_BASE
#define PORT_D GPIO_PORTD_BASE

// Method Prototype
void init_stepper(void);
void step_cw_15(void);
void step_ccw_15(void);
void step_variable_x(uint32_t deg_x);
void step_ccw_once(void);
void step_cw_once(void);
int step(int count, bool dir);
void doStep(int st);

//steps
void oneA();
void twoA();
void threeA();
void fourA();
void oneB();
void twoB();
void threeB();
void fourB();

// Data Defines
#define HIGH 0xff
#define LOW 0x00

#endif /* MOTOR_MOTOR_H_ */
