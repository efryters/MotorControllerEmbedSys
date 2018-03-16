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



int main(void)
{
    char letter = 'a';
    const char *greeting = "Hello World\0";
    const char *clrScreen = "Clearing scrn\0";
    const char *rstCursor = "Reset Cursor\0";

    init_i2c0();
    setup_lcd();
    backlight_on();
    clear_screen();
    cursor_home();

    set_cursor_pos(0,0);
    print_string_4bit_mode(greeting);
    set_cursor_pos(1,0);
    print_string_4bit_mode(clrScreen);
    set_cursor_pos(2,0);
    print_string_4bit_mode(rstCursor);

    int i = 0;

    while(1)
    {
        delayMs(5000);
        set_cursor_pos(0,0);
        print_string_4bit_mode(greeting);
        set_cursor_pos(1,0);
        print_string_4bit_mode(clrScreen);
        set_cursor_pos(2,0);
        print_string_4bit_mode(rstCursor);

        set_cursor_pos(3,0);
        for (i = 0; i < 20; i++)
        {
            delayMs(200);
            print_char_4bit_mode(letter);
            letter++;
            if (letter> 0x7E)
                letter = 0x21;
        }
        set_cursor_pos(3,19);
    }

    return 0;
}


