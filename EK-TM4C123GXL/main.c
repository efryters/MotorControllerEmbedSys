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
#include "menu.h"
#include "motor/motor.h"

//SET PASSWORD HERE
#define PASSWORD 5555

/*
 * Prototypes
 */

void disp_main_menu();

int main(void)
{
    int pass;
    int rot;
    volatile char input;
    bool dir = false;

    init_i2c0();
    init_stepper();
    setup_lcd();
    backlight_on();
    clear_screen();
    cursor_home();
    init_keypad();


    while(1)
    {
        menu_page1();
        pass = get_ct_input(1, 2, true);
        if (pass == PASSWORD)
        {
            print_string_justify("Password Accepted!", 1, 3);
            delayMs(3000);
            menu_page2();
            while(get_input() != '#') {} //loop while waiting for "escape" key
            while(1)
            {
                menu_page3(); //Display motor controls
                while(get_input() != '0')
                {
                    input = get_input();
                    if(input == '1')
                    {
                        step(2, true);
                        //delayMs(1);
                    }
                    else if(input == '3')
                    {
                        step(2, false);
                        //delayMs(1);
                    }
                }

                menu_page4(dir);
                while(get_input() != '0')
                {
                    input = get_input();
                    if(input == '1')
                        {
                            dir = !dir;
                            menu_page4(dir);
                        }
                    else if(input == '2')
                    {
                        clear_line(2);
                        rot = get_ct_input(0, 2, false);
                        step(rot, dir);
                        menu_page4(dir);
                    }
                    else
                    {
                        step(rot, false);
                    }
                }
            }

        }
        else
        {
            print_string_justify("Wrong passwd!", 0, 3);
        }

    delayMs(5000);
    }

    return 0;
}

void disp_main_menu()
{
    clear_screen();
    print_string_justify("Solar Controller", 1, 0);
    print_string_justify("Please type passwd", 1, 1);
    print_string_justify("Push # to start...", 0, 2);
    cursor_off();
    while(get_input() != '#') {} //loop while waiting for "escape" key
    clear_line(2);
    //get_ct_input(1, 2);
    delayMs(1000*5);

}

