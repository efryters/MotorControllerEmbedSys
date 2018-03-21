/*
 * keypad.c
 *
 *  Created on: Mar 16, 2018
 *      Author: eric
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "keypad.h"


void Init_port(void)
{
    volatile uint32_t ui32Loop;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL| SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    printf("Waiting for Port C\n");
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    printf("Waiting for Port E\n");
    }

}
void Define_pins(void){

    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

}

