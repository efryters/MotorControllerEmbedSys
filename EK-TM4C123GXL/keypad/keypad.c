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


uint8_t prev;
void init_keypad(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    //printf("Waiting for Port C\n");
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    //printf("Waiting for Port E\n");
    }

    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    //GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,  GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    prev = 0x00;
}


/*
 *        08   04   02   01   08   04   02   01
 *      +----+----+----+----+----+----+----+----+
 *      | R1 | R2 | R3 | R0 | C2 | C0 | C1 | xx |
 * BIT7 +----+----+----+----+----+----+----+----+ BIT0
 *      | C7 | C6 | C5 | C4 | E3 | E2 | E1 | xx |
 *
 */

uint8_t scan_keypad()
{
    uint8_t read;
    uint8_t key;
    key = 0x20;


    uint8_t upper;
    uint8_t lower;

    //GET ROW
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0E);
    upper = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    //READ COLUMN
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,  GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, upper);

    lower = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Restore pin config
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    //GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,  GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    read = upper | lower;

    if (read != prev)
    {
        delayMs(40);

        switch (read)
        {
        case KP_ASTERISK:
            key = '*';
            break;
        case KP_POUND:
            key = '#';
            break;
        case KP_ONE:
            key = '1';
            break;
        case KP_TWO:
            key = '2';
            break;
        case KP_THREE:
            key = '3';
            break;
        case KP_FOUR:
            key = '4';
            break;
        case KP_FIVE:
            key = '5';
            break;
        case KP_SIX:
            key = '6';
            break;
        case KP_SEVEN:
            key = '7';
            break;
        case KP_EIGHT:
            key = '8';
            break;
        case KP_NINE:
            key = '9';
            break;
        case KP_ZERO:
            key = '0';
            break;
        default:
            key = 'x';

        }

    }

    return key;
}
