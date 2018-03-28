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
    uint32_t test;

    init_i2c0();
    setup_lcd();
    backlight_on();
    clear_screen();
    cursor_home();
    init_keypad();


    while(1)
    {
        delayMs(1500);
        clear_screen();
        set_cursor_pos(0, 0);
        test = get_input();
        set_cursor_pos(1, 0);
        print_string_justify("Input accept!", 1, 1);
    }

    return 0;
}


