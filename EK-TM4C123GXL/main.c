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

    init_i2c0();
    setup_lcd();
    backlight_on();
    clear_screen();
    cursor_home();

    print_string_justify("Motor Control", 1, 0);
    print_string_justify("Main Menu", 1, 1);
    print_string_justify("TestTest", 2, 3);


    while(1)
    {
        print_busy_cursor(2, 10);

    }

    return 0;
}


