/*
 * menu.c
 *
 *  Created on: Apr 11, 2018
 *      Author: eric
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lcd/lcd_functions.h"
#include "menu.h"

/* LCD Menu 1 - Password prompt welcome screen */
const char *menu_p1l1 = "-Solar Controller-";
const char *menu_p1l2 = "Please input passwd";
const char *menu_p1l3 = "";
const char *menu_p1l4 = "";

/* LCD Menu 2 - Password accept, motor connect screen */
const char *menu_p2l1 = "-Motor Ctrl Connect-";
const char *menu_p2l2 = "Please connect";
const char *menu_p2l3 = "stepper motor";
const char *menu_p2l4 = "Press # to proceed";

/* LCD Menu 3 - Motor control screen 1 */
const char *menu_p3l1 = "-Motor Ctrl Step-";
const char *menu_p3l2 = "1 - Step CCW";
const char *menu_p3l3 = "3 - Step CW";
const char *menu_p3l4 = "0 - Change mode";

/* LCD Menu 4 - Motor control screen 2 */
const char *menu_p4l1 = "-Motor Ctrl Drive-";
const char *menu_p4l2 = "# - Direction";
const char *menu_p4l3 = "Amount:";
const char *menu_p4l4 = "0 - Change mode";


void menu_page1()
{
    clear_screen();
    print_string_justify(menu_p1l1, 1, 0);
    print_string_justify(menu_p1l2, 1, 1);
    print_string_justify(menu_p1l3, 0, 2);
    print_string_justify(menu_p1l4, 0, 3);
    cursor_off();

}

void menu_page2()
{
    clear_screen();
    print_string_justify(menu_p2l1, 1, 0);
    print_string_justify(menu_p2l2, 1, 1);
    print_string_justify(menu_p2l3, 1, 2);
    print_string_justify(menu_p2l4, 0, 3);
    cursor_off();
}

void menu_page3()
{
    clear_screen();
    print_string_justify(menu_p3l1, 1, 0);
    print_string_justify(menu_p3l2, 0, 1);
    print_string_justify(menu_p3l3, 0, 2);
    print_string_justify(menu_p3l4, 0, 3);
    cursor_off();
}

void menu_page4()
{
    clear_screen();
    print_string_justify(menu_p4l1, 1, 0);
    print_string_justify(menu_p4l2, 0, 1);
    print_string_justify(menu_p4l3, 0, 2);
    print_string_justify(menu_p4l4, 0, 3);
    cursor_off();
}
