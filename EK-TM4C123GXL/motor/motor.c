/*
 * motor.c
 *
 *  Created on: Apr 17, 2018
 *      Author: Daniel C
 *      Modified: Eric F
 *      Borrowed code
 */


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"

#include"driverlib/sysctl.h"
#include"driverlib/pin_map.h"
#include"driverlib/gpio.h"
#include"driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "delay.h"
#include "motor.h"


//Our stepper motor has 50 steps per revolution, with the current configuration using the L293D (highly inefficient for stepper driver)
//We would prefer to use microstepping to greatly increase resolution of the steps.

void init_stepper(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);    // Enable port D
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    // Enable Port E
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
    GPIOPinTypeGPIOOutput(PORT_E,  GPIO_PIN_4 | GPIO_PIN_5);    // Set Port E pins 4,5 to outputs
    GPIOPinTypeGPIOOutput(PORT_D,  GPIO_PIN_2 | GPIO_PIN_3);    // Set Port D pins 2,3 to outputs
    GPIOPadConfigSet(PORT_D, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(PORT_E, GPIO_PIN_4 | GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

}

/*
//call this function to step CCW one time (1.8 degs)
void step_cw_once(void){

    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
        GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
        GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
        GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
        delayMs(1);
        GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
        GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
        GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
        GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
        delayMs(1);
        GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
        GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
        GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
        GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
        delayMs(1);
        GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
        GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
        GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
        GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
}

//call this function to step CW one time (1.8 degs)
void step_ccw_once(void){
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
            GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
            GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
            GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
            delayMs(1);
            GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
            GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
            GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
            GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
            delayMs(1);
            GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
            GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
            GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
            GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
            delayMs(1);
            GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
            GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
            GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
            GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
}
// call this function passing in the amount of degrees desired
void step_variable_x(uint32_t deg_x){
    unsigned char pos_x;
    pos_x = deg_x / 1.8;


}
*/

long duty = 50;
int waitMicroSeconds = 100;
int pulseCount = 10;

//
int step(int count, bool dir)
{
    const int list[] = {1,11,2,12,3,13,4,14};
    int i;
    int j;
    for (i = 0; i < count; i++){
        duty = 50;
        if (dir)
            {for (j = 0; j <= 7; j++){doStep(list[j]);}}
        else
            {for (j = 7; j >= 0; j--){doStep(list[j]);}}
    }
}

// main routine to microstep
void doStep(int st){

  long dt1 = waitMicroSeconds * duty / 100;
  long dt2 = waitMicroSeconds * (100-duty) / 100;
  int j;

  for (j = 0; j < pulseCount; j++){
    switch (st){
    case 1: oneA();break;
    case 2: twoA();break;
    case 3: threeA();break;
    case 4: fourA();break;
    case 11: oneB();break;
    case 12: twoB();break;
    case 13: threeB();break;
    case 14: fourB();break;

    case 21: oneA();break;
    case 22: twoA();break;
    case 23: threeA();break;
    case 24: fourA();break;
    case 31: oneB();break;
    case 32: twoB();break;
    case 33: threeB();break;
    case 34: fourB();break;

    }

    delayUs(dt1);

    switch (st){
    case 1: oneA();break;
    case 2: twoA();break;
    case 3: threeA();break;
    case 4: fourA();break;
    case 11: oneB();break;
    case 12: twoB();break;
    case 13: threeB();break;
    case 14: fourB();break;

    case 21: oneB();break;
    case 22: twoB();break;
    case 23: threeB();break;
    case 24: fourB();break;
    case 31: twoA();break;
    case 32: threeA();break;
    case 33: fourA();break;
    case 34: oneA();break;
    }
    delayUs(dt2);
  }
}

void oneA()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
}

void oneB()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
}

void twoA()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
}

void twoB()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
}
void threeA()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
}

void threeB()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
}
void fourA()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
}
void fourB()
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
}

/*
void step_ccw_once(void)
{
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, HIGH);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, LOW);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
    delayUs(250);
    GPIOPinWrite(PORT_E, GPIO_PIN_4, HIGH);
    GPIOPinWrite(PORT_D, GPIO_PIN_3, LOW);
    GPIOPinWrite(PORT_D, GPIO_PIN_2, HIGH);
    GPIOPinWrite(PORT_E, GPIO_PIN_5, LOW);
    delayUs(250);
}
*/
