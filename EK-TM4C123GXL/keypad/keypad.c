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
#include "../lcd/lcd_functions.h"

#include "keypad.h"

uint8_t prev;
void init_keypad(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}

    GPIOPinTypeGPIOInput(ROW_BASE, ROW_PINS);
    GPIOPadConfigSet(ROW_BASE, ROW_PINS, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeGPIOOutput(COL_BASE,  COL_PINS);
    GPIOPadConfigSet(COL_BASE, COL_PINS, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    prev = 0x00;
}


/*      +-------UPPER-------+------LOWER--------+
 *        08   04   02   01   08   04   02   01
 *      +----+----+----+----+----+----+----+----+
 *      | R2 | R3 | R4 | R1 | C3 | C1 | C2 | xx |
 * BIT7 +----+----+----+----+----+----+----+----+ BIT0
 *      | C7 | C6 | C5 | C4 | E3 | E2 | E1 | xx |
 *
 */


/**
 * Returns the key code for the button pressed, or 0xff is nothing pressed.
 */
uint8_t scan_keypad()
{
    uint8_t read;
    uint8_t key;
    key = 0x20; //Default to ascii space


    uint8_t upper;
    uint8_t lower;

    delayMs(80);
    //Read selected row
    GPIOPinWrite(COL_BASE, COL_PINS, 0x0E);
    upper = GPIOPinRead(ROW_BASE, ROW_PINS);

    //Switch pin roles and read selected column
    GPIOPinTypeGPIOOutput(ROW_BASE, ROW_PINS);
    GPIOPadConfigSet(ROW_BASE, ROW_PINS, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeGPIOInput(COL_BASE,  COL_PINS);
    GPIOPadConfigSet(COL_BASE, COL_PINS, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinWrite(ROW_BASE, ROW_PINS, upper);

    lower = GPIOPinRead(COL_BASE, COL_PINS);

    //Restore previous pin settings.
    GPIOPinTypeGPIOInput(ROW_BASE, ROW_PINS);
    GPIOPadConfigSet(ROW_BASE, ROW_PINS, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPinTypeGPIOOutput(COL_BASE,  COL_PINS);
    GPIOPadConfigSet(COL_BASE, COL_PINS, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    read = upper | lower;

    return read;
}

char get_char(uint8_t key_code)
{
    switch (key_code)
    {
        case KP_ASTERISK:
            return '*';
            break;
        case KP_POUND:
            return '#';
            break;
        case KP_ONE:
            return '1';
            break;
        case KP_TWO:
            return '2';
            break;
        case KP_THREE:
            return '3';
            break;
        case KP_FOUR:
            return '4';
            break;
        case KP_FIVE:
            return '5';
            break;
        case KP_SIX:
            return'6';
            break;
        case KP_SEVEN:
            return '7';
            break;
        case KP_EIGHT:
            return '8';
            break;
        case KP_NINE:
            return '9';
            break;
        case KP_ZERO:
            return '0';
            break;
        default:
            return 0xff;
    }
}

uint32_t get_input()
{
    bool accept;
    accept = false;
    uint32_t pass;
    pass = 0x00000000;
    char k = 0xff;

    while (!accept)
    {
        while (!accept)
        {
            k = get_char(scan_keypad());
            if (k != 0x2a)
            {
               //pass = pass << k;
               if (k!=0xff)
               {
                   print_char_4bit_mode(k);
                   delayMs(200);
               }
               }
            else
                accept = true;
        }
    }
    return pass;
}
