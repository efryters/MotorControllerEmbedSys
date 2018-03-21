#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "lcd/lcd_functions.h"
#include "i2c/i2c_setup.h"
#include "delay.h"
#include "keypad/keypad.h"



int main(void)
{

    init_i2c0();
    setup_lcd();
    backlight_on();
    clear_screen();
    cursor_home();
    init_keypad();

    set_cursor_pos(0, 0);

    while(1)
    {
        delayMs(500);
        print_char_4bit_mode( scan_keypad() );
    }

    return 0;
}


