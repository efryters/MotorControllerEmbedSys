/*
 * lcd_functions.c
 *
 *  Created on: Feb 16, 2018
 *      Author: Eric Fryters
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "lcd_functions.h"
#include "i2c_setup.h"
#include "delay.h"
#include <string.h>


/*
 * 1. init lcd in 4-bit mode (using full byte sends via i2c)
 * 2. after init, begin sending data via nibble mode.
 *
 */

char *greeting = "Hello World\0";

void setup_lcd(void)
{

    i2c_send(LCD_ADDR, 1, 0x00);
    delayMs(15);

    //enable LCD
    trig_en();
    i2c_send(LCD_ADDR, 1, 0x34);
    check_busy_flag();
    i2c_send(LCD_ADDR, 1, 0x00);
    delayUs(4500);


    trig_en();
    i2c_send(LCD_ADDR, 1, 0x34);
    check_busy_flag();
    i2c_send(LCD_ADDR, 1, 0x00);
    delayUs(200);


    trig_en();
    i2c_send(LCD_ADDR, 2, 0x34,0x24);
    //i2c_send(LCD_ADDR, 1, 0x24|BACKLIGHT);
    i2c_send(LCD_ADDR, 1, 0x00);
    //now in 4-bit interface mode...


    //function set, 2 lines, font set
    write_byte_4bit_mode(0x24, false);
    delayUs(50);
    //display off
    write_byte_4bit_mode(0x08, false);
    delayUs(50);
    //display clear
    write_byte_4bit_mode(0x01, false);
    delayUs(50);
    //set entry mode (increment by 1 after data in, with no disp shift)
    write_byte_4bit_mode(0x06, false);
    delayUs(50);

    delayMs(15);

    //Clear screen and return cursor home. Twice for reliablilty.
    write_byte_4bit_mode(0x01, false);
    write_byte_4bit_mode(0x02, false);
    write_byte_4bit_mode(0x01, false);
    write_byte_4bit_mode(0x02, false);
}

///Register select RS, true to select CGRAM
///Input is the 8-bit for D0-D7, as noted in datasheet for LCD
void write_byte_4bit_mode(uint8_t byte, bool rs)
{

    uint8_t rs_en;
    //with backlight, always
    if (rs)
    {
        //select register
        rs_en = RS|EN|BACKLIGHT;
    }
    else
    {
        //dont select register
        rs_en = EN|BACKLIGHT;
    }

    //shift the lower nibble to the front, then apply the RS and En
    uint8_t lower = byte << 4;
    lower |= rs_en;

    //clear out the lower nibble, then apply the RS and En
    uint8_t upper = byte;
    upper &= 0xF0;
    upper |= rs_en;

    //send our mangled bytes to i2c, then close enable, but leave on backlight
    i2c_send(LCD_ADDR, 1, rs_en);
    i2c_send(LCD_ADDR, 1, upper);
    check_busy_flag();
    i2c_send(LCD_ADDR, 1, lower);
    i2c_send(LCD_ADDR, 1, BACKLIGHT);
}

//Write a single character to the display
void print_char_4bit_mode(const char *c)
{
    write_byte_4bit_mode((uint8_t) c, true);
}

//Write a string (char array) to the display at the current cursor position
void print_string_4bit_mode(const char *c)
{
    int i = 0;
    int len = strlen(c);
    for (i = 0; i < len; i++)
    {
        write_byte_4bit_mode((uint8_t) *(c+i), true);
    }
}

//Check for busy flag and hold further execution until BF is cleared
void check_busy_flag()
{
    //read DB7, and if true, loop until
    uint32_t read = i2c_read(LCD_ADDR, 0x80);


    while(read != 0x80)
    {
        read = i2c_read(LCD_ADDR, 0x80);
    }
}

void backlight_on()
{
    i2c_send(LCD_ADDR, 1, BACKLIGHT);
}

void backlight_off()
{
    i2c_send(LCD_ADDR, 1, 0x00);
}

uint8_t get_cursor_pos()
{
    while(1);

}

void clear_screen()
{
    write_byte_4bit_mode(0x01, false);
}
void cursor_home()
{
    write_byte_4bit_mode(0x02, false);
}

void trig_en()
{
    i2c_send(LCD_ADDR, 1, BACKLIGHT);
    i2c_send(LCD_ADDR, 1, EN|BACKLIGHT);
    i2c_send(LCD_ADDR, 1, BACKLIGHT);
}

void cmd_lcd(uint8_t cmd)
{
    cmd |= BACKLIGHT;
    i2c_send(LCD_ADDR, 1, cmd);
}

